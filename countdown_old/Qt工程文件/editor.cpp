//主窗口
#include "editor.h"
#include "ui_editor.h"
#include "line1.h"
#include "textbox.h"
#include "star.h"


Editor::Editor(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::Editor),
    colorPicker(new Color(this)),  // 第一个颜色选择器
    colorPicker2(new Color2(this)),  // 第二个颜色选择器
    colorPicker3(new Color3(this))  // 第二个颜色选择器


{
    ui->setupUi(this);
    tcpServer = new QTcpServer(this);
    QTimer *timer1 = new QTimer(this);
    connect(timer1, &QTimer::timeout, this, &Editor::initializeComboBox_vdo);
    timer1->start(1000);  // 1000毫秒 = 1秒

    QTimer *timer2 = new QTimer(this);
    connect(timer2, &QTimer::timeout, this, &Editor::initializeComboBox);
    timer2->start(1000);  // 1000毫秒 = 1秒
    settime();
    setupHttpServer();
    sons();
    sendPostRequestToTargetPage();
    initializeComboBox();
    initializeComboBox_vdo();
    readAndSaveStyles();
    textboxreadandwrite();
    countdowndayreadandwrite();
    BGreadandwrite();
    STARreadandwrite();
    readScheduleFromFile(real_path + "/files/js/Schedule.js");
    auto_check_update();
    TIMERreadandwrite();
    WaitLoadreadandwrite();
    // 初始化 selectors
    selectors << "xianzaishiWENZI" << "line2text" << "Now_number" << "time"
              << "line3text" << "line3num" << "gaokao" << "Line4text"
              << "Line4num" << "Line4text_lite" << "Line4num_lite";
    //第一颜色选择器
    colorPicker->getColorButton()->setParent(ui->chosebgcolor);
    colorPicker->setFixedSize(ui->chosebgcolor->size());
    colorPicker->setCurrentColor(QColor(bgcolor));
    connect(colorPicker, &Color::colorSelected, this, &Editor::handleColorSelected);
    //第二颜色选择器
    colorPicker2->getColorButton()->setParent(ui->coloram_btn);
    colorPicker2->setFixedSize(ui->coloram_btn->size());
    colorPicker2->setCurrentColor(QColor(s_colorAM));
    connect(colorPicker2, &Color2::colorSelected, this, &Editor::handleColorSelected2);
    //第三颜色选择器
    colorPicker3->getColorButton()->setParent(ui->colorpm_btn);
    colorPicker3->setFixedSize(ui->colorpm_btn->size());
    colorPicker3->setCurrentColor(QColor(s_colorPM));
    connect(colorPicker3, &Color3::colorSelected, this, &Editor::handleColorSelected3);
    // 设置打开外部链接的策略
    ui->textBrowser->setOpenExternalLinks(true);
    // 设置边框隐藏和背景透明
    ui->textBrowser->setStyleSheet("QTextBrowser { border: none; background: transparent; }");
    // 设置边框隐藏和背景透明
    // 连接链接点击事件的槽函数
    connect(ui->textBrowser, &QTextBrowser::anchorClicked, this, &Editor::on_textBrowser_anchorClicked);
    // 创建 QLabel，并加载图片

    imageLabel = new QLabel(ui->tabWidget);  // 替换为你的实际的 TabWidget 变量名
    QPixmap image(":/bg/OIP.png");  // 替换为你的图片路径
    int scaledWidth = image.width() * 0.25;
    int scaledHeight = image.height() * 0.25;
    image = image.scaled(scaledWidth, scaledHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imageLabel->setPixmap(image);
    // 获取 TabWidget 的第五个（索引为4）标签页
    QWidget *tabWidget = ui->tabWidget->widget(4);

    // 创建一个新的垂直布局，并将 QLabel 添加到布局中
    QVBoxLayout *layout = new QVBoxLayout(tabWidget);
    layout->addWidget(imageLabel, 0);  // 仅传递小部件和伸缩因子
    layout->setAlignment(Qt::AlignBottom | Qt::AlignRight);  // 设置布局的对齐方式
    imageLabel->setGeometry(530, 205, 128, 128);
    // 将 QLabel 放在所有元素的后面
    imageLabel->lower();

}

Editor::~Editor()
{
    delete ui;
    delete tcpServer;

}

void Editor::setupHttpServer()
{
    connect(tcpServer, &QTcpServer::newConnection, this, &Editor::handleNewConnection);

    if (!tcpServer->listen(QHostAddress::Any, 30518)) {
        qWarning("Failed to start server. Error: %s", qPrintable(tcpServer->errorString()));
    } else {
        qDebug("Server is running on port 30518.");
    }
}

void Editor::handleNewConnection()
{
    while (tcpServer->hasPendingConnections()) {
        QTcpSocket *socket = tcpServer->nextPendingConnection();
        connect(socket, &QTcpSocket::readyRead, this, [this, socket]() { handleHttpRequest(socket); });
    }
}

void Editor::handleHttpRequest(QTcpSocket *socket)
{
    QByteArray requestData = socket->readAll();

    // HTTP response headers
    QString httpResponse = "HTTP/1.1 200 OK\r\n";
    httpResponse += "Content-Length: " + QByteArray::number(htmlContent.toUtf8().size()) + "\r\n";
    httpResponse += "Content-Type: text/html\r\n\r\n";

    // Send the HTTP response
    socket->write(httpResponse.toUtf8());
    socket->write(htmlContent.toUtf8());
    socket->waitForBytesWritten();
    socket->disconnectFromHost();
}

void Editor::settime()
{
    // 获取当前时间
    QDateTime currentDateTime = QDateTime::currentDateTime();

    // 将当前时间转换为字符串
    QString currentTimeString = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
    QString timeline = "var time = \"%1\";";
    timeline = timeline.arg(currentTimeString);
    // QString jquery = "<script src=\"%1\"></script>>";
    // jquery = jquery.arg("/files/js/Timer.js");
    htmlContent = "<!DOCTYPE html>"
                  "<html lang='en'>"
                  "<head>"
                  "<meta charset='UTF-8'>"
                  "<title>Child Page</title>"
                  // "<script src='https://code.jquery.com/jquery-3.7.1.js'></script><!--jquery-->"
                  "</head>"
                  "<body>"
                  "<p id='time'></p>"
                  "<script>" +
                  timeline +
                  "document.getElementById('time').innerHTML = time;"
                  "window.parent.postMessage(time, '*');"
                  "</script>"
                  "</body>"
                  "</html>";
    qDebug() <<"更新服务器时间";
}

void Editor::onTabChanged(int index)
{
    (void)index;
    imageLabel->lower();

}
void Editor::on_check_update_clicked() {}
void Editor::on_save_clicked() {}

void Editor::textboxreadandwrite() {
    // 读取 JavaScript 文件内容
    QFile inputFile(real_path + "/files/js/TextBox.js");
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&inputFile);
        QString jsCode = in.readAll();
        inputFile.close();

        // 使用 QQmlEngine 解析 JavaScript 代码
        QQmlEngine engine;
        QQmlContext context(engine.rootContext());

        // 设置上下文属性，确保 JavaScript 代码中可以访问到 editor 对象
        context.setContextProperty("editor", this);

        // 使用 QML 引擎执行 JavaScript 代码
        engine.evaluate(jsCode);

        // 可以在这里通过上下文属性获取变量值
        QJSValue BOXswitch = engine.evaluate("BOXswitch");
        QJSValue APIswitch = engine.evaluate("APIswitch");
        QJSValue line1 = engine.evaluate("line1");
        QJSValue line2 = engine.evaluate("line2");

        // 检查变量是否存在
        if (BOXswitch.isUndefined() || APIswitch.isUndefined() || line1.isUndefined() || line2.isUndefined()) {
            QMessageBox::critical(this, "Error", "JavaScript 变量提取错误");
            return;
        }

        // 写入文件
        QFile outputFile(real_path + "/cache/textbox_setting.txt");
        if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&outputFile);
            out << BOXswitch.toString() << Qt::endl;
            out << APIswitch.toString() << Qt::endl;
            out << line1.toString() << Qt::endl;
            out << line2.toString() << Qt::endl;
            outputFile.close();
        } else {
            QMessageBox::critical(this, "Error", "无法打开文件 " + real_path + "/textbox_setting.txt 进行写入操作");
        }
    } else {
        QMessageBox::critical(this, "Error", "无法打开文件 " + real_path + "/TextBox.js 进行读取操作");
    }
}

void Editor::handleColorSelected(const QColor &color)
{
    // 在这里处理第一个颜色选择事件，比如更新UI或执行其他操作
    // 更新 text_color 变量
    bgcolor = color.name();
    qDebug() << "Selected Color:" << bgcolor;
    ui->bgcolor->setText(bgcolor);
}
void Editor::handleColorSelected2(const QColor &color)
{
    // 在这里处理第二个颜色选择事件，比如更新UI或执行其他操作
    s_colorAM = color.name();
    qDebug() << "Selected Color:" << bgcolor;
    ui->colorAM->setText(s_colorAM);
}
void Editor::handleColorSelected3(const QColor &color)
{
    // 在这里处理第一个颜色选择事件，比如更新UI或执行其他操作
    // 更新 text_color 变量
    s_colorPM = color.name();
    qDebug() << "Selected Color:" << bgcolor;
    ui->colorPM->setText(s_colorPM);
}
void Editor::on_pushButton_17_clicked()
{
    int currentIndex = ui->stackedWidget->currentIndex();
    int nextPageIndex = (currentIndex + 1) % ui->stackedWidget->count();
    ui->stackedWidget->setCurrentIndex(nextPageIndex);
}

