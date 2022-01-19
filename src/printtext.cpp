#include "printtext.h"

PrintText::PrintText()
{

}


void PrintText::setText(const QString &text){
    this->text = text;

    QFontMetrics fm(font);
    size = QSize(fm.width(this->text), fm.height());
}


void PrintText::setFont(const QString& family, int textSize){
    font.setFamily(family);
    font.setPointSize(textSize);
}


void PrintText::setWeight(int weight){
    font.setWeight(weight);
}


const QSize& PrintText::getSize() const{
    return size;
}


const QFont& PrintText::getFont() const{
    return font;
}


QString& PrintText::operator()(){
    return text;
}
