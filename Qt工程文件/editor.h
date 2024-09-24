#ifndef EDITOR_H
#define EDITOR_H
#include "color.h"
#include "color2.h"
#include "color3.h"
#include <QTimer>
#include <QTextCodec>
#include <QLabel>
#include <QWidget>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include <QRegularExpression>
#include <QDir>
#include <QFileInfoList>
#include <QStringList>
#include <QQmlEngine>
#include <QQmlContext>
#include <QJSEngine>
#include <QJSValue>
#include <QMessageBox>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPushButton>
#include <QCoreApplication>
#include <QWebEnginePage>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QUrl>
#include <QDesktopServices>
#include <QTcpServer>
#include <QTextStream>
#include <QTcpSocket>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui {
class Editor;
}
QT_END_NAMESPACE

class Editor : public QWidget
{
    Q_OBJECT

public:
    QString getRealPath() const { return real_path; }
    Editor(QWidget *parent = nullptr);
    ~Editor();
    static const QRegularExpression arrayRegex;


private slots:
    void handleNewConnection();
    void handleHttpRequest(QTcpSocket *socket);
    void settime();
    void on_pushButton_18_clicked();
    void on_pushButton_17_clicked();
    void on_line1_clicked();
    void on_line2_text_clicked();
    void on_line2_date_clicked();
    void on_line2_num_clicked();
    void on_line3_text_clicked();
    void on_line3_num_clicked();
    void on_line3_gaokao_clicked();
    void on_line4_text_clicked();
    void on_line4_num_clicked();
    void on_line4_text_lite_clicked();
    void on_line4_num_lite_clicked();
    void on_textbox_clicked();
    void on_setday_toggled(bool checked);
    void on_textbox_3_clicked();
    void on_textbox_2_clicked();
    void on_open_file_clicked();
    void on_add_file_clicked();
    void on_chosebgcolor_clicked();
    void on_bg_vdo_mute_toggled(bool checked);
    void on_star_Setting_clicked();
    void on_save_schedule_clicked();
    void on_check_update_clicked();
    void on_check_update_released();
    void on_checkBox_toggled(bool checked);
    void on_Timer1_switch_toggled(bool checked);
    void on_Timer2_switch_toggled(bool checked);
    void on_Waitload_switch_toggled(bool checked);
    void on_save_clicked();
    void on_save_released();
    void on_textBrowser_anchorClicked(const QUrl &arg1);

private:
    Ui::Editor *ui;
    QTcpServer *tcpServer;
    QString htmlContent;
    void relaodcombobox();
    void setupHttpServer();
    void readAndSaveStyles();
    void writeStylesToFile(const QStringList& styles);
    void updateTimerJS(const QString& settingFilePath, const QString& jsFilePath) ;
    void updateScheduleJS(const QString& scheduleFilePath, const QString& jsFilePath);
    void updateCoutdownJS(const QString& settingFilePath, const QString& jsFilePath);
    void writeTextToJS(const QString& textFilePath, const QString& jsFilePath);
    void handleColorSelected(const QColor &color);
    void handleColorSelected2(const QColor &color);
    void handleColorSelected3(const QColor &color);
    void initializeComboBox();
    void onComboBoxIndexChanged(int index);
    void initializeComboBox_vdo();
    void onComboBoxIndexChanged_vdo(int index);
    void textboxreadandwrite();
    void countdowndayreadandwrite();
    void sendPostRequestToTargetPage();
    QStringList selectors;
    void onTabChanged(int index);
    void writeSettingsToFile(QStringList date1,QString DATEswitch,const QString &filePath);
    void updateTextBoxJS(const QString& settingFilePath, const QString& jsFilePath);
    void writeSettingsToBGcache(QString bg_config,QString sound_config,QString bgcolor,QString imageSRC,QString videoSRC,QString customeAPI,QString defaultIMG,const QString &filePath);
    void BGreadandwrite();
    void copyFile(const QString& sourceFilePath, const QString& destinationFilePath);
    void readScheduleFromFile(const QString &filePath);
    void STARreadandwrite();
    void writeScheduleToFile(const QStringList &scheduleList);
    void writeRemainingVariables(const QString& scheduleFilePath);
    void updateWaitLoadJS(const QString& settingFilePath, const QString& jsFilePath);
    void updateStarJS(const QString& settingFilePath, const QString& jsFilePath);
    void assignScheduleToLineEdit();
    void assignLineEditToSchedule();
    void auto_check_update();
    void save_other();
    void save_bg();
    void sons();
    QString paths;
    QString real_path;
    float current_version = 1.2;
    void updateBackgroundJS(const QString& configFilePath, const QString& jsFilePath);
    QLabel *label;  // 声明 QLabel 类型的成员变量
    QLabel *imageLabel;  // QLabel 指针
    Color *colorPicker;  // 声明 Color 对象
    Color2 *colorPicker2;  // 声明 Color 对象
    Color3 *colorPicker3;  // 声明 Color 对象
    QWidget *imageTab;   // QWidget 指针
    QString imgSRC;
    QString vdoSRC;
    QQmlEngine qmlEngine;
    QString DATEswitch;
    QString settingDAY;
    QString year ;
    QString month;
    QString day;
    QString hour;
    QString minute;
    QString second;
    QString bg_config;
    QString sound_config;
    QString bgcolor;
    QString imageSRC;
    QString defaultimageSRC;
    QString videoSRC;
    QString customeAPI;
    QString defaultIMG;
    QString starswitch;
    QString starmin;
    QString starmax;
    QString starcolor;
    QString s_positionMODE;
    QString s_positionX;
    QString s_positionY;
    QString s_shadowSIZE;
    QString s_opacity;
    QString s_gapSIZE;
    QString s_order;
    QString s_hide;
    QString s_colorAM;
    QString s_colorPM;
    QString mediator1;
    QString mediator2;
    QString mediator3;
    QString mediator4;
    QString mediator5;
    QString mediator6;
    QString mediators[7];
    QStringList course[16];
    int auto_check_update_status;
    int timer1_switch;
    int timer2_switch;
    //Timer 1
    int aimhours;
    int aimminutes;
    int aimseconds;
    //Timer 2
    int aimhours2;
    int aimminutes2;
    int aimseconds2;

    int wait_switch;
    int fadetime;
    int outtime;

    void readcourseFromCache(int lineNumber);
    void writeToSCHEDULEcache(QStringList course[15],const QString &filePath);
    void TIMERreadandwrite();
    void WaitLoadreadandwrite();

};
#endif // EDITOR_H
