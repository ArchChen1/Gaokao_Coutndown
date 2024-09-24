// plus.cpp
#include "plus.h"
#include <QStringList>
#include <QRegularExpression>

QString plus(QString font_family, QString font_size, QString font_weight, QString offsetx, QString offsety, QString radius, QString shadow_color, QString text_color)
{
    QStringList shadowS;
    shadowS << offsetx << offsety << radius << shadow_color;
    QString result0 = "\"" + shadowS.join(" ") + "\"";

    // 判断 font_size 是否为数字，如果是数字则在后面添加 "rem"
    bool isFontSizeNumeric = QRegularExpression("^[0-9.]+$").match(font_size).hasMatch();
    QString result1 = isFontSizeNumeric ? "\"" + font_size + "rem\"" : font_size;

    QStringList stringList;
    stringList << "\"" + font_family + "\"" << result1 << font_weight << result0 << "\"" + text_color + "\"";

    return "[" + stringList.join(", ") + "]";
}