void Editor::on_pushButton_18_clicked()
{
    int currentIndex = ui->stackedWidget->currentIndex();
    int prevPageIndex = (currentIndex - 1 + ui->stackedWidget->count()) % ui->stackedWidget->count();
    ui->stackedWidget->setCurrentIndex(prevPageIndex);
}

void Editor::on_line1_clicked()
{
    line1 *subWindow = new line1(1, this);
    subWindow->show();
}

void Editor::on_line2_text_clicked()
{
    line1 *subWindow = new line1(2, this);
    subWindow->show();
}

void Editor::writeStylesToFile(const QStringList& styles) {
    // 打开文件以保存样式数据
    QFile outputFile(real_path + "/cache/text_setting.txt");
    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&outputFile);

        // 遍历数组，将每个样式写入文件
        for (const QString& style : styles) {
            out << style << Qt::endl;
        }

        outputFile.close();
    }
}

void Editor::readAndSaveStyles() {
    QFile file(real_path + "/files/js/TextSetting.js");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString jsonData = file.readAll(); // 使用 QString 类型
        file.close();

        // 使用 QRegularExpression 替代 QRegExp
        QRegularExpression arrayRegex("\\[.*?\\]");
        QRegularExpressionMatchIterator matches = arrayRegex.globalMatch(jsonData);

        QStringList stylesList;

        // 遍历匹配到的数组
        int lineCount = 0;
        while (matches.hasNext()) {
            QRegularExpressionMatch match = matches.next();
            QString arrayString = match.captured(0);

            // 检查是否包含无用的字符串组，如果不包含则添加到样式列表
            if (!arrayString.contains("xianzaishiWENZI") &&
                !arrayString.contains("line2text") &&
                !arrayString.contains("Now_number") &&
                !arrayString.contains("time") &&
                !arrayString.contains("line3text") &&
                !arrayString.contains("line3num") &&
                !arrayString.contains("gaokao") &&
                !arrayString.contains("Line4text") &&
                !arrayString.contains("Line4num") &&
                !arrayString.contains("Line4text_lite") &&
                !arrayString.contains("Line4num_lite") && lineCount<= 10) {
                stylesList.append(arrayString);
                lineCount++;
            }
        }

        // 写入文件
        writeStylesToFile(stylesList);
    }
}

void Editor::sons(){
    real_path = QDir(QCoreApplication::applicationDirPath()).absolutePath();
    QDir parentDir(real_path);
    parentDir.cdUp();  // 切换到上级目录

    real_path = parentDir.absolutePath();
    // qDebug() << "Parent directory path: " << real_path;

}



void Editor::on_line2_date_clicked()
{
    line1 *subWindow = new line1(3, this);
    subWindow->show();
}


void Editor::on_line2_num_clicked()
{
    line1 *subWindow = new line1(4, this);
    subWindow->show();
}


void Editor::on_line3_text_clicked()
{
    line1 *subWindow = new line1(5, this);
    subWindow->show();
}


void Editor::on_line3_num_clicked()
{
    line1 *subWindow = new line1(6, this);
    subWindow->show();
}


void Editor::on_line3_gaokao_clicked()
{
    line1 *subWindow = new line1(7, this);
    subWindow->show();
}


void Editor::on_line4_text_clicked()
{
    line1 *subWindow = new line1(8, this);
    subWindow->show();
}


void Editor::on_line4_num_clicked()
{
    line1 *subWindow = new line1(9, this);
    subWindow->show();
}


void Editor::on_line4_text_lite_clicked()
{
    line1 *subWindow = new line1(10, this);
    subWindow->show();
}


void Editor::on_line4_num_lite_clicked()
{
    line1 *subWindow = new line1(11, this);
    subWindow->show();
}

void Editor::initializeComboBox() {
    int currentIndex = ui->bg_img->currentIndex();  //读取现在选项
    // int currentdefault =  ui->defaultimg->currentIndex();
    ui->bg_img->clear();//清除所有选项
    // ui->defaultimg->clear();
    // 设置目录路径
    QString directoryPath = real_path + "/BgFile/image";
    // 获取目录下的所有文件
    QDir directory(directoryPath);
    QStringList nameFilters;
    nameFilters << "*.jpg" << "*.png";  // 添加你需要的文件类型
    QFileInfoList fileList = directory.entryInfoList(nameFilters, QDir::Files);
    
    // 遍历文件列表，将文件名添加到 QComboBox
    foreach (const QFileInfo &fileInfo, fileList) {
        QString fileName = fileInfo.fileName();
        ui->bg_img->addItem(fileName);
        // ui->defaultimg->addItem(fileName);
    }


    // 连接信号槽，当选择项发生变化时调用相应的槽函数
    connect(ui->bg_img, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Editor::onComboBoxIndexChanged);
    ui->bg_img->setCurrentIndex(currentIndex);//还原原本选项
    // ui->defaultimg->setCurrentIndex(currentdefault);//还原原本选项
}

// 在槽函数中获取当前选择项的文本并赋值给 imgSRC
void Editor::onComboBoxIndexChanged(int index) {
    imgSRC = ui->bg_img->itemText(index);
}

void Editor::initializeComboBox_vdo() {
    // 设置目录路径
    int currentIndex = ui->bg_vdo->currentIndex();  //读取现在选项

    ui->bg_vdo->clear();//清除所有选项

    QString directoryPath = real_path + "/BgFile/video";

    // 获取目录下的所有文件
    QDir directory(directoryPath);
    QStringList nameFilters;
    nameFilters << "*.webm";  // 添加你需要的文件类型
    QFileInfoList fileList = directory.entryInfoList(nameFilters, QDir::Files);

    // 遍历文件列表，将文件名添加到 QComboBox
    foreach (const QFileInfo &fileInfo, fileList) {
        QString fileName = fileInfo.fileName();
        ui->bg_vdo->addItem(fileName);
    }

    // 连接信号槽，当选择项发生变化时调用相应的槽函数
    connect(ui->bg_vdo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Editor::onComboBoxIndexChanged);
    ui->bg_vdo->setCurrentIndex(currentIndex);//还原原本选项

}

// 在槽函数中获取当前选择项的文本并赋值给 imgSRC
void Editor::onComboBoxIndexChanged_vdo(int index) {
    Q_UNUSED(index);
    vdoSRC = ui->bg_vdo->currentText();
}

void Editor::on_textbox_clicked()
{
    textbox textBox(this);  // 使用正确的构造函数，这里假设有一个适当的构造函数
    textBox.exec();          // 或者使用 show()，具体取决于你的需求
}

void Editor::countdowndayreadandwrite() {
    // 读取 JavaScript 文件内容
    QFile inputFile(real_path + "/files/js/countdown.js");
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&inputFile);
        QString jsCode = in.readAll();
        inputFile.close();

        // 使用 QQmlEngine 解析 JavaScript 代码
        QQmlEngine engine;
        QQmlContext context(engine.rootContext());

        // 设置上下文属性，确保 JavaScript 代码中可以访问到 editor 对象
        context.setContextProperty("editor", this);

        // 使用 QML 引擎执行 JavaScript 代码
        engine.evaluate(jsCode);

        // 可以在这里通过上下文属性获取变量值
        QJSValue DATEswitch_fake = engine.evaluate("setting");
        QJSValue settingDAY_fake = engine.evaluate("settingDAY");
        DATEswitch = DATEswitch_fake.toString();
        settingDAY = settingDAY_fake.toString();
        if (DATEswitch == "0") {
            ui->setday->setCheckState(Qt::Checked);
        }
        QDateTime dateTime = QDateTime::fromString(settingDAY, "yyyy/MM/dd hh:mm:ss");

        if (dateTime.isValid()) {
            // 获取年、月、日、时、分、秒
            year = QString::number(dateTime.date().year());
            month = QString::number(dateTime.date().month());
            day = QString::number(dateTime.date().day());
            hour = QString::number(dateTime.time().hour());
            minute = QString::number(dateTime.time().minute());
            second = QString::number(dateTime.time().second());

            ui->year->setText(year);
            ui->month->setText(month);
            ui->day->setText(day);
            ui->hour->setText(hour);
            ui->minute->setText(minute);
            ui->second->setText(second);

        } else {
            qDebug() << "Invalid DateTime Format";
        }
        // 写入文件
        QFile outputFile(real_path + "/cache/countdownday_setting.txt");
        if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&outputFile);
            out << DATEswitch << Qt::endl;
            out << settingDAY << Qt::endl;
            outputFile.close();
        } else {
            QMessageBox::critical(this, "Error", "无法打开文件 " + real_path + "/textbox_setting.txt 进行写入操作");
        }
    } else {
        QMessageBox::critical(this, "Error", "无法打开文件 " + real_path + "/TextBox.js 进行读取操作");
    }
}

void Editor::on_setday_toggled(bool checked)
{
    if (checked) {
        DATEswitch = "0";

    }else{
        DATEswitch = "1";
    }
}

