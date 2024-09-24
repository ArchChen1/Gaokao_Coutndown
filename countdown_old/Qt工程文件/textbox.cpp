#include "textbox.h"
#include "ui_textbox.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "editor.h"

textbox::textbox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::textbox)
{
    ui->setupUi(this);

    // 在构造函数内使用parentWidget()
    path = dynamic_cast<Editor*>(parentWidget())->getRealPath();
    readSettingsFromFile(path + "/cache/textbox_setting.txt");
}
textbox::~textbox()
{
    delete ui;
}

void textbox::readSettingsFromFile(const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        // 逐行读取文件内容
        BOXswitch = in.readLine();
        APIswitch = in.readLine();
        line1 = in.readLine();
        line2 = in.readLine();

        qDebug() << "BOXswitch: " << BOXswitch;
        qDebug() << "APIswitch: " << APIswitch;
        qDebug() << "line1: " << line1;
        qDebug() << "line2: " << line2;
        if (BOXswitch == "0") {
            ui->hideswitch->setCheckState(Qt::Checked);
        }

        if (APIswitch == "1") {
            ui->hikoto->setChecked(true);
        } else {
            ui->custom->setChecked(true);
        }

        ui->line1Edit->setText(line1);
        ui->line2Edit->setText(line2);

        file.close();
    } else {
        QMessageBox::critical(this, "Error", "无法打开文件 " + filePath + " 进行读取操作");
    }
}


void textbox::on_hikoto_toggled(bool checked) {
    if (checked) {
        APIswitch = "1";
        qDebug() << "APIswitch: " << APIswitch;

    }
}

void textbox::on_custom_toggled(bool checked) {
    if (checked) {
        APIswitch = "0";
        qDebug() << "APIswitch: " << APIswitch;

    }
}


void textbox::on_hideswitch_toggled(bool checked)
{
    if (checked) {
        BOXswitch = "0";
        qDebug() << "BOXswitch: " << BOXswitch;

    }else{
        BOXswitch = "1";
        qDebug() << "BOXswitch: " << BOXswitch;
    }
}


void textbox::on_pushButton_clicked()
{
    line1 = ui->line1Edit->text();
    line2 = ui->line2Edit->text();
    qDebug() << "BOXswitch: " << BOXswitch;
    qDebug() << "APIswitch: " << APIswitch;
    qDebug() << "line1: " << line1;
    qDebug() << "line2: " << line2;
    writeSettingsToFile(path + "/cache/textbox_setting.txt");

    close();
}

void textbox::writeSettingsToFile(const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);

        // 依次写入变量的值，每个变量独占一行
        out << BOXswitch << Qt::endl;
        out << APIswitch << Qt::endl;
        out << line1 << Qt::endl;
        out << line2 << Qt::endl;
        file.close();
    } else {
        QMessageBox::critical(this, "Error", "无法打开文件 " + filePath + " 进行写入操作");
    }
}
