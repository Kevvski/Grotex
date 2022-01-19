#include "hoverlabel.h"

HoverLabel::HoverLabel(QWidget* parent) : QLabel(parent)
{
}


void HoverLabel::setEnterStyle(const QString &text){
    enterStyleSheet = text;
}


void HoverLabel::setLeaveStyle(const QString &text){
    leaveStyleSheet = text;
}


void HoverLabel::enterEvent(QEvent *ev){
    setStyleSheet(enterStyleSheet);
}


void HoverLabel::leaveEvent(QEvent *ev){
    setStyleSheet(leaveStyleSheet);
}

