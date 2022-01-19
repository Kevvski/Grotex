#include "companydialog.h"

CompanyDialog::CompanyDialog(bool dataExists, const QString& id)
{
    this->dataExists = dataExists;
    this->companyId = id;
    setStyleSheet("background-color: white");
    setWindowTitle("KadriX - Dodaj firmę");
    initializeUI();
    addToLayout();
    connectUI();
}


void CompanyDialog::initializeUI(){
    for(int i = 0; i < 10; ++i){
        labels.push_back(new QLabel);
        lineEdits.push_back(new QLineEdit);
    }

    for(auto& label : labels){
        label->setStyleSheet(Data::boldLabel);
        label->setMargin(5);
    }

    for(auto& lineEdit : lineEdits){
        lineEdit->setStyleSheet(Data::defaultLineEdit);
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

    if(AppState::getState() == AppState::State::COMPANY)
        labels.at(9)->setText("Urząd Skarbowy:");
    else if(AppState::getState() == AppState::State::CUSTOMERS)
        labels.at(9)->setText("Nr. Konta:");

    saveBtn = new QPushButton("Zapisz");
    saveBtn->setFixedSize(Data::defaultButtonSize);
    saveBtn->setStyleSheet(Data::defaultButtonSheet);

    cancelBtn = new QPushButton("Anuluj");
    cancelBtn->setFixedSize(Data::defaultButtonSize);
    cancelBtn->setStyleSheet(Data::defaultButtonSheet);

    if(AppState::getState() == AppState::State::COMPANY && dataExists){
        QSqlQuery query;
        query.exec("SELECT name, director, nip, regon, city, cityCode, address, email, phone, office FROM mainCompany WHERE id = 1");

        if(query.next()){
            for(int i = 0; i < lineEdits.size(); ++i)
                lineEdits.at(i)->setText(query.value(i).toString());
        }
    }
    else if(AppState::getState() == AppState::State::CUSTOMERS && dataExists){
        QSqlQuery query;
        query.exec("SELECT name, director, nip, regon, city, cityCode, address, email, phone, account FROM companies WHERE id = " + companyId);

        if(query.next()){
            for(int i = 0; i < lineEdits.size(); ++i)
                lineEdits.at(i)->setText(query.value(i).toString());
        }
    }
}


void CompanyDialog::addToLayout(){
    QGridLayout* layout = new QGridLayout;

    for(int i = 0; i < labels.size(); ++i)
        layout->addWidget(labels.at(i), i, 0);

    for(int i = 0; i < lineEdits.size(); ++i)
        layout->addWidget(lineEdits.at(i), i, 1);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->setAlignment(Qt::AlignRight);
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(cancelBtn);

    layout->addLayout(btnLayout, lineEdits.size() + 1, 1);
    setLayout(layout);
}


void CompanyDialog::connectUI(){
    if(AppState::getState() == AppState::State::COMPANY)
        connect(saveBtn, &QPushButton::clicked, this, &CompanyDialog::saveCompanyData);
    else if(AppState::getState() == AppState::State::CUSTOMERS)
        connect(saveBtn, &QPushButton::clicked, this, &CompanyDialog::saveCompaniesData);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::close);
}