void Editor::on_textbox_3_clicked()
{
    ui->year->setText("");
    ui->month->setText("");
    ui->day->setText("");
    ui->hour->setText("");
    ui->minute->setText("");
    ui->second->setText("");


}

void Editor::on_textbox_2_clicked()
{
    year = ui->year->text();
    month = ui->month->text().rightJustified(2, '0');  // 使用rightJustified来确保为两位数
    day = ui->day->text().rightJustified(2, '0');
    hour = ui->hour->text().rightJustified(2, '0');
    minute = ui->minute->text().rightJustified(2, '0');
    second = ui->second->text().rightJustified(2, '0');

    QString formattedDate = QString("%1/%2/%3 %4:%5:%6")
                                .arg(year)
                                .arg(month)
                                .arg(day)
                                .arg(hour)
                                .arg(minute)
                                .arg(second);

    QStringList dateList;
    dateList << formattedDate;

    writeSettingsToFile(dateList, DATEswitch, real_path + "/cache/countdownday_setting.txt");
}

void Editor::writeSettingsToFile(QStringList date1,QString DATEswitch,const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        QString date = date1.join("");
        // 依次写入变量的值，每个变量独占一行
        out << DATEswitch << Qt::endl;
        out << date << Qt::endl;
        file.close();
    } else {
        QMessageBox::critical(this, "Error", "无法打开文件 " + filePath + " 进行写入操作");
    }
}

void Editor::copyFile(const QString &sourceFilePath, const QString &destinationFilePath) {
    // 打开源文件
    QFile sourceFile(sourceFilePath);
    if (!sourceFile.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开源文件:" << sourceFilePath;
        return;
    }

    // 打开目标文件
    QFile destinationFile(destinationFilePath);
    if (!destinationFile.open(QIODevice::WriteOnly)) {
        qDebug() << "无法打开目标文件:" << destinationFilePath;
        return;
    }

    // 从源文件读取数据并写入目标文件
    QByteArray data = sourceFile.readAll();
    destinationFile.write(data);

    // 关闭文件
    sourceFile.close();
    destinationFile.close();

    qDebug() << "文件复制成功:" << sourceFilePath << " -> " << destinationFilePath;
}

void Editor::BGreadandwrite() {
    // 读取 JavaScript 文件内容
    QFile inputFile(real_path + "/files/js/background.js");
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&inputFile);
        QString jsCode = in.readAll();
        inputFile.close();

        // 使用 QQmlEngine 解析 JavaScript 代码
        QQmlEngine engine;
        QQmlContext context(engine.rootContext());

        // 设置上下文属性，确保 JavaScript 代码中可以访问到 editor 对象
        context.setContextProperty("editor", this);

        // 使用 QML 引擎执行 JavaScript 代码
        engine.evaluate(jsCode);

        // 可以在这里通过上下文属性获取变量值
        bg_config = engine.evaluate("backgroundCONFIG").toString();
        sound_config = engine.evaluate("soundCONFIG").toString();
        bgcolor = engine.evaluate("bgcolor").toString();
        imageSRC = engine.evaluate("imageSRC").toString();
        videoSRC = engine.evaluate("videoSRC").toString();
        customeAPI = engine.evaluate("customAPI").toString();
        defaultIMG = engine.evaluate("defaultIMG").toString();
        // 写入文件
        QFile outputFile(real_path + "/cache/background_setting.txt");
        if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&outputFile);
            out << bg_config << Qt::endl;
            out << sound_config << Qt::endl;
            out << bgcolor << Qt::endl;
            out << imageSRC << Qt::endl;
            out << videoSRC << Qt::endl;
            out << customeAPI << Qt::endl;
            out << defaultIMG  << Qt::endl;

            outputFile.close();
            ui->bg_type->setCurrentIndex(bg_config.toInt());
            ui->bg_api->setText(customeAPI);
            ui->bgcolor->setText(bgcolor);
            if(sound_config == "0"){
                ui->bg_vdo_mute->setCheckState(Qt::Checked);
            }
            imageSRC = engine.evaluate("imageSRC").toString();
            QString fileName = QFileInfo(imageSRC).fileName();
            ui->bg_img->setCurrentText(fileName);

            videoSRC = engine.evaluate("videoSRC").toString();
            QString fileName_vdo = QFileInfo(videoSRC).fileName();
            ui->bg_vdo->setCurrentText(fileName_vdo);

            defaultimageSRC = engine.evaluate("defaultIMG").toString();
            QString fileName_defaultimage = QFileInfo(defaultIMG).fileName();
            // ui->defaultimg->setCurrentText(fileName_defaultimage);

        } else {
            QMessageBox::critical(this, "Error", "无法打开文件 " + real_path + "/background_setting.txt进行写入操作");
        }
    } else {
        QMessageBox::critical(this, "Error", "无法打开文件 " + real_path + "/background_setting.txt 进行读取操作");
    }
}

void Editor::on_open_file_clicked()
{
    QString folderPath = real_path + "/BgFile";  // 替换为你的文件夹路径
    QDesktopServices::openUrl(QUrl("file:///" + folderPath, QUrl::TolerantMode));
}

void Editor::on_add_file_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::homePath(),tr("所有文件 (*.*);;图片文件 (*.jpg *.png);;视频文件 (*.webm)"));

    if (!filePath.isEmpty()) {
        // 用户选择了文件，可以在这里处理选择的文件路径
        qDebug() << "选择的文件路径:" << filePath;

        // 检测文件类型
        QFileInfo fileInfo(filePath);
        QString suffix = fileInfo.suffix().toLower();

        if (suffix == "jpg" || suffix == "png") {
            qDebug() << "选择的是图片文件";
            copyFile(filePath, real_path + "/BgFile/image/" + fileInfo.fileName());
            QTimer::singleShot(1500, this, [this]() {
                initializeComboBox();
            });

        } else if (suffix == "webm") {
            qDebug() << "选择的是视频文件";
            copyFile(filePath, real_path + "/BgFile/video/" + fileInfo.fileName());
            QTimer::singleShot(1500, this, [this]() {
                initializeComboBox_vdo();
            });
        } else {
            qDebug() << "选择的是其他文件类型，弹出警告";
            // 弹出警告
            QMessageBox msgBox;
            msgBox.setIconPixmap(QPixmap(":/icons/oppp64.png"));
            msgBox.setText("\n文件格式错误！请选择图片文件（.jpg, .png）或视频文件（.webm）");
            msgBox.setWindowTitle("错误");
            msgBox.setWindowIcon(QIcon(":/icons/icon.bmp"));
            msgBox.exec();
            // 设置主窗口为父对象，使消息框以主窗口为中心
            msgBox.setParent(this);
            msgBox.setWindowModality(Qt::WindowModal);


        }
    } else {
        // 用户取消了选择
        qDebug() << "用户取消了文件选择";
    }
}


void Editor::on_chosebgcolor_clicked()
{
    // 获取第一个颜色并转为字符串
    bgcolor = colorPicker->getColor().name();
    // 更新第一个颜色选择按钮的当前颜色为 text_color
    colorPicker->setCurrentColor(QColor(bgcolor));
}



void Editor::on_bg_vdo_mute_toggled(bool checked)
{
    if (checked) {
        sound_config = "0";

    }else{
        sound_config = "1";
    }
}


void Editor::writeSettingsToBGcache(QString bg_config,QString sound_config,QString bgcolor,QString imageSRC,QString videoSRC,QString customeAPI,QString defaultIMG,const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        // 依次写入变量的值，每个变量独占一行
        out << bg_config << Qt::endl;
        out << sound_config << Qt::endl;
        out << bgcolor << Qt::endl;
        out << imageSRC << Qt::endl;
        out << videoSRC << Qt::endl;
        out << customeAPI << Qt::endl;
        out << defaultIMG << Qt::endl;

        file.close();
    } else {
        QMessageBox::critical(this, "Error", "无法打开文件 " + filePath + " 进行写入操作");
    }
}


void Editor::STARreadandwrite() {
    // 读取 JavaScript 文件内容
    QFile inputFile(real_path + "/files/js/star.js");
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&inputFile);
        QString jsCode = in.readAll();
        inputFile.close();

        // 使用 QQmlEngine 解析 JavaScript 代码
        QQmlEngine engine;
        QQmlContext context(engine.rootContext());

        // 设置上下文属性，确保 JavaScript 代码中可以访问到 editor 对象
        context.setContextProperty("editor", this);

        // 使用 QML 引擎执行 JavaScript 代码
        engine.evaluate(jsCode);

        // 可以在这里通过上下文属性获取变量值
        starswitch = engine.evaluate("starswitch").toString();
        starcolor = engine.evaluate("color").toString();
        starmax = engine.evaluate("starmax").toString();
        starmin = engine.evaluate("starmin").toString();

        // 写入文件
        QFile outputFile(real_path + "/cache/star_setting.txt");
        if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&outputFile);
            out << starswitch << Qt::endl;
            out << starcolor << Qt::endl;
            out << starmax << Qt::endl;
            out << starmin << Qt::endl;

            outputFile.close();
        } else {
            QMessageBox::critical(this, "Error", "无法打开文件" + real_path + "/cache/star_setting.txt");
        }
    } else {
        QMessageBox::critical(this, "Error", "无法打开文件" + real_path + "/files/js/star.js");
    }
}

