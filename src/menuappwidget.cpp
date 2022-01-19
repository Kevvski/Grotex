#include "menuappwidget.h"

MenuAppWidget::MenuAppWidget(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("background-color: #1c45a6;");
    setFixedHeight(60);

    QString btnSheet = "QPushButton {"
                       "color: white;"
                       "font-size: 15px;"
                       "border: 1px solid #1c45a6;"
                       "}"
                       "QPushButton:hover:!pressed{"
                       "background-color: #2667ff"
                       "}";

    const QSize btnSize { 100, 60 };

    logoLabel = new QLabel("KadriX");
    logoLabel->setFixedWidth(700);
    logoLabel->setContentsMargins(50, 0, 0, 0);
    logoLabel->setStyleSheet("color: white; font-size: 25px;");

    companyButton = new QPushButton("Firma");
    companyButton->setFixedSize(btnSize);
    companyButton->setStyleSheet(btnSheet);

    employeesButton = new QPushButton("Kadra");
    employeesButton->setFixedSize(btnSize);
    employeesButton->setStyleSheet(btnSheet);

    customersButton = new QPushButton("Finanse");
    customersButton->setFixedSize(btnSize);
    customersButton->setStyleSheet(btnSheet);

    databasesButton = new QPushButton("Bazy");
    databasesButton->setFixedSize(btnSize);
    databasesButton->setStyleSheet(btnSheet);

    cashButton = new QPushButton("Kasa");
    cashButton->setFixedSize(btnSize);
    cashButton->setStyleSheet(btnSheet);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(logoLabel);
    layout->addWidget(companyButton);
    layout->addWidget(cashButton);
    layout->addWidget(employeesButton);
    layout->addWidget(customersButton);
    layout->addWidget(databasesButton);
    setLayout(layout);
}


QPushButton* MenuAppWidget::getCompanyBtn(){
    return companyButton;
}


QPushButton* MenuAppWidget::getEmplyeesBtn(){
    return employeesButton;
}


QPushButton* MenuAppWidget::getCustomersBtn(){
    return  customersButton;
}


QPushButton* MenuAppWidget::getDatabasesBtn(){
    return databasesButton;
}


QPushButton* MenuAppWidget::getCashBtn(){
    return cashButton;
}