void CompanyDialog::saveCompanyData(){
    QSqlQuery query;

    if(dataExists){
        QString updateVariables {
            "name = '" + lineEdits.at(0)->text() + "'"
            ",director = '" + lineEdits.at(1)->text() + "'"
            ",nip = '" + lineEdits.at(2)->text() + "'"
            ",regon = '" + lineEdits.at(3)->text() + "'"
            ",city = '" + lineEdits.at(4)->text() + "'"
            ",cityCode = '" + lineEdits.at(5)->text() + "'"
            ",address = '" + lineEdits.at(6)->text() + "'"
            ",email = '" + lineEdits.at(7)->text() + "'"
            ",phone = '" + lineEdits.at(8)->text() + "'"
            ",office = '" + lineEdits.at(9)->text() + "'"
        };

        query.exec("UPDATE mainCompany SET " + updateVariables + " WHERE id = 1");

        QMessageBox::information(this, "Sukces!", "Dane firmy zaktualizowano pomyślnie.");
        close();
    }
    else{
        QString tableVariables = "(id, name, director, nip, regon, city, cityCode, address, email, phone, office)";
        QString bindVariables = "(:id, :name, :director, :nip, :regon, :city, :cityCode, :address, :email, :phone, :office)";

        query.prepare("INSERT INTO mainCompany " + tableVariables + " values" + bindVariables);
        query.bindValue(":id", 1);
        query.bindValue(":name", lineEdits.at(0)->text());
        query.bindValue(":director", lineEdits.at(1)->text());
        query.bindValue(":nip", lineEdits.at(2)->text());
        query.bindValue(":regon", lineEdits.at(3)->text());
        query.bindValue(":city", lineEdits.at(4)->text());
        query.bindValue(":cityCode", lineEdits.at(5)->text());
        query.bindValue(":address", lineEdits.at(6)->text());
        query.bindValue(":email", lineEdits.at(7)->text());
        query.bindValue(":phone", lineEdits.at(8)->text());
        query.bindValue(":office", lineEdits.at(9)->text());
        query.exec();

        QMessageBox::information(this, "Sukces!", "Dane firmy dodano pomyślnie.");
        close();
    }
}


void CompanyDialog::saveCompaniesData(){
    QSqlQuery query;
    const auto result = std::count_if(lineEdits.begin(), lineEdits.end(), [](QLineEdit* edit){
        return edit->text().isEmpty();
    });

    if(result == 0){
        if(dataExists){
            QString updateVariables {
                "name = '" + lineEdits.at(0)->text() + "'"
                ",director = '" + lineEdits.at(1)->text() + "'"
                ",nip = '" + lineEdits.at(2)->text() + "'"
                ",regon = '" + lineEdits.at(3)->text() + "'"
                ",city = '" + lineEdits.at(4)->text() + "'"
                ",cityCode = '" + lineEdits.at(5)->text() + "'"
                ",address = '" + lineEdits.at(6)->text() + "'"
                ",email = '" + lineEdits.at(7)->text() + "'"
                ",phone = '" + lineEdits.at(8)->text() + "'"
                ",account = '" + lineEdits.at(9)->text() + "'"
            };

            query.exec("UPDATE companies SET " + updateVariables + " WHERE id = " + companyId);
            query.exec("UPDATE account SET companyName = '" + lineEdits.at(0)->text() + "' WHERE id = " + companyId);

            QMessageBox::information(this, "Sukces!", "Dane firmy zaktualizowano pomyślnie.");
            close();
        }
        else{
            QString tableVariables = "(name, director, nip, regon, city, cityCode, address, email, phone, account)";
            QString bindVariables = "(:name, :director, :nip, :regon, :city, :cityCode, :address, :email, :phone, :account)";

            query.prepare("INSERT INTO companies " + tableVariables + " values" + bindVariables);
            query.bindValue(":name", lineEdits.at(0)->text());
            query.bindValue(":director", lineEdits.at(1)->text());
            query.bindValue(":nip", lineEdits.at(2)->text());
            query.bindValue(":regon", lineEdits.at(3)->text());
            query.bindValue(":city", lineEdits.at(4)->text());
            query.bindValue(":cityCode", lineEdits.at(5)->text());
            query.bindValue(":address", lineEdits.at(6)->text());
            query.bindValue(":email", lineEdits.at(7)->text());
            query.bindValue(":phone", lineEdits.at(8)->text());
            query.bindValue(":account", lineEdits.at(9)->text());
            query.exec();

            query.prepare("INSERT INTO account (companyName, balance) values (:companyName, :balance)");
            query.bindValue(":companyName", lineEdits.at(0)->text());
            query.bindValue(":balance", 0.0f);
            query.exec();

            QMessageBox::information(this, "Sukces!", "Pomyślnie dodano nową firmę.");
            close();
        }
    }
    else{
        QMessageBox::information(this, "Ups!", "Proszę o wypełnienie wszystkich danych.");
    }
}