void Editor::on_star_Setting_clicked()
{
    star starDialog(this);
    starDialog.exec();
}


void Editor::readScheduleFromFile(const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // 读取 JavaScript 文件内容
        QTextStream in(&file);
        QString jsCode = in.readAll();  // 保存 JavaScript 代码到 jsCode 变量
        file.close();

        // 使用 QRegularExpression 替代 QRegExp
        QRegularExpression arrayRegex("\\[.*?\\]");
        QRegularExpressionMatchIterator matches = arrayRegex.globalMatch(jsCode);

        QStringList scheduleList;

        // 遍历匹配到的数组，读取前十四个数组
        int lineCount = 0;
        while (matches.hasNext() && lineCount < 14) {
            QRegularExpressionMatch match = matches.next();
            QString arrayString = match.captured(0);

            // 添加到课程表字符串列表
            scheduleList.append(arrayString);
            qDebug() << "Matched string: " << arrayString;
            lineCount++;
        }


        // ///////// 使用 QQmlEngine 解析 JavaScript 代码
        QQmlEngine engine;
        QQmlContext context(engine.rootContext());

        // 使用 QML 引擎执行 JavaScript 代码
        engine.evaluate(jsCode);

            // 可以在这里通过上下文属性获取变量值
            s_positionMODE = engine.evaluate("positionMODE").toString();
            s_positionX = engine.evaluate("positionX").toString();
            s_positionY = engine.evaluate("positionY").toString();
            s_shadowSIZE = engine.evaluate("shadowSIZE").toString();
            s_opacity = engine.evaluate("opacity").toString();
            s_gapSIZE = engine.evaluate("gapSIZE").toString();
            s_order = engine.evaluate("order").toString();
            s_hide = engine.evaluate("hide").toString();
            s_colorAM = engine.evaluate("colorAM").toString();
            s_colorPM = engine.evaluate("colorPM").toString();
            #ifdef s_colorAM
            #else
            s_colorAM = "";
            #endif
            #ifdef s_colorPM
            #else
            s_colorPM = "";
            #endif

            if(s_positionMODE=="2"){
                ui->positionMODE->setCurrentText("左上角");
            }else{
                ui->positionMODE->setCurrentText("中心");
            }
            ui->positionX->setText(s_positionX);
            ui->positionY->setText(s_positionY);
            ui->opacity->setText(QString::number(s_opacity.toInt() * 100));
            ui->shadowSIZE->setText(s_shadowSIZE);
            ui->gapSIZE->setText(s_gapSIZE);
            ui->colorAM->setText(s_colorAM);
            ui->colorPM->setText(s_colorPM);
            if(s_hide == "0"){
                ui->hide_switch->setCheckState(Qt::Checked);
            }
            // 调试
            // qDebug() << "s_positionMODE: " << s_positionMODE;
            // qDebug() << "s_positionX: " << s_positionX;
            // qDebug() << "s_positionY: " << s_positionY;
            // qDebug() << "s_opacity: " << s_opacity;
            // qDebug() << "s_shadowSIZE: " << s_shadowSIZE;
            // qDebug() << "s_gapSIZE: " << s_gapSIZE;
            // qDebug() << "s_hide: " << s_hide;
        // 写入文件
        writeScheduleToFile(scheduleList);
        assignScheduleToLineEdit();

    } else {
        qDebug() << "Failed to open file: " << filePath;
    }
}

void Editor::writeScheduleToFile(const QStringList &scheduleList) {
    // 打开文件以保存课程表数据  tocache
    QFile outputFile(real_path + "/cache/schedule.txt");
    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&outputFile);

        // 遍历数组，将每个数组写入文件
        for (const QString &schedule : scheduleList) {
            out << schedule << Qt::endl;
        }

        // 将其他变量的值写入文件
        out << s_positionMODE << Qt::endl;
        out << s_positionX << Qt::endl;
        out << s_positionY << Qt::endl;
        out << s_shadowSIZE << Qt::endl;
        out << s_opacity << Qt::endl;
        out << s_gapSIZE << Qt::endl;
        out << s_order << Qt::endl;
        out << s_hide << Qt::endl;
        out << s_colorAM << Qt::endl;
        out << s_colorPM << Qt::endl;
        outputFile.close();
    } else {
        qDebug() << "Failed to open file for writing: " + real_path + "/schedule.txt";
    }
}
void Editor::readcourseFromCache(int lineNumber)
{
    QFile file(real_path + "/cache/schedule.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    int currentLine = 1;

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (currentLine == lineNumber) {
            // 处理可能存在的额外引号和斜杠
            line.remove(QRegularExpression("[\"\\[\\]]"));

            // 解析行为样式元素
            QStringList stylesList = line.split(", ", Qt::SkipEmptyParts);
            if (stylesList.size() == 6) { // 确保匹配预期元素数量
                // 将样式元素赋值给相应的变量
                mediators[1] = stylesList[0];
                mediators[2] = stylesList[1];
                mediators[3] = stylesList[2];
                mediators[4] = stylesList[3];
                mediators[5] = stylesList[4];
                mediators[6] = stylesList[5];

                file.close();
                return;
            } else {
                qDebug() << "数组中元素数量无效";
                file.close();
                return;
            }
        }
        currentLine++;
    }

    qDebug() << "指定行" << lineNumber << "未找到";
    file.close();
}

QStringList lineEditNames = {"mon_am_1", "mon_am_2", "mon_am_3", "mon_am_4", "mon_am_5", "mon_am_6",
                             "mon_pm_1", "mon_pm_2", "mon_pm_3", "mon_pm_4", "mon_pm_5", "mon_pm_6",
                             "tue_am_1", "tue_am_2", "tue_am_3", "tue_am_4", "tue_am_5", "tue_am_6",
                             "tue_pm_1", "tue_pm_2", "tue_pm_3", "tue_pm_4", "tue_pm_5", "tue_pm_6",
                             "wed_am_1", "wed_am_2", "wed_am_3", "wed_am_4", "wed_am_5", "wed_am_6",
                             "wed_pm_1", "wed_pm_2", "wed_pm_3", "wed_pm_4", "wed_pm_5", "wed_pm_6",
                             "thu_am_1", "thu_am_2", "thu_am_3", "thu_am_4", "thu_am_5", "thu_am_6",
                             "thu_pm_1", "thu_pm_2", "thu_pm_3", "thu_pm_4", "thu_pm_5", "thu_pm_6",
                             "fri_am_1", "fri_am_2", "fri_am_3", "fri_am_4", "fri_am_5", "fri_am_6",
                             "fri_pm_1", "fri_pm_2", "fri_pm_3", "fri_pm_4", "fri_pm_5", "fri_pm_6",
                             "sat_am_1", "sat_am_2", "sat_am_3", "sat_am_4", "sat_am_5", "sat_am_6",
                             "sat_pm_1", "sat_pm_2", "sat_pm_3", "sat_pm_4", "sat_pm_5", "sat_pm_6",
                             "sun_am_1", "sun_am_2", "sun_am_3", "sun_am_4", "sun_am_5", "sun_am_6",
                             "sun_pm_1", "sun_pm_2", "sun_pm_3", "sun_pm_4", "sun_pm_5", "sun_pm_6"};

void Editor::assignScheduleToLineEdit() {
        int count = 1 ;
        int turn = 1;
        int num = 0 ;


            while(turn<=14){
            readcourseFromCache(turn);
            QString lineEditName = lineEditNames[num];
            QLineEdit *lineEdit = findChild<QLineEdit*>(lineEditName);
            if (mediators[count].trimmed().isEmpty()) {
                // 如果 mediator 的内容为空或者只包含空格，将其设为 ""
                lineEdit->setText("");
            } else {
                // 否则，将 QLineEdit 的文本内容设为相应的样式元素
                lineEdit->setText(mediators[count]);
            }

                count++;
                num++;
                if(count==7){
                    turn++;
                    count=1;

                }
            }
}

void Editor::assignLineEditToSchedule() {
    int count = 1;
    int turn = 1;
    int num = 0;

    while (turn <= 14) {
        QString lineEditName = lineEditNames[num];
        QLineEdit *lineEdit = findChild<QLineEdit*>(lineEditName);
        QString middle;
        // 逆向赋值
        if(lineEdit->text().trimmed().isEmpty()){
            middle = "\" \"";
        }else{
            middle = "\"" +lineEdit->text() + "\"";

        }

        course[turn] <<middle;
        count++;
        num++;
        if (count == 7) {
            turn++;
            count = 1;
        }
    }
    if (ui->positionMODE->currentText() == "左上角") {
        s_positionMODE = "2";
    } else {
        s_positionMODE = "1";
    }
    s_positionX = ui->positionX->text();
    s_positionY = ui->positionY->text();
    s_opacity = QString::number( ((ui->opacity->text()).toInt())/100.00 );
    s_shadowSIZE = ui->shadowSIZE->text();
    s_gapSIZE = ui->gapSIZE->text();
    s_hide = (ui->hide_switch->checkState() == Qt::Checked) ? "0" : "1" ;
    s_colorAM =ui->colorAM->text();
    s_colorPM = ui->colorPM->text();
    writeToSCHEDULEcache(course,real_path + "/cache/schedule.txt");

}

