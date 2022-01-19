#include "employeedatawidget.h"

EmployeeDataWidget::EmployeeDataWidget(const QString& employeeID, bool workContract, QWidget *parent) : QWidget(parent)
{
    initializeUI(employeeID);
    addToLayout(workContract);
}


void EmployeeDataWidget::initializeUI(const QString &employeeID){
    for(int i = 0; i < 32; ++i)
        labels.push_back(new QLabel);

    for(auto& label : labels){
        label->setStyleSheet(Data::boldLabel);
        label->setMargin(5);
    }

    QSqlQuery query;
    query.exec("SELECT id, name, secondName, surname, birthDate, birthPlace, cityCode, city, address, houseNumber, localNumber, pesel, parentName, parentSurname, evidenceNumber FROM employees WHERE id = " + employeeID);
    if(query.next()){

        for(int i = 0; i < labels.size(); ++i){
            if(i < 16)
                labels.at(i)->setText(DBManager::getEmployeeVariable(i));
            else
                labels.at(i)->setText(query.value(i - 16).toString());
        }

    }
}


void EmployeeDataWidget::addToLayout(bool workContract){
    QGridLayout* layout = new QGridLayout;

    if(!workContract){
        for(int i = 0; i < labels.size(); ++i){
            if(i < 16)
                layout->addWidget(labels.at(i), i, 0);
            else
                layout->addWidget(labels.at(i), i - 16, 1);
        }
    }
    else{
        layout->addWidget(labels.at(1), 0, 0);
        layout->addWidget(labels.at(3), 1, 0);
        layout->addWidget(labels.at(11), 2, 0);
        layout->addWidget(labels.at(15), 3, 0);

        layout->addWidget(labels.at(17), 0, 1);
        layout->addWidget(labels.at(19), 1, 1);
        layout->addWidget(labels.at(27), 2, 1);
        layout->addWidget(labels.at(31), 3, 1);
    }

    setLayout(layout);
}
