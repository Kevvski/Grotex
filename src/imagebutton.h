#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QDir>
#include <QPixmap>
#include <QVBoxLayout>
#include <QMessageBox>

class ImageButton : public QPushButton
{
    Q_OBJECT

public:
    ImageButton(const QString& text);

    void setButtonImage(const QString& fileName);
    void create();

private:
    QPixmap pixmap;
    QLabel* icon;
    QLabel* text;
};

#endif // IMAGEBUTTON_H