void Editor::writeToSCHEDULEcache(QStringList courses[15], const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        // 依次写入每个QStringList的内容
        for (int i = 1; i <= 14; ++i) {
            out << "[" + courses[i].join(", ")  + "]"<< Qt::endl;
        }
        int i = 0;
        while(i<=14){
            course[i].clear();
            i++;
        }
        int j = 0;
        while(j<=14){
            courses[j].clear();
            j++;
        }
        out << s_positionMODE << Qt::endl;
        out << s_positionX << Qt::endl;
        out << s_positionY << Qt::endl;
        out << s_shadowSIZE << Qt::endl;
        out << s_opacity << Qt::endl;
        out << s_gapSIZE << Qt::endl;
        out << s_order << Qt::endl;
        out << s_hide << Qt::endl;
        out << s_colorAM << Qt::endl;
        out << s_colorPM << Qt::endl;
        file.close();
    } else {
        QMessageBox::critical(this, "Error", "无法打开文件 " + filePath + " 进行写入操作");
    }
}

void Editor::on_save_schedule_clicked() //清除按钮
{
    int count = 1 ;
    int turn = 1;
    int num = 0 ;

    QMessageBox msgBox;
    msgBox.setIconPixmap(QPixmap(":/icons/oppp64.png"));
    msgBox.setText("\n你真的要清空课表吗？");
    msgBox.setWindowTitle("哎呀！");
    msgBox.setWindowIcon(QIcon(":/icons/icon.bmp"));

    // 添加确认按钮和取消按钮
    QPushButton *confirmButton = msgBox.addButton(tr("确认"), QMessageBox::ActionRole);
    QPushButton *cancelButton = msgBox.addButton(tr("取消"), QMessageBox::RejectRole);

    // 将取消按钮设置为默认按钮
    cancelButton->setDefault(true);

    // 设置主窗口为父对象，使消息框以主窗口为中心
    msgBox.setParent(this);
    msgBox.setWindowModality(Qt::WindowModal);


    // 显示消息框
    msgBox.exec();

    // 判断用户点击的是确认按钮还是取消按钮
    if (msgBox.clickedButton() == confirmButton) {
        while(turn<=14){
            QString lineEditName = lineEditNames[num];
            QLineEdit *lineEdit = findChild<QLineEdit*>(lineEditName);
            lineEdit->setText("");

            count++;
            num++;
            if(count==7){
                turn++;
                count=1;

            }
        }
    }
}

void Editor::on_check_update_released()
{
    QSslSocket::supportsSsl(); // 用于初始化 SSL 支持
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl("http://checkupdate.cenmin.top/index.json")));

    // 处理响应
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            // 读取响应数据
            QByteArray responseData = reply->readAll();
            responseData.replace("\\n", "\n"); // 替换转义字符

            // 解析JSON
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObject = jsonResponse.object();

            qDebug() << "Network request failed. Error code: " << reply->error();
            qDebug() << "Raw JSON response:" << responseData;

            // 检查 JSON 对象是否包含 "status" 字段
            if (!jsonObject.contains("status")) {
                qDebug() << "JSON parsing error: 'status' field not found.";
            }

            // 检查 "status" 字段
            QJsonValue statusValue = jsonObject.value("status");
            QJsonValue statusValue2 = jsonObject.value("version");
            QJsonValue statusValue3 = jsonObject.value("url");
            QJsonValue statusValue4 = jsonObject.value("changelog");

            if (statusValue.isString()) {
                bool conversionOK;
                QString versionString = statusValue2.toString();
                QString status = statusValue.toString();
                float version = versionString.toFloat(&conversionOK);
                QString url = statusValue3.toString();
                QString changelog = statusValue4.toString();
                qDebug() << status <<version << url;
                // 根据不同的状态执行不同的操作
                if (status == "ready" && version>current_version) {
                    // 执行A动作
                    qDebug() << "Status is OK. Perform action A.";
                    // 弹出警告
                    QMessageBox msgBox;
                    msgBox.setIconPixmap(QPixmap(":/icons/eat64.png"));
                    msgBox.setText("有新更新，点击确认跳转下载\n" + changelog);
                    msgBox.setWindowTitle("有新更新");
                    msgBox.setWindowIcon(QIcon(":/icons/icon.bmp"));

                    // 添加确认按钮和取消按钮
                    QPushButton *confirmButton = msgBox.addButton(tr("确认"), QMessageBox::ActionRole);
                    msgBox.addButton(tr("取消"), QMessageBox::RejectRole);
                    // 设置主窗口为父对象，使消息框以主窗口为中心
                    msgBox.setParent(this);
                    msgBox.setWindowModality(Qt::WindowModal);


                    // 显示消息框
                    msgBox.exec();

                    // 判断用户点击的是确认按钮还是取消按钮
                    if (msgBox.clickedButton() == confirmButton) {
                        // 执行跳转到网页的操作
                        QDesktopServices::openUrl(QUrl(url));
                    }
                }  else {
                    QMessageBox msgBox;
                    msgBox.setIconPixmap(QPixmap(":/icons/oppp64.png"));
                    msgBox.setText("\n暂时还没有更新");
                    msgBox.setWindowTitle("哎呀");
                    msgBox.setWindowIcon(QIcon(":/icons/icon.bmp"));

                    // 显示消息框
                    msgBox.exec();                }
            }
        } else {
            // 处理网络错误
            QMessageBox msgBox;
            msgBox.setIconPixmap(QPixmap(":/icons/oppp64.png"));
            msgBox.setText("\n网络错误: " + reply->errorString());
            msgBox.setWindowTitle("错误");
            msgBox.setWindowIcon(QIcon(":/icons/icon.bmp"));

            // 显示消息框
            msgBox.exec();
            qDebug() << "Network error: " << reply->errorString();
        }

        // 清理资源
        reply->deleteLater();
        manager->deleteLater();
    });
}

void Editor::auto_check_update()
{
    // 打开文件
    QString filePath = real_path + "/bin/check_update.txt";
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString firstLine = in.readLine();

        // 判断第一行是否为 "0"，如果是则将 a 赋值给 b
        if (firstLine.trimmed() == "0") {
            auto_check_update_status = 0;
            qDebug() <<"close";
            ui->checkBox->setCheckState(Qt::Unchecked);
        } else {
            auto_check_update_status = 1;
            ui->checkBox->setCheckState(Qt::Checked);
            qDebug() <<"open";

        }

        // 关闭文件
        file.close();
    }
    // 创建网络访问管理器和请求
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl("http://checkupdate.cenmin.top/index.json")));

    // 连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=]() {


        // 处理网络响应
        if (reply->error() == QNetworkReply::NoError) {
            // 读取响应数据
            QByteArray responseData = reply->readAll();
            responseData.replace("\\n", "\n"); // 替换转义字符

            // 解析JSON
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObject = jsonResponse.object();

            // 检查 "status" 字段
            QJsonValue statusValue = jsonObject.value("status");
            QJsonValue statusValue2 = jsonObject.value("version");
            QJsonValue statusValue3 = jsonObject.value("url");
            QJsonValue statusValue4 = jsonObject.value("changelog");

            if (statusValue.isString()) {
                bool conversionOK;
                QString versionString = statusValue2.toString();
                QString status = statusValue.toString();
                float version = versionString.toFloat(&conversionOK);
                QString url = statusValue3.toString();
                QString changelog = statusValue4.toString();

                qDebug() << status << version << url;
                if (status == "ready" && version > current_version && auto_check_update_status == 1) {

                // 根据不同的状态执行不同的操作
                // 执行A动作
                qDebug() << "Status is OK. Perform action A.";
                // 弹出警告
                QMessageBox msgBox;
                msgBox.setIconPixmap(QPixmap(":/icons/eat64.png"));
                msgBox.setText("有新更新，点击确认跳转下载\n" + changelog);
                msgBox.setWindowTitle("有新更新");
                msgBox.setWindowIcon(QIcon(":/icons/icon.bmp"));

                // 添加确认按钮和取消按钮
                QPushButton *confirmButton = msgBox.addButton(tr("确认"), QMessageBox::ActionRole);
                msgBox.addButton(tr("取消"), QMessageBox::RejectRole);
                // 设置主窗口为父对象，使消息框以主窗口为中心
                msgBox.setParent(this);
                msgBox.setWindowModality(Qt::WindowModal);


                // 显示消息框
                msgBox.exec();

                // 判断用户点击的是确认按钮还是取消按钮
                if (msgBox.clickedButton() == confirmButton) {
                    // 执行跳转到网页的操作
                    QDesktopServices::openUrl(QUrl(url));
                }
            }
            }
        }

        // 清理资源
        reply->deleteLater();
        manager->deleteLater();
    });
}

void Editor::on_checkBox_toggled(bool checked)
{
    if(checked){
        auto_check_update_status=1;
        qDebug() <<auto_check_update_status;
        QFile outputFile(real_path + "/bin/check_update.txt");
        if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&outputFile);
            out << auto_check_update_status << Qt::endl;
            outputFile.close();
        }
    }else{
        auto_check_update_status=0;
        qDebug() <<auto_check_update_status;
        QFile outputFile(real_path + "/bin/check_update.txt");
        if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&outputFile);
            out << auto_check_update_status << Qt::endl;
            outputFile.close();
        }

    }
}

