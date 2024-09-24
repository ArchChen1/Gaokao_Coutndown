// line1.h
#ifndef LINE1_H
#define LINE1_H

#include <QDialog>
#include "color.h"
#include "color2.h"

namespace Ui {
class line1;
}

class line1 : public QDialog
{
    Q_OBJECT

public:
    void setLines(int newLines);
    explicit line1(int lineNumber, QWidget *parent = nullptr);
    ~line1();
    void readLineFromCache(int lineNumber);

private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void handleColorSelected(const QColor &color);
    void handleColorSelected2(const QColor &color);
    void on_pushButton_clicked();

private:
    Ui::line1 *ui;
    Color *colorPicker;
    Color2 *colorPicker2;
    QString font_family;
    QString font_size;
    void sons();
    QString path;
    QString font_weight;
    QString offsetx;
    QString offsety;
    QString radius;
    QString shadow_color;
    QString text_color;
    int lineNumber; // Added member variable to store lineNumber
    void writeStylesToCacheFile(const QString& filePath, const QString& line, int lineNumber);


};

#endif // LINE1_H
