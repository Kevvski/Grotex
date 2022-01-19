#include "testwidget.h"

TestWidget::TestWidget(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("background-color: darkgrey");

    btn = new QLabel("Test");
    btn->setFixedWidth(200);
    btn->setAlignment(Qt::AlignCenter);
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(btn);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}