void Editor::TIMERreadandwrite() {
    // 读取 JavaScript 文件内容
    QFile inputFile(real_path + "/files/js/Timer.js");
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&inputFile);
        QString jsCode = in.readAll();
        inputFile.close();

        // 使用 QQmlEngine 解析 JavaScript 代码
        QQmlEngine engine;
        QQmlContext context(engine.rootContext());

        // 设置上下文属性，确保 JavaScript 代码中可以访问到 editor 对象
        context.setContextProperty("editor", this);

        // 使用 QML 引擎执行 JavaScript 代码
        engine.evaluate(jsCode);

        // 可以在这里通过上下文属性获取变量值
        timer1_switch = engine.evaluate("switch1").toInt();
        aimhours = engine.evaluate("aimhours").toInt();
        aimminutes = engine.evaluate("aimminutes").toInt();
        aimseconds = engine.evaluate("aimseconds").toInt();

        timer2_switch = engine.evaluate("switch2").toInt();
        aimhours2 = engine.evaluate("aimhours2").toInt();
        aimminutes2 = engine.evaluate("aimminutes2").toInt();
        aimseconds2 = engine.evaluate("aimseconds2").toInt();


        if(timer1_switch==1){
            ui->Timer1_switch->setCheckState(Qt::Checked);
        }else{
            ui->Timer1_switch->setCheckState(Qt::Unchecked);
        }
        if(timer2_switch==1){
            ui->Timer2_switch->setCheckState(Qt::Checked);
        }else{
            ui->Timer2_switch->setCheckState(Qt::Unchecked);
        }
        ui->Timer1_1->setText(QString::number(aimhours));
        ui->Timer1_2->setText(QString::number(aimminutes));
        ui->Timer1_3->setText(QString::number(aimseconds));
        ui->Timer2_1->setText(QString::number(aimhours2));
        ui->Timer2_2->setText(QString::number(aimminutes2));
        ui->Timer2_3->setText(QString::number(aimseconds2));

        // 写入文件
        QFile outputFile(real_path + "/cache/Timer_setting.txt");
        if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&outputFile);
            out << timer1_switch << Qt::endl;
            out << timer2_switch << Qt::endl;

            out << aimhours << Qt::endl;
            out << aimminutes << Qt::endl;
            out << aimseconds << Qt::endl;
            out << aimhours2 << Qt::endl;
            out << aimminutes2 << Qt::endl;
            out << aimseconds2 << Qt::endl;
            outputFile.close();
        } else {
            QMessageBox::critical(this, "Error", "无法打开文件" + real_path + "/cache/Timer_setting.txt");
        }
    } else {
        QMessageBox::critical(this, "Error", "无法打开文件" + real_path + "/cache/Timer_setting.txt");
    }
}

void Editor::WaitLoadreadandwrite(){
    // 读取 JavaScript 文件内容
    QFile inputFile(real_path + "/files/js/WaitLoad.js");
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&inputFile);
        QString jsCode = in.readAll();
        inputFile.close();

        // 使用 QQmlEngine 解析 JavaScript 代码
        QQmlEngine engine;
        QQmlContext context(engine.rootContext());

        // 设置上下文属性，确保 JavaScript 代码中可以访问到 editor 对象
        context.setContextProperty("editor", this);

        // 使用 QML 引擎执行 JavaScript 代码
        engine.evaluate(jsCode);

        // 可以在这里通过上下文属性获取变量值
        wait_switch = engine.evaluate("wait").toInt();
        fadetime = engine.evaluate("time").toInt();
        outtime = engine.evaluate("timeout").toInt();

        if(wait_switch==1){
            ui->Waitload_switch->setCheckState(Qt::Checked);
        }else{
            ui->Waitload_switch->setCheckState(Qt::Unchecked);
            // qDebug() << "wait switch:" << QString::number(wait_switch);
        }
        ui->Waitload_fadetime->setText(QString::number(fadetime));
        ui->Waitload_outtime->setText(QString::number(outtime));
        // 写入文件

        QFile outputFile(real_path + "/cache/WaitLoad_setting.txt");
        if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&outputFile);
            out << wait_switch << Qt::endl;
            out << fadetime << Qt::endl;
            out << outtime << Qt::endl;
            outputFile.close();
        } else {
            QMessageBox::critical(this, "Error", "无法打开文件" + real_path + "/cache/WaitLoad_setting.txt");
        }
    } else {
        QMessageBox::critical(this, "Error", "无法打开文件" + real_path + "/files/js/WaitLoad.js");
    }
}

void Editor::on_Timer1_switch_toggled(bool checked)
{
    if(checked){
        timer1_switch=1;

    }else{
        timer1_switch=0;
    }
}

void Editor::on_Timer2_switch_toggled(bool checked)
{
    if(checked){
        timer2_switch=1;

    }else{
        timer2_switch=0;
    }
}

void Editor::save_other()
{

    aimhours = (ui->Timer1_1->text()).toInt();
    aimminutes = (ui->Timer1_2->text()).toInt();
    aimseconds = (ui->Timer1_3->text()).toInt();
    aimhours2 = (ui->Timer2_1->text()).toInt();
    aimminutes2 = (ui->Timer2_2->text()).toInt();
    aimseconds2 = (ui->Timer2_3->text()).toInt();

    // 写入文件
    QFile outputFile(real_path + "/cache/Timer_setting.txt");
    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&outputFile);
        out << timer1_switch << Qt::endl;
        out << timer2_switch << Qt::endl;
        out << aimhours << Qt::endl;
        out << aimminutes << Qt::endl;
        out << aimseconds << Qt::endl;
        out << aimhours2 << Qt::endl;
        out << aimminutes2 << Qt::endl;
        out << aimseconds2 << Qt::endl;
        outputFile.close();
    } else {
        QMessageBox::critical(this, "Error", "无法打开文件" + real_path + "/cache/Timer_setting.txt");
    }
    fadetime= (ui->Waitload_fadetime->text()).toInt();
    outtime = (ui->Waitload_outtime->text()).toInt();

    // 写入文件

    QFile outputFile2(real_path + "/cache/WaitLoad_setting.txt");
    if (outputFile2.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&outputFile2);
        out << wait_switch << Qt::endl;
        out << fadetime << Qt::endl;
        out << outtime << Qt::endl;
        outputFile2.close();
    } else {
        QMessageBox::critical(this, "Error", "无法打开文件" + real_path + "/cache/WaitLoad_setting.txt");
    }

}

int i;
void Editor::save_bg()
{
    if(ui->bg_type->currentText() == "纯色"){
        bg_config = "0";
    }else if(ui->bg_type->currentText() == "图片"){
        bg_config = "1";
    }else if(ui->bg_type->currentText() == "视频"){
        bg_config = "2";
    }else if(ui->bg_type->currentText() == "每日Bing"){
        bg_config = "3";
    }else if(ui->bg_type->currentText() == "Windows聚焦"){
        bg_config = "4";
    }else if(ui->bg_type->currentText() == "ACG壁纸"){
        bg_config = "5";
    }else if(ui->bg_type->currentText() == "自定义API"){
        bg_config = "6";
    }
    customeAPI = ui->bg_api->text();
    bgcolor = ui->bgcolor->text();
    QString imgname = ui->bg_img->currentText();
    QString vdoname = ui->bg_vdo->currentText();
    imageSRC = "./BgFile/image/" + imgname;
    videoSRC = "./BgFile/video/" + vdoname;
    writeSettingsToBGcache(bg_config,sound_config,bgcolor,imageSRC,videoSRC,customeAPI,defaultIMG,real_path + "/cache/background_setting.txt");
    i++;
    qDebug() << "count:" << i;
    qDebug() << "bg_config: " << bg_config;
    qDebug() << "sound_config: " << sound_config;
    qDebug() << "bgcolor: " << bgcolor;
    qDebug() << "imageSRC: " << imageSRC;
    qDebug() << "videoSRC: " << videoSRC;
    qDebug() << "customeAPI: " << customeAPI;
    qDebug() << "defaultIMG: " << defaultIMG;
}

void Editor::on_Waitload_switch_toggled(bool checked)
{
    if(checked){
        wait_switch=1;
        // qDebug() << "wait switch:" << QString::number(wait_switch);

    }else{
        wait_switch=0;
        // qDebug() << "wait switch:" << QString::number(wait_switch);
    }
}

