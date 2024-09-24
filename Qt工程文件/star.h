#ifndef STAR_H
#define STAR_H

#include "color.h"

#include <QFile>

#include <QMessageBox>

namespace Ui {
class star;
}

class star : public QDialog
{
    Q_OBJECT

public:
    explicit star(QWidget *parent = nullptr);
    ~star();

private slots:
    void on_star_switch_toggled(bool checked);
    void readSettingsFromFile(const QString &filePath);
    void handleColorSelected(const QColor &color);
    void on_chose_color_clicked();
    void writeSettingsToSTARcache(QString starswitch,QString starcolor,QString starmax,QString starmin,const QString &filePath);

    void on_pushButton_3_clicked();

private:
    Ui::star *ui;
    Color *colorPicker;  // 声明 Color 对象
    QString starswitch;
    QString starmin;
    void sons();
    QString path;
    QString starmax;
    QString starcolor;
};

#endif // STAR_H
