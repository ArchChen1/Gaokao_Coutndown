// color3.h
#ifndef COLOR3_H
#define COLOR3_H

#include <QColorDialog>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class Color3 : public QWidget {
    Q_OBJECT

public:
    explicit Color3(QWidget *parent = nullptr) : QWidget(parent) {
        colorButton = new QPushButton("选择颜色", this);
        connect(colorButton, &QPushButton::clicked, this, &Color3::pickColor);

        layout = new QVBoxLayout(this);
        layout->addWidget(colorButton);

        setLayout(layout);
    }

    QColor getColor() const {
        return currentColor;
    }

    void setCurrentColor(const QColor &color) {
        currentColor = color;
    }

    QPushButton* getColorButton() const {
        return colorButton;
    }

signals:
    void colorSelected(const QColor &color);

private slots:
    void pickColor() {
        QColor color = QColorDialog::getColor(currentColor, this, "Select Color");

        if (color.isValid()) {
            currentColor = color;
            emit colorSelected(currentColor);
        }
    }

private:
    QColor currentColor;
    QVBoxLayout *layout;
    QPushButton *colorButton;
};

#endif // COLOR3_H