void Editor::updateTimerJS(const QString& settingFilePath, const QString& jsFilePath) {
    // 读取 Timer_setting.txt 文件内容
    QFile settingFile(settingFilePath);
    if (!settingFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "打开 Timer_setting.txt 文件时出错。";
        return;
    }

    QTextStream settingStream(&settingFile);

    // 逐行读取数据
    QString switch1 = settingStream.readLine();
    QString switch2 = settingStream.readLine();
    QString aimhours = settingStream.readLine();
    QString aimminutes = settingStream.readLine();
    QString aimseconds = settingStream.readLine();
    QString aimhours2 = settingStream.readLine();
    QString aimminutes2 = settingStream.readLine();
    QString aimseconds2 = settingStream.readLine();

    settingFile.close();

    // 读取 Timer.js 文件内容
    QFile jsFile(jsFilePath);
    if (!jsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "打开 Timer.js 文件时出错。";
        return;
    }

    QTextStream jsStream(&jsFile);
    QString jsContent = jsStream.readAll();
    jsFile.close();

    // 静态 QRegularExpression 对象
    static const QRegularExpression switch1Pattern("var switch1\\s*=\\s*\\d+;");
    static const QRegularExpression switch2Pattern("var switch2\\s*=\\s*\\d+;");
    static const QRegularExpression aimhoursPattern("var aimhours\\s*=\\s*\\d+;");
    static const QRegularExpression aimminutesPattern("var aimminutes\\s*=\\s*\\d+;");
    static const QRegularExpression aimsecondsPattern("var aimseconds\\s*=\\s*\\d+;");
    static const QRegularExpression aimhours2Pattern("var aimhours2\\s*=\\s*\\d+;");
    static const QRegularExpression aimminutes2Pattern("var aimminutes2\\s*=\\s*\\d+;");
    static const QRegularExpression aimseconds2Pattern("var aimseconds2\\s*=\\s*\\d+;");

    // 替换变量值
    jsContent.replace(switch1Pattern, "var switch1 = " + switch1 + ";");
    jsContent.replace(switch2Pattern, "var switch2 = " + switch2 + ";");
    jsContent.replace(aimhoursPattern, "var aimhours = " + aimhours + ";");
    jsContent.replace(aimminutesPattern, "var aimminutes = " + aimminutes + ";");
    jsContent.replace(aimsecondsPattern, "var aimseconds = " + aimseconds + ";");
    jsContent.replace(aimhours2Pattern, "var aimhours2 = " + aimhours2 + ";");
    jsContent.replace(aimminutes2Pattern, "var aimminutes2 = " + aimminutes2 + ";");
    jsContent.replace(aimseconds2Pattern, "var aimseconds2 = " + aimseconds2 + ";");

    // 写回文件
    QFile outputJsFile(jsFilePath);
    if (!outputJsFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "打开 Timer.js 文件进行写操作时出错。";
        return;
    }

    QTextStream outputJsStream(&outputJsFile);
    QTextCodec *utf8Codec = QTextCodec::codecForName("UTF-8");
    outputJsStream.setCodec(utf8Codec);
    outputJsStream << jsContent;
    outputJsFile.close();
}

void Editor::updateScheduleJS(const QString& scheduleFilePath, const QString& jsFilePath) {
    // Open schedule.txt for reading
    QFile txtFile(scheduleFilePath);
    if (!txtFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open txt file";
        return;
    }

    QTextStream txtStream(&txtFile);
    QStringList lines;
    while (!txtStream.atEnd()) {
        lines.append(txtStream.readLine());
    }
    txtFile.close();

    // 读取其他JavaScript代码
    QFile otherJsFile(real_path + "/bin/ScheduleOtherJSCode.js");
    if (!otherJsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open other JavaScript file";
        return;
    }

    QTextStream otherJsStream(&otherJsFile);
    QString otherJsCode = otherJsStream.readAll();
    otherJsFile.close();

    // 构建JavaScript代码
    QString jsCode = QString(R"(var courseSchedule = {
    Mon: {morning: %1, afternoon: %2},
    Tue: {morning: %3, afternoon: %4},
    Wed: {morning: %5, afternoon: %6},
    Thu: {morning: %7, afternoon: %8},
    Fri: {morning: %9, afternoon: %10},
    Sat: {morning: %11, afternoon: %12},
    Sun: {morning: %13, afternoon: %14},
};
var positionMODE = "%15";
var positionX = "%16";
var positionY = "%17";
var shadowSIZE = "%18";
var opacity = "%19";
var gapSIZE = "%20";
var order = "%21";
var hide = "%22";
var colorAM = "%23";
var colorPM = "%24";
var AM = "";
var PM = "";
////////////////////////////你可以更改内容，但请不要改变以上格式,如需更改以下内容请前往/bin/ScheduleOtherJSCode.js//////////////////////////
%25
)").arg(lines[0], lines[1], lines[2], lines[3], lines[4], lines[5], lines[6], lines[7], lines[8], lines[9], lines[10], lines[11], lines[12], lines[13], lines[14], lines[15], lines[16], lines[17], lines[18], lines[19], lines[20], lines[21], lines[22],lines[23],otherJsCode);

    // 写入JavaScript文件
    QFile jsFile(jsFilePath);
    if (!jsFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open JavaScript file";
        return;
    }

    QTextStream jsStream(&jsFile);
    QTextCodec *utf8Codec = QTextCodec::codecForName("UTF-8");
    jsStream.setCodec(utf8Codec);
    jsStream << jsCode;
    jsFile.close();

}

void Editor::writeRemainingVariables(const QString& scheduleFilePath) {
    // Open schedule.txt for appending
    QFile scheduleFile(scheduleFilePath);
    if (!scheduleFile.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Error opening schedule.txt for appending.";
    }
    QTextStream scheduleStream(&scheduleFile);
    scheduleFile.close();

}

void Editor::updateBackgroundJS(const QString& configFilePath, const QString& jsFilePath) {
    // 读取 config 文件内容
    QFile configFile(configFilePath);
    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "打开" << configFilePath << "文件时出错。";
        return;
    }

    QTextStream configStream(&configFile);

    // 逐行读取数据
    QString backgroundCONFIG = configStream.readLine();
    QString soundCONFIG = configStream.readLine();
    QString bgcolor = configStream.readLine();
    QString imageSRC = configStream.readLine();
    QString videoSRC = configStream.readLine();
    QString customAPI = configStream.readLine();
    QString defaultIMG = configStream.readLine();
    configFile.close();

    // 读取 Timer.js 文件内容
    QFile jsFile(jsFilePath);
    if (!jsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "打开" << jsFilePath << "文件时出错。";
        return;
    }

    QTextStream jsStream(&jsFile);
    QString jsContent = jsStream.readAll();
    jsFile.close();

    // 静态 QRegularExpression 对象
    static const QRegularExpression backgroundCONFIGPattern("var backgroundCONFIG\\s*=\\s*\\d+;");
    static const QRegularExpression soundCONFIGPattern("var soundCONFIG\\s*=\\s*\\d+;");
    static const QRegularExpression bgcolorPattern("var bgcolor\\s*=\\s*\".*\";");
    static const QRegularExpression imageSRCPattern("var imageSRC\\s*=\\s*\".*\";");
    static const QRegularExpression videoSRCPattern("var videoSRC\\s*=\\s*\".*\";");
    static const QRegularExpression customAPIPattern("var customAPI\\s*=\\s*\".*\";");
    static const QRegularExpression defaultIMGPattern("var defaultIMG\\s*=\\s*\".*\";");

    // 替换变量值
    jsContent.replace(backgroundCONFIGPattern, "var backgroundCONFIG = " + backgroundCONFIG + ";");
    jsContent.replace(soundCONFIGPattern, "var soundCONFIG = " + soundCONFIG + ";");
    jsContent.replace(bgcolorPattern, "var bgcolor = \"" + bgcolor + "\";");
    jsContent.replace(imageSRCPattern, "var imageSRC = \"" + imageSRC + "\";");
    jsContent.replace(videoSRCPattern, "var videoSRC = \"" + videoSRC + "\";");
    jsContent.replace(customAPIPattern, "var customAPI = \"" + customAPI + "\";");
    jsContent.replace(defaultIMGPattern, "var defaultIMG = \"" + defaultIMG + "\";");

    // 写回文件
    QFile outputJsFile(jsFilePath);
    if (!outputJsFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "打开" << jsFilePath << "文件进行写操作时出错。";
        return;
    }

    QTextStream outputJsStream(&outputJsFile);
    QTextCodec *utf8Codec = QTextCodec::codecForName("UTF-8");
    outputJsStream.setCodec(utf8Codec);
    outputJsStream << jsContent;
    outputJsFile.close();
}

void Editor::updateWaitLoadJS(const QString& settingFilePath, const QString& jsFilePath) {
    // 读取 Timer_setting.txt 文件内容
    QFile settingFile(settingFilePath);
    if (!settingFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "打开"<< settingFilePath << "文件时出错。";
        return;
    }

    QTextStream settingStream(&settingFile);

    // 逐行读取数据
    QString wait = settingStream.readLine();
    QString time = settingStream.readLine();
    QString timeout = settingStream.readLine();
    settingFile.close();

    // 读取 Timer.js 文件内容
    QFile jsFile(jsFilePath);
    if (!jsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "打开"<< jsFilePath << "文件时出错。";
        return;
    }

    QTextStream jsStream(&jsFile);
    QString jsContent = jsStream.readAll();
    jsFile.close();

    // 静态 QRegularExpression 对象
    static const QRegularExpression switch1Pattern("var wait\\s*=\\s*\\d+;");
    static const QRegularExpression switch2Pattern("var time\\s*=\\s*\\d+;");
    static const QRegularExpression aimhoursPattern("var timeout\\s*=\\s*\\d+;");

    // 替换变量值
    jsContent.replace(switch1Pattern, "var wait = " + wait + ";");
    jsContent.replace(switch2Pattern, "var time = " + time + ";");
    jsContent.replace(aimhoursPattern, "var timeout = " + timeout + ";");

    // 写回文件
    QFile outputJsFile(jsFilePath);
    if (!outputJsFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "打开 Timer.js 文件进行写操作时出错。";
        return;
    }

    QTextStream outputJsStream(&outputJsFile);
    QTextCodec *utf8Codec = QTextCodec::codecForName("UTF-8");
    outputJsStream.setCodec(utf8Codec);
    outputJsStream << jsContent;
    outputJsFile.close();
}

