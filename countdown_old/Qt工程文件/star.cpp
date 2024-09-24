#include "star.h"
#include "ui_star.h"
#include "editor.h"

star::star(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::star)
{
    colorPicker = new Color(this);

    ui->setupUi(this);
    path = dynamic_cast<Editor*>(parentWidget())->getRealPath();

    readSettingsFromFile(path + "/cache/star_setting.txt");

    colorPicker->getColorButton()->setParent(ui->chose_color);
    colorPicker->setFixedSize(ui->chose_color->size());
    connect(colorPicker, &Color::colorSelected, this, &star::handleColorSelected);

    colorPicker->setCurrentColor(QColor(starcolor));
    ui->star_color->setText(starcolor);

}

star::~star()
{
    delete ui;
}

void star::readSettingsFromFile(const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        // 逐行读取文件内容
        starswitch = in.readLine();
        starcolor = in.readLine();
        starmax = in.readLine();
        starmin = in.readLine();

        if (starswitch == "1") {
            ui->star_switch->setCheckState(Qt::Checked);
        }

        ui->starmax->setText(starmax);
        ui->starmin->setText(starmin);

        file.close();
    } else {
        QMessageBox::critical(this, "Error", "无法打开文件 " + filePath + " 进行读取操作");
    }
}

void star::on_chose_color_clicked()
{
    // 获取第一个颜色并转为字符串
    starcolor = colorPicker->getColor().name();
    // 更新第一个颜色选择按钮的当前颜色为 text_color
    colorPicker->setCurrentColor(QColor(starcolor));
}
void star::handleColorSelected(const QColor &color)
{
    // 在这里处理第一个颜色选择事件，比如更新UI或执行其他操作
    // 更新 text_color 变量
    starcolor = color.name();
    qDebug() << "Selected Color:" << starcolor;
    ui->star_color->setText(starcolor);
}

void star::on_star_switch_toggled(bool checked)
{
    if (checked) {
        starswitch = "1";
        qDebug() << "starswitch" << starswitch;
    }else{
        starswitch = "0";
        qDebug() << "starswitch" << starswitch;

    }
}

void star::writeSettingsToSTARcache(QString starswitch,QString starcolor,QString starmax,QString starmin,const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        // 依次写入变量的值，每个变量独占一行
        out << starswitch << Qt::endl;
        out << starcolor << Qt::endl;
        out << starmax << Qt::endl;
        out << starmin << Qt::endl;

        file.close();
    } else {
        QMessageBox::critical(this, "Error", "无法打开文件 " + filePath + " 进行写入操作");
    }
}

void star::on_pushButton_3_clicked()
{
    starmax = ui->starmax->text();
    starmin = ui->starmin->text();
    writeSettingsToSTARcache(starswitch, starcolor, starmax, starmin,path + "/cache/star_setting.txt");
    close();
}

