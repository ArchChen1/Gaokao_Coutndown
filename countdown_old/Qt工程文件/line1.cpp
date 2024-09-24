// line1.cpp
#include "line1.h"
#include "ui_line1.h"
#include "color2.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "plus.h"
#include "editor.h"


line1::line1(int lineNumber, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::line1),
    colorPicker(new Color(this)),  // 第一个颜色选择器
    colorPicker2(new Color2(this)),  // 第二个颜色选择器
    lineNumber(lineNumber) // Initialize the lineNumber member variable

{
    ui->setupUi(this);
    path = dynamic_cast<Editor*>(parentWidget())->getRealPath();
    // 设置
    readLineFromCache(lineNumber);

    colorPicker->setCurrentColor(QColor(text_color));
    colorPicker2->setCurrentColor(QColor(shadow_color));
    ui->textcolor_space->setText(text_color);
    ui->font_familybox->setCurrentFont(font_family);
    ui->font_sizebox->setText(font_size);
    ui->font_weightbox->setText(font_weight);
    ui->offsetxbox->setText(offsetx);
    ui->offsetybox->setText(offsety);
    ui->radiusbox->setText(radius);
    ui->shadowcolor_space->setText(shadow_color);

    // 将第一个颜色选择按钮添加到 pushButton_3 的父容器中
    colorPicker->getColorButton()->setParent(ui->pushButton_3);
    colorPicker->setFixedSize(ui->pushButton_3->size());
    // 连接第一个颜色选择信号槽
    connect(colorPicker, &Color::colorSelected, this, &line1::handleColorSelected);

    // 将第二个颜色选择按钮添加到 pushButton_4 的父容器中
    colorPicker2->getColorButton()->setParent(ui->pushButton_4);
    colorPicker2->setFixedSize(ui->pushButton_4->size());

    // 连接第二个颜色选择信号槽
    connect(colorPicker2, &Color2::colorSelected, this, &line1::handleColorSelected2);
}

line1::~line1()
{
    delete ui;
}

void line1::readLineFromCache(int lineNumber)
{
    QFile file(path + "/cache/text_setting.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "打开文件" << path<<"/cache/text_setting.txt失败";
        return;
    }

    QTextStream in(&file);
    int currentLine = 1;

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (currentLine == lineNumber) {
            // 处理可能存在的额外引号和斜杠
            line.remove(QRegularExpression("[\"\\[\\]]")).remove("rem");

            // 解析行为样式元素
            QStringList stylesList = line.split(", ", Qt::SkipEmptyParts);
            if (stylesList.size() == 5) { // 确保匹配预期元素数量
                // 将样式元素赋值给相应的变量
                font_family = stylesList[0];

                // 去掉 font_size 中的引号和 "rem"
                font_size = stylesList[1].replace("\"", "").replace("rem", "");

                // 处理字重的字符串
                font_weight = stylesList[2].replace("rem", "");

                // 处理阴影字符串
                QStringList shadowValues = stylesList[3].split(" ", Qt::SkipEmptyParts);
                if (shadowValues.size() == 4) {
                    offsetx = shadowValues[0];
                    offsety = shadowValues[1];
                    radius = shadowValues[2];
                    shadow_color = shadowValues[3];
                } else {

                }

                text_color = stylesList[4];
                // 用于调试的输出结果
                // qDebug() << "字体族:" << font_family;
                // qDebug() << "字体大小:" << font_size;
                // qDebug() << "字重:" << font_weight;
                // qDebug() << "横向偏移:" << offsetx;
                // qDebug() << "纵向偏移:" << offsety;
                // qDebug() << "半径:" << radius;
                // qDebug() << "阴影颜色:" << shadow_color;
                // qDebug() << "文本颜色:" << text_color;

                file.close();
                return;
            } else {
                file.close();
                return;
            }
        }
        currentLine++;
    }

    file.close();
}

void line1::on_pushButton_3_clicked()
{
    // 获取第一个颜色并转为字符串
    text_color = colorPicker->getColor().name();
    // 更新第一个颜色选择按钮的当前颜色为 text_color
    colorPicker->setCurrentColor(QColor(text_color));
}

void line1::on_pushButton_4_clicked()
{
    // 获取第二个颜色并转为字符串
    text_color = colorPicker->getColor().name();
    // 更新第二个颜色选择按钮的当前颜色为 text_color
    colorPicker2->setCurrentColor(QColor(shadow_color));
}

void line1::handleColorSelected(const QColor &color)
{
    // 在这里处理第一个颜色选择事件，比如更新UI或执行其他操作
    // 更新 text_color 变量
    text_color = color.name();
    qDebug() << "Selected Color:" << text_color;
    ui->textcolor_space->setText(text_color);
}

void line1::handleColorSelected2(const QColor &color)
{
    // 在这里处理第二个颜色选择事件，比如更新UI或执行其他操作
    // 更新 text_color 变量
    shadow_color = color.name();
    qDebug() << "Selected Color 2:" << shadow_color;
    ui->shadowcolor_space->setText(shadow_color);
}

void line1::on_pushButton_clicked()
{
    text_color = ui->textcolor_space->text();
    font_family = ui->font_familybox->currentFont().family();
    font_size = ui->font_sizebox->text();
    font_weight = ui->font_weightbox->text();
    offsetx = ui->offsetxbox->text()+"rem";
    offsety = ui->offsetybox->text()+"rem";
    radius = ui->radiusbox->text()+"rem";
    shadow_color = ui->shadowcolor_space->text();
    QString line = plus(font_family, font_size, font_weight, offsetx, offsety, radius, shadow_color, text_color);

    qDebug() << line;
    writeStylesToCacheFile(path + "/cache/text_setting.txt", line, lineNumber);
    close();

}

void line1::writeStylesToCacheFile(const QString& filePath, const QString& line, int lineNumber)
{
    text_color = ui->textcolor_space->text();
    shadow_color = ui->shadowcolor_space->text();
    QFile file(filePath);

    if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        // 读取原文件内容
        QString content = file.readAll();
        QTextStream out(&file);

        // 将原文件内容按行分割
        QStringList lines = content.split("\n", Qt::SkipEmptyParts);

        // 替换指定行
        if (lineNumber > 0 && lineNumber <= lines.size()) {
            lines[lineNumber - 1] = line; // 数组索引从 0 开始，行号从 1 开始
        } else {
            file.close();
            return;
        }

        // 将修改后的内容写回文件
        file.seek(0);
        out << lines.join("\n") << "\n";
        file.close();
    } else {
        qDebug() << "无法打开文件" << filePath << "进行读写操作";
    }
}
