#include "imagebutton.h"

ImageButton::ImageButton(const QString& text)
{
    icon = new QLabel;
    icon->setAlignment(Qt::AlignCenter);
    icon->setStyleSheet("QLabel{ background-color: #1c45a6; padding: 10px;} QLabel:hover{ background-color: #2667ff; }");

    this->text = new QLabel;
    this->text->setAlignment(Qt::AlignCenter);
    this->text->setStyleSheet("QLabel{ background-color: #1c45a6; color: white; font-weight: bold; font-size: 15px; }");
    if(!text.isEmpty())
        this->text->setText(text);
}


void ImageButton::setButtonImage(const QString &fileName){
    QDir currentPath;
    if(pixmap.load(currentPath.currentPath() + "/Dane/Images/" + fileName)){
        pixmap = pixmap.scaled(QSize(80, 80), Qt::KeepAspectRatio, Qt::TransformationMode::SmoothTransformation);
        icon->setPixmap(pixmap);
    }
    else{
        QMessageBox::information(this, "Brak pliku", "Nie można załadować " + fileName);
    }
}


void ImageButton::create(){
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(icon);
    layout->addWidget(text);
    setLayout(layout);
}
