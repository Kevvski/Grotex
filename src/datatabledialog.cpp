#include "datatabledialog.h"

DataTableDialog::DataTableDialog(const DataTableDialog::State& state, QVector<Data::EvidenceRecord>& records, const QString& date)
{
    this->state = state;
    this->records = records;
    this->date = date;
    initializeUI();
    addToLayout();
    connectUI();
}


DataTableDialog::DataTableDialog(const DataTableDialog::State& state, QVector<Data::JobEvidenceRecord>& records, const QString& date)
{
    this->state = state;
    jobEvidenceRecords = records;
    this->date = date;
    initializeUI();
    addToLayout();
    connectUI();
}


DataTableDialog::DataTableDialog(const DataTableDialog::State& state, QVector<Data::BillRecord>& records, const QString& date)
{
    this->state = state;
    billRecords = records;
    this->date = date;
    initializeUI();
    addToLayout();
    connectUI();
}


void DataTableDialog::initializeUI(){
    setStyleSheet("background-color: white;");

    headerLabel = new QLabel;
    headerLabel->setAlignment(Qt::AlignCenter);
    headerLabel->setStyleSheet("QLabel{ font-size: 25px; font-weight: bold; }");

    table = new QTableWidget;
    table->setFixedSize(QSize{800, 500});

    printBtn = new QPushButton("Drukuj");
    printBtn->setStyleSheet(Data::defaultButtonSheet);
    printBtn->setFixedSize(Data::defaultButtonSize);

    cancelBtn = new QPushButton("Wróć");
    cancelBtn->setStyleSheet(Data::defaultButtonSheet);
    cancelBtn->setFixedSize(Data::defaultButtonSize);

    switch(state){
        case DataTableDialog::State::SICK_LEAVE:
            setWindowTitle("KadriX - Ewidencja zwolnień chorobowych");
            headerLabel->setText("Miesięczna ewidencja zwolnień chorobowych");
            table->setColumnCount(7);
            table->setHorizontalHeaderLabels(QStringList{"ID", "Imię", "Nazwisko", "Pesel", "Data rozpoczęcia", "Data zakończenia", "Ilość dni"});
            initializeLeaveTable();
            break;

        case DataTableDialog::State::FREE_LEAVE:
            setWindowTitle("KadriX - Ewidencja urlopowa");
            headerLabel->setText("Miesięczna ewidencja urlopów");
            table->setColumnCount(8);
            table->setHorizontalHeaderLabels(QStringList{"ID", "Imię", "Nazwisko", "Pesel", "Data rozpoczęcia", "Data zakończenia", "Ilość dni", "Rodzaj urlopu"});
            initializeLeaveTable();
            break;

        case DataTableDialog::State::JOB_TIME_EVIDENCE:
            setWindowTitle("KadriX - Ewidencja czasu pracy");
            headerLabel->setText("Miesięczna ewidencja czasu pracy");
            table->setColumnCount(9);
            table->setHorizontalHeaderLabels(QStringList{"ID", "Numer Pracownika", "Imię", "Nazwisko", "Miesiąc", "Rok", "Suma godzin", "Suma dni", "Rodzaj umowy"});
            initializeJobTimeEvidenceTable();
            break;

        case DataTableDialog::State::JOB_BILL_LIST:
            setWindowTitle("KadriX - Lista płac");
            headerLabel->setText("Miesięczna Lista Płac");
            table->setColumnCount(6);
            table->setHorizontalHeaderLabels(QStringList{"ID", "Numer Rozliczenia", "Kwota brutto", "Kwota netto", "Sposób płatności", "Data Utworzenia"});
            initializeJobBillTable();
            break;
    }
}


void DataTableDialog::initializeLeaveTable(){
    table->setRowCount(records.size());
    table->setColumnWidth(0, 80);
    table->setColumnWidth(1, 140);
    table->setColumnWidth(2, 140);
    table->setColumnWidth(3, 120);
    table->setColumnWidth(4, 120);
    table->setColumnWidth(5, 120);
    table->setColumnWidth(6, 60);

    if(state == DataTableDialog::State::FREE_LEAVE)
        table->setColumnWidth(7, 120);

    for(int i = 0; i < records.size(); ++i){
        QSqlQuery query;
        query.exec("SELECT name, surname, pesel FROM employees WHERE id = " + QString::number(records.at(i).employeeId));
        if(query.next()){
            records[i].name = query.value(0).toString();
            records[i].surname = query.value(1).toString();
            records[i].pesel = query.value(2).toString();
        }
    }

    for(int i = 0; i < records.size(); ++i){
        table->setItem(i, 0, new QTableWidgetItem(QString::number(records.at(i).id)));
        table->setItem(i, 1, new QTableWidgetItem(records.at(i).name));
        table->setItem(i, 2, new QTableWidgetItem(records.at(i).surname));
        table->setItem(i, 3, new QTableWidgetItem(records.at(i).pesel));
        table->setItem(i, 4, new QTableWidgetItem(records.at(i).startDate.toString("dd.MM.yyyy")));
        table->setItem(i, 5, new QTableWidgetItem(records.at(i).endDate.toString("dd.MM.yyyy")));
        table->setItem(i, 6, new QTableWidgetItem(QString::number(records.at(i).daysCount)));

        if(state == DataTableDialog::State::FREE_LEAVE)
            table->setItem(i, 7, new QTableWidgetItem(records.at(i).type));
    }
}


