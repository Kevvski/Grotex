#include "settlementsdialog.h"

SettlementsDialog::SettlementsDialog(const SettlementsDialog::State& state)
{
    this->state = state;
    setStyleSheet("background-color: white");
    initializeUI();
    addToLayout();
    connectUI();

    switch(this->state){
        case SettlementsDialog::State::RECEIVABLES: setWindowTitle("KadriX - Należności"); break;
        case SettlementsDialog::State::COMMITMENT: setWindowTitle("KadriX - Zobowiązania"); break;
    }
}


void SettlementsDialog::initializeUI(){
    header = new QLabel;
    header->setAlignment(Qt::AlignCenter);
    header->setStyleSheet("QLabel{ font-size: 25px; font-weight: bold; }");

    table = new QTableWidget;

    switch(state){
        case SettlementsDialog::State::RECEIVABLES:
            table->setFixedSize(902, 500);
            header->setText("Należności");
            initializeReceivables();
            break;

        case SettlementsDialog::State::COMMITMENT:
            table->setFixedSize(1102, 500);
            header->setText("Zobowiązania");
            initializeCommitment();
            break;
    }

    cancelBtn = new QPushButton("Wróć");
    cancelBtn->setStyleSheet(Data::defaultButtonSheet);
    cancelBtn->setFixedSize(Data::defaultButtonSize);
}


void SettlementsDialog::addToLayout(){
    QVBoxLayout* layout = new QVBoxLayout;
    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->setAlignment(Qt::AlignRight);
    btnLayout->addWidget(cancelBtn);

    layout->addWidget(header);
    layout->addWidget(table);
    layout->addLayout(btnLayout);
    setLayout(layout);
}


void SettlementsDialog::connectUI(){
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::close);
}


void SettlementsDialog::initializeReceivables(){
    QSqlQuery query;
    QStringList companyId, name, docId, createDate, payDate, sumNetto, sumVat, sumBrutto, status;

    table->setColumnCount(9);
    table->setHorizontalHeaderLabels(QStringList{ "LP", "Kontrahent", "Nr. Faktury", "Data Wystawienia", "Data Płatności", "Netto", "VAT", "Brutto", "Status" });

    query.exec("SELECT companyId, docId, createDate, payDate, sumNetto, sumVat, sumBrutto, status FROM finance WHERE type = 'FS' AND status = 'Nie'");
    while(query.next()){
        companyId.push_back(query.value(0).toString());
        docId.push_back(query.value(1).toString());
        createDate.push_back(query.value(2).toString());
        payDate.push_back(query.value(3).toString());
        sumNetto.push_back(query.value(4).toString());
        sumVat.push_back(query.value(5).toString());
        sumBrutto.push_back(query.value(6).toString());
        status.push_back(query.value(7).toString());
    }

    for(int i = 0; i < companyId.size(); ++i){
        query.exec("SELECT name FROM companies WHERE id = " + companyId.at(i));
        if(query.next()){
            name.push_back(query.value(0).toString());
        }
    }

    table->setRowCount(companyId.size());

    for(int i = 1; i < companyId.size() + 1; ++i){
        table->setItem(i - 1, 0, new QTableWidgetItem(QString::number(i)));
        table->setItem(i - 1, 1, new QTableWidgetItem(name.at(i - 1)));
        table->setItem(i - 1, 2, new QTableWidgetItem(docId.at(i - 1)));
        table->setItem(i - 1, 3, new QTableWidgetItem(createDate.at(i - 1)));
        table->setItem(i - 1, 4, new QTableWidgetItem(payDate.at(i - 1)));
        table->setItem(i - 1, 5, new QTableWidgetItem(sumNetto.at(i - 1)));
        table->setItem(i - 1, 6, new QTableWidgetItem(sumVat.at(i - 1)));
        table->setItem(i - 1, 7, new QTableWidgetItem(sumBrutto.at(i - 1)));
        table->setItem(i - 1, 8, new QTableWidgetItem(status.at(i - 1)));
    }
}


void SettlementsDialog::initializeCommitment(){
    QSqlQuery query;
    QStringList companyId, name, accountNumber, docId, createDate, payDate, payType, sumNetto, sumVat, sumBrutto, status;

    table->setColumnCount(11);
    table->setHorizontalHeaderLabels(QStringList{ "LP", "Nazwa Firmy", "Nr. Faktury", "Data", "Termin Zapłaty", "Sposób Zapłaty", "Netto", "VAT", "Brutto", "Nr. Konta", "Status" });

    query.exec("SELECT companyId, docId, createDate, payDate, payType, sumNetto, sumVat, sumBrutto, status FROM finance WHERE type = 'FZ' AND status = 'Nie'");
    while(query.next()){
        companyId.push_back(query.value(0).toString());
        docId.push_back(query.value(1).toString());
        createDate.push_back(query.value(2).toString());
        payDate.push_back(query.value(3).toString());
        payType.push_back(query.value(4).toString());
        sumNetto.push_back(query.value(5).toString());
        sumVat.push_back(query.value(6).toString());
        sumBrutto.push_back(query.value(7).toString());
        status.push_back(query.value(8).toString());
    }

    for(int i = 0; i < companyId.size(); ++i){
        query.exec("SELECT name, account FROM companies WHERE id = " + companyId.at(i));
        if(query.next()){
            name.push_back(query.value(0).toString());
            accountNumber.push_back(query.value(1).toString());
        }
    }

    table->setRowCount(companyId.size());

    for(int i = 1; i < companyId.size() + 1; ++i){
        table->setItem(i - 1, 0, new QTableWidgetItem(QString::number(i)));
        table->setItem(i - 1, 1, new QTableWidgetItem(name.at(i - 1)));
        table->setItem(i - 1, 2, new QTableWidgetItem(docId.at(i - 1)));
        table->setItem(i - 1, 3, new QTableWidgetItem(createDate.at(i - 1)));
        table->setItem(i - 1, 4, new QTableWidgetItem(payDate.at(i - 1)));
        table->setItem(i - 1, 5, new QTableWidgetItem(payType.at(i - 1)));
        table->setItem(i - 1, 6, new QTableWidgetItem(sumNetto.at(i - 1)));
        table->setItem(i - 1, 7, new QTableWidgetItem(sumVat.at(i - 1)));
        table->setItem(i - 1, 8, new QTableWidgetItem(sumBrutto.at(i - 1)));
        table->setItem(i - 1, 9, new QTableWidgetItem(accountNumber.at(i - 1)));
        table->setItem(i - 1, 10, new QTableWidgetItem(status.at(i - 1)));
    }
}
