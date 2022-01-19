#include "companywidget.h"

bool CompanyWidget::dataExists = false;

CompanyWidget::CompanyWidget(QWidget *parent) : QWidget(parent)
{
    //loadCompanyData();
    initializeUI();
    addToLayout();
    connectUI();
}


const bool CompanyWidget::isExists(){
    return dataExists;
}


void CompanyWidget::initializeUI(){
    for(int i = 0; i < 20; ++i)
        labels.push_back(new QLabel);

    for(int i = 0; i < labels.size(); ++i){
        labels.at(i)->setStyleSheet(Data::boldLabelGreater);
        labels.at(i)->setMargin(5);
        if(i > 9) labels.at(i)->setText("Brak");
    }

    labels.at(0)->setText("Nazwa firmy:");
    labels.at(1)->setText("Przedstawiciel:");
    labels.at(2)->setText("NIP:");
    labels.at(3)->setText("Nr. Regon:");
    labels.at(4)->setText("Miasto:");
    labels.at(5)->setText("Kod pocztowy:");
    labels.at(6)->setText("Ulica:");
    labels.at(7)->setText("E-mail:");
    labels.at(8)->setText("Telefon:");
    labels.at(9)->setText("Urząd Skarbowy:");

    changeDataBtn = new QPushButton("Dodaj");
    changeDataBtn->setFixedSize(Data::defaultButtonSize);
    changeDataBtn->setStyleSheet(Data::defaultButtonSheet);

    if(dataExists){
        QSqlQuery query;
        query.exec("SELECT name, director, nip, regon, city, cityCode, address, email, phone, office FROM mainCompany WHERE id = 1");

        if(query.next()){
            labels.at(10)->setText(query.value(0).toString());
            labels.at(11)->setText(query.value(1).toString());
            labels.at(12)->setText(query.value(2).toString());
            labels.at(13)->setText(query.value(3).toString());
            labels.at(14)->setText(query.value(4).toString());
            labels.at(15)->setText(query.value(5).toString());
            labels.at(16)->setText(query.value(6).toString());
            labels.at(17)->setText(query.value(7).toString());
            labels.at(18)->setText(query.value(8).toString());
            labels.at(19)->setText(query.value(9).toString());
            changeDataBtn->setText("Edytuj");
        }
    }
}


void CompanyWidget::addToLayout(){
    QVBoxLayout* layout = new QVBoxLayout;

    QGridLayout* labelsLayout = new QGridLayout;
    labelsLayout->setSpacing(1);
    labelsLayout->setMargin(0);

    for(int i = 0; i < labels.size(); ++i){
        if(i < 10) labelsLayout->addWidget(labels.at(i), i, 0);
        else labelsLayout->addWidget(labels.at(i), i - 10, 1);
    }

    layout->addLayout(labelsLayout);
    layout->addWidget(changeDataBtn);
    setLayout(layout);
}


void CompanyWidget::connectUI(){
    if(DBManager::isConnected()){
        connect(changeDataBtn, &QPushButton::clicked, this, &CompanyWidget::changeCompanyData);
    }
}


void CompanyWidget::loadCompanyData(){
    if(DBManager::isConnected()){
        QSqlQuery query;
        query.exec("SELECT name FROM mainCompany WHERE id = 1");
        if(query.next()){
            dataExists = true;
        }
        else{
            dataExists = false;
        }
    }
    /*else{
        QMessageBox::warning(this, "Błąd", "Proszę najpierw otworzyć bazę danych.");
    }*/
}


int CompanyWidget::changeCompanyData(){
    CompanyDialog* dialog = new CompanyDialog(dataExists);
    return dialog->exec();
}
