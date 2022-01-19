#include "checkcontractsdialog.h"

CheckContractsDialog::CheckContractsDialog()
{
    setWindowTitle("Coś znalazłem...");
    setStyleSheet("background-color: white;");
    initializeUI();
    connectUI();
}


void CheckContractsDialog::checkContracts(){
    if(searchContracts("jobContracts") || searchContracts("orderContracts") || searchContracts("workContracts")){
        addToLayout();
        show();
    }
}


void CheckContractsDialog::initializeUI(){
    headerLabel = new QLabel("Wykryto następujące umowy, które dzisiaj uległy przedawnieniu:");
    headerLabel->setStyleSheet(Data::boldLabel);

    okBtn = new QPushButton("Ok");
    okBtn->setFixedSize(Data::defaultButtonSize);
    okBtn->setStyleSheet(Data::defaultButtonSheet);
}


void CheckContractsDialog::addToLayout(){
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(headerLabel);

    for(auto& label : labels){
        layout->addWidget(label);
    }

    layout->addWidget(okBtn);
    setLayout(layout);
}


void CheckContractsDialog::connectUI(){
    connect(okBtn, &QPushButton::clicked, this, &QDialog::close);
}


bool CheckContractsDialog::searchContracts(const QString &tableName){
    if(!tableName.isEmpty()){
        QVector<Contract> contracts;
        QSqlQuery query;

        query.exec("SELECT docId, id, employeeId, jobEndDate FROM " + tableName);
        while(query.next()){
            contracts.push_back(Contract{});
        }

        for(auto& contract : contracts){
            if(contract.endDate == QDate::currentDate()){
                labels.push_back(new QLabel(contract.textId));

                query.exec("UPDATE " + tableName + " SET active = false WHERE docId = " + contract.id);

                if(tableName == "jobContracts")
                    query.exec("UPDATE employees SET uop = false WHERE id = " + QString::number(contract.employeeId));
                else if(tableName == "orderContracts")
                    query.exec("UPDATE employees SET uz = false WHERE id = " + QString::number(contract.employeeId));
                else if(tableName == "workContracts")
                   query.exec("UPDATE employees SET uod = false WHERE id = " + QString::number(contract.employeeId));

                //If all contracts are false for employee then activeWork is false
                query.exec("SELECT uop, uz, uod FROM employees WHERE id = " + QString::number(contract.employeeId));
                if(query.next()){
                    /*if(){
                        query.exec("UPDATE employees SET activeWork = false WHERE id = " + QString::number(contract.employeeId));
                    }*/
                }
            }
        }

        if(labels.size() != 0)
            return true;

        return false;
    }

    return false;
}
