// textbox.h

#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <QDialog>

namespace Ui {
class textbox;
}

class textbox : public QDialog
{
    Q_OBJECT

public:
    explicit textbox(QWidget *parent = nullptr);
    ~textbox();
    void writeSettingsToFile(const QString &filePath);

private slots:
    void on_pushButton_clicked();

    void on_hikoto_toggled(bool checked);

    void on_custom_toggled(bool checked);

    void on_hideswitch_toggled(bool checked);

private:
    Ui::textbox *ui;
    QString BOXswitch;
    QString APIswitch;
    QString line1;
    QString line2;
    QString path;
    void sons();
    void readSettingsFromFile(const QString &filePath);
    void do_API();
};

#endif // TEXTBOX_H