void DataTableDialog::initializeJobTimeEvidenceTable(){
    table->setRowCount(jobEvidenceRecords.size());
    table->setColumnWidth(0, 80);
    table->setColumnWidth(1, 140);
    table->setColumnWidth(2, 140);
    table->setColumnWidth(3, 120);
    table->setColumnWidth(4, 120);
    table->setColumnWidth(5, 120);
    table->setColumnWidth(6, 100);
    table->setColumnWidth(7, 100);
    table->setColumnWidth(8, 120);

    for(int i = 0; i < jobEvidenceRecords.size(); ++i){
        table->setItem(i, 0, new QTableWidgetItem(QString::number(jobEvidenceRecords.at(i).id)));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(jobEvidenceRecords.at(i).employeeId)));
        table->setItem(i, 2, new QTableWidgetItem(jobEvidenceRecords.at(i).name));
        table->setItem(i, 3, new QTableWidgetItem(jobEvidenceRecords.at(i).surname));
        table->setItem(i, 4, new QTableWidgetItem(jobEvidenceRecords.at(i).month));
        table->setItem(i, 5, new QTableWidgetItem(jobEvidenceRecords.at(i).year));
        table->setItem(i, 6, new QTableWidgetItem(QString::number(jobEvidenceRecords.at(i).sum)));
        table->setItem(i, 7, new QTableWidgetItem(QString::number(jobEvidenceRecords.at(i).daysSum)));
        table->setItem(i, 8, new QTableWidgetItem(jobEvidenceRecords.at(i).type));
    }
}


void DataTableDialog::initializeJobBillTable(){
    table->setRowCount(billRecords.size());

    for(int i = 0; i < billRecords.size(); ++i){
        table->setItem(i, 0, new QTableWidgetItem(QString::number(billRecords.at(i).id)));
        table->setItem(i, 1, new QTableWidgetItem(billRecords.at(i).docId));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(billRecords.at(i).rewardBrutto)));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(billRecords.at(i).rewardNetto)));
        table->setItem(i, 4, new QTableWidgetItem(billRecords.at(i).payType));
        table->setItem(i, 5, new QTableWidgetItem(billRecords.at(i).date.toString("dd.MM.yyyy")));
    }
}


void DataTableDialog::addToLayout(){
    QVBoxLayout* layout = new QVBoxLayout;
    QHBoxLayout* btnLayout = new QHBoxLayout;

    layout->addWidget(headerLabel);
    layout->addWidget(table);

    btnLayout->setAlignment(Qt::AlignRight);
    btnLayout->addWidget(printBtn);
    btnLayout->addWidget(cancelBtn);

    layout->addLayout(btnLayout);
    setLayout(layout);
}


void DataTableDialog::connectUI(){
    switch(state){
        case DataTableDialog::State::SICK_LEAVE: connect(printBtn, &QPushButton::clicked, this, &DataTableDialog::printSickEvidence); break;
        case DataTableDialog::State::FREE_LEAVE: connect(printBtn, &QPushButton::clicked, this, &DataTableDialog::printFreeEvidence); break;
        case DataTableDialog::State::JOB_TIME_EVIDENCE: connect(printBtn, &QPushButton::clicked, this, &DataTableDialog::printJobTimeEvidence); break;
        case DataTableDialog::State::JOB_BILL_LIST: connect(printBtn, &QPushButton::clicked, this, &DataTableDialog::printJobListBill); break;
    }

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::close);
}


void DataTableDialog::printSickEvidence(){
    if(CompanyWidget::isExists()){
        PrintDocuments document;
        document.loadDocuments();
        document.printSickLeaveEvidence(date, records);

        QMessageBox::information(this, "Sukces!", "Pomyślnie zapisano Ewidencję zowlnień chorobowych do pliku.");
    }
    else{
        QMessageBox::information(this, "Informacja", "Brak danych firmy.");
    }
}


void DataTableDialog::printFreeEvidence(){
    if(CompanyWidget::isExists()){
        PrintDocuments document;
        document.loadDocuments();
        document.printFreeLeaveEvidence(date, records);

        QMessageBox::information(this, "Sukces!", "Pomyślnie zapisano Ewidencję urlopów do pliku.");
    }
    else{
        QMessageBox::information(this, "Informacja", "Brak danych firmy.");
    }
}


void DataTableDialog::printJobTimeEvidence(){
    if(CompanyWidget::isExists()){
        PrintDocuments document;
        document.loadDocuments();

        for(int i = 0; i < table->rowCount(); ++i)
            document.printJobTimeEvidence(date, jobEvidenceRecords.at(i));

        QMessageBox::information(this, "Sukces!", "Pomyślnie zapisano Ewidencję Czasu Pracy do pliku.");
    }
    else{
        QMessageBox::information(this, "Informacja", "Brak danych firmy.");
    }
}


void DataTableDialog::printJobListBill(){
    if(CompanyWidget::isExists()){
        PrintDocuments document;
        document.loadDocuments();
        document.printJobListBill(billRecords);
        document.printJobBill(billRecords);

        QMessageBox::information(this, "Sukces!", "Pomyślnie zapisano Listę płac do pliku.");
    }
    else{
        QMessageBox::information(this, "Informacja", "Brak danych firmy.");
    }
}