void Editor::updateStarJS(const QString& settingFilePath, const QString& jsFilePath) {
    // 读取 txt 文件内容
    QFile settingFile(settingFilePath);
    if (!settingFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "打开"<< settingFilePath << "文件时出错。";
        return;
    }

    QTextStream settingStream(&settingFile);

    // 逐行读取数据
    QString starswitch = settingStream.readLine();
    QString color = settingStream.readLine();
    QString starmax = settingStream.readLine();
    QString starmin = settingStream.readLine();
    settingFile.close();

    // 读取 js 文件内容
    QFile jsFile(jsFilePath);
    if (!jsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "打开"<< jsFilePath << "文件时出错。";
        return;
    }

    QTextStream jsStream(&jsFile);
    QString jsContent = jsStream.readAll();
    jsFile.close();

    // 静态 QRegularExpression 对象
    static const QRegularExpression starswitchPattern("var starswitch\\s*=\\s*\\d+;");
    static const QRegularExpression colorPattern("var color\\s*=\\s*\".*\";");
    static const QRegularExpression starmaxPattern("var starmax\\s*=\\s*\\d+;");
    static const QRegularExpression starminPattern("var starmin\\s*=\\s*\\d+;");

    // 替换变量值
    jsContent.replace(starswitchPattern, "var starswitch = " + starswitch + ";");
    jsContent.replace(colorPattern, "var color = \"" + color + "\";");
    jsContent.replace(starmaxPattern, "var starmax = " + starmax + ";");
    jsContent.replace(starminPattern, "var starmin = " + starmin + ";");

    // 写回文件
    QFile outputJsFile(jsFilePath);
    if (!outputJsFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "打开 js 文件进行写操作时出错。";
        return;
    }

    QTextStream outputJsStream(&outputJsFile);
    QTextCodec *utf8Codec = QTextCodec::codecForName("UTF-8");
    outputJsStream.setCodec(utf8Codec);
    outputJsStream << jsContent;
    outputJsFile.close();
}

void Editor::updateCoutdownJS(const QString& settingFilePath, const QString& jsFilePath) {
    // 读取 txt 文件内容
    QFile settingFile(settingFilePath);
    if (!settingFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "打开"<< settingFilePath << "文件时出错。";
        return;
    }

    QTextStream settingStream(&settingFile);

    // 逐行读取数据
    QString dateswitch = settingStream.readLine();
    QString date = settingStream.readLine();
    settingFile.close();

    // 读取 js 文件内容
    QFile jsFile(jsFilePath);
    if (!jsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "打开"<< jsFilePath << "文件时出错。";
        return;
    }

    QTextStream jsStream(&jsFile);
    QString jsContent = jsStream.readAll();
    jsFile.close();

    // 静态 QRegularExpression 对象
    static const QRegularExpression dateswitchPattern("var setting\\s*=\\s*\\d+;");
    static const QRegularExpression datePattern("var settingDAY\\s*=\\s*\".*\";");

    // 替换变量值
    jsContent.replace(dateswitchPattern, "var setting = " + dateswitch + ";");
    jsContent.replace(datePattern, "var settingDAY = \"" + date + "\";");

    // 写回文件
    QFile outputJsFile(jsFilePath);
    if (!outputJsFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "打开 js 文件进行写操作时出错。";
        return;
    }

    QTextStream outputJsStream(&outputJsFile);
    QTextCodec *utf8Codec = QTextCodec::codecForName("UTF-8");
    outputJsStream.setCodec(utf8Codec);
    outputJsStream << jsContent;
    outputJsFile.close();
}

void Editor::updateTextBoxJS(const QString& settingFilePath, const QString& jsFilePath) {
    // 读取 txt 文件内容
    QFile settingFile(settingFilePath);
    if (!settingFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "打开"<< settingFilePath << "文件时出错。";
        return;
    }

    QTextStream settingStream(&settingFile);

    // 逐行读取数据
    QString BOXswitch = settingStream.readLine();
    QString APIswitch = settingStream.readLine();

    QString line1 = settingStream.readLine();
    QString line2 = settingStream.readLine();

    settingFile.close();

    // 读取 js 文件内容
    QFile jsFile(jsFilePath);
    if (!jsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "打开"<< jsFilePath << "文件时出错。";
        return;
    }

    QTextStream jsStream(&jsFile);
    QString jsContent = jsStream.readAll();
    jsFile.close();

    // 静态 QRegularExpression 对象
    static const QRegularExpression BOXswitchPattern("var BOXswitch\\s*=\\s*\\d+;");
    static const QRegularExpression APIswitchPattern("var APIswitch\\s*=\\s*\\d+;");
    static const QRegularExpression line1Pattern("var line1\\s*=\\s*\".*\";");
    static const QRegularExpression line2Pattern("var line2\\s*=\\s*\".*\";");
    // 替换变量值
    jsContent.replace(BOXswitchPattern, "var BOXswitch = " + BOXswitch + ";");
    jsContent.replace(APIswitchPattern, "var APIswitch = " + APIswitch + ";");
    jsContent.replace(line1Pattern, "var line1 = \"" + line1 + "\";");
    jsContent.replace(line2Pattern, "var line2 = \"" + line2 + "\";");
    // 写回文件
    QFile outputJsFile(jsFilePath);
    if (!outputJsFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "打开 js 文件进行写操作时出错。";
        return;
    }

    QTextStream outputJsStream(&outputJsFile);
    QTextCodec *utf8Codec = QTextCodec::codecForName("UTF-8");
    outputJsStream.setCodec(utf8Codec);

    outputJsStream << jsContent;
    outputJsFile.close();
}

void Editor::writeTextToJS(const QString& textFilePath, const QString& jsFilePath) {
    // 读取 text.txt 文件内容
    QFile textFile(textFilePath);
    if (!textFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "打开" << textFilePath << "文件时出错。";
        return;
    }

    QTextStream textStream(&textFile);

    // 逐行读取数据
    QStringList variableLines;
    while (!textStream.atEnd()) {
        variableLines << textStream.readLine();
    }

    textFile.close();

    // 读取 text.js 文件内容
    QFile jsFile(jsFilePath);
    if (!jsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "打开" << jsFilePath << "文件时出错。";
        return;
    }

    QTextStream jsStream(&jsFile);
    QTextCodec *utf8Codec = QTextCodec::codecForName("UTF-8");
    jsStream.setCodec(utf8Codec);
    QString jsContent = jsStream.readAll();
    jsFile.close();

    // 遍历 selectors 数组
    for (const QString& selector : selectors) {
        // 在 text.js 中查找对应的变量数组
        QString variablePattern = QString("var %1\\s*=\\s*\\[.*\\];").arg(selector);
        QRegularExpression re(variablePattern);
        QRegularExpressionMatch match = re.match(jsContent);

        if (match.hasMatch()) {
            // 在变量数组的末尾添加值
            QString line = variableLines.takeFirst();
            jsContent.replace(match.capturedStart(), match.capturedLength(), QString("var %1 = %2;").arg(selector, line));
        }
    }

    // 写回 text.js 文件
    QFile outputJsFile(jsFilePath);
    if (!outputJsFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "打开 js 文件进行写操作时出错。";
        return;
    }

    outputJsFile.write(QByteArray::fromHex("EFBBBF"));

    QTextStream outputJsStream(&outputJsFile);
    QTextCodec *utf8Codec1 = QTextCodec::codecForName("UTF-8");
    outputJsStream.setCodec(utf8Codec1);
    outputJsStream << jsContent;
    outputJsFile.close();
}

void Editor::on_save_released()
{
    save_bg();
    assignLineEditToSchedule();
    save_other();
    updateTimerJS(real_path + "/cache/Timer_setting.txt", real_path + "/files/js/Timer.js");
    updateScheduleJS(real_path + "/cache/schedule.txt", real_path + "/files/js/Schedule.js");
    writeRemainingVariables(real_path + "/cache/schedule.txt");
    updateBackgroundJS( real_path + "/cache/background_setting.txt", real_path + "/files/js/background.js");
    updateWaitLoadJS( real_path + "/cache/WaitLoad_setting.txt", real_path + "/files/js/WaitLoad.js");
    updateStarJS( real_path + "/cache/star_setting.txt", real_path + "/files/js/Star.js");
    updateCoutdownJS( real_path + "/cache/countdownday_setting.txt", real_path + "/files/js/countdown.js");
    updateTextBoxJS( real_path + "/cache/textbox_setting.txt", real_path + "/files/js/TextBox.js");
    writeTextToJS( real_path + "/cache/text_setting.txt", real_path + "/files/js/TextSetting.js");
    settime();
    QMessageBox msgBox;
    msgBox.setIconPixmap(QPixmap(":/icons/done64.png"));
    msgBox.setText("\n 保存成功  ");
    msgBox.setWindowTitle("成功");
    msgBox.setWindowIcon(QIcon(":/icons/icon.bmp"));
    msgBox.exec();
    // 设置主窗口为父对象，使消息框以主窗口为中心
    msgBox.setParent(this);
    msgBox.setWindowModality(Qt::WindowModal);


}

void Editor::sendPostRequestToTargetPage()
{

}

void Editor::on_textBrowser_anchorClicked(const QUrl &link)
{
    QDesktopServices::openUrl(link);
}
