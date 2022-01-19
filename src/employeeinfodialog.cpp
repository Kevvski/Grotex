#include "employeeinfodialog.h"

EmployeeInfoDialog::EmployeeInfoDialog(const QString& id)
{
    setWindowTitle("Dane personalne pracownika");
    setStyleSheet("background-color: white");
    employeeId = id;
    initializeUI();
    addToLayout();
    connectUI();
    resize(500, 600);
}


void EmployeeInfoDialog::initializeUI(){
    QSqlQuery query;
    QVector<Data::EmployeeInfoRecord> info;

    scroll = new QScrollArea;
    scroll->setWidgetResizable(true);

    empWidget = new EmployeeDataWidget(employeeId);

    infoHeader = new QLabel("Informacje osobiste:");
    infoHeader->setStyleSheet(Data::boldLabel);

    jobHeader = new QLabel("Przebieg zatrudnienia:");
    jobHeader->setStyleSheet(Data::boldLabel);

    freeHeader = new QLabel("Ewidencja urlopowa:");
    freeHeader->setStyleSheet(Data::boldLabel);

    sickHeader = new QLabel("Ewidencja chorobowa:");
    sickHeader->setStyleSheet(Data::boldLabel);

    freeDaysCountLabel = new QLabel;
    freeDaysCountLabel->setStyleSheet(Data::boldLabel);
    query.exec("SELECT freeDaysCount FROM employees WHERE id = " + employeeId);
    if(query.next())
        freeDaysCountLabel->setText("Dni urlopowe(Pozostało): " + query.value(0).toString());

    freeSumLabel = new QLabel;
    freeSumLabel->setStyleSheet(Data::boldLabel);

    sickSumLabel = new QLabel;
    sickSumLabel->setStyleSheet(Data::boldLabel);

    jobStoryTable = new QTableWidget;
    jobStoryTable->setColumnCount(4);
    jobStoryTable->setFixedHeight(200);
    jobStoryTable->setHorizontalHeaderLabels(QStringList{ "Rodzaj umowy", "Data zatrudnienia", "Data zwolnienia", "Stanowisko" });
    query.exec("SELECT jobStartDate, jobEndDate, job FROM jobContracts WHERE employeeId = " + employeeId);
    while(query.next()){
        Data::EmployeeInfoRecord record;
        record.type = "Umowa o pracę";
        record.startDate = query.value(0).toString();
        record.endDate = query.value(1).toString();
        record.jobType = query.value(2).toString();
        info.push_back(record);
    }
    query.exec("SELECT jobStartDate, jobEndDate, job FROM orderContracts WHERE employeeId = " + employeeId);
    while(query.next()){
        Data::EmployeeInfoRecord record;
        record.type = "Umowa zlecenie";
        record.startDate = query.value(0).toString();
        record.endDate = query.value(1).toString();
        record.jobType = query.value(2).toString();
        info.push_back(record);
    }
    query.exec("SELECT jobStartDate, jobEndDate FROM workContracts WHERE employeeId = " + employeeId);
    while(query.next()){
        Data::EmployeeInfoRecord record;
        record.type = "Umowa o dzieło";
        record.startDate = query.value(0).toString();
        record.endDate = query.value(1).toString();
        record.jobType = query.value(2).toString();
        info.push_back(record);
    }
    jobStoryTable->setRowCount(info.size());

    for(int i = 0; i < info.size(); ++i){
        jobStoryTable->setItem(i, 0, new QTableWidgetItem(info.at(i).type));
        jobStoryTable->setItem(i, 1, new QTableWidgetItem(info.at(i).startDate));
        jobStoryTable->setItem(i, 2, new QTableWidgetItem(info.at(i).endDate));
        jobStoryTable->setItem(i, 3, new QTableWidgetItem(info.at(i).jobType));
    }
    info.clear();

    int sum{0};
    freeStoryTable = new QTableWidget;
    freeStoryTable->setColumnCount(4);
    freeStoryTable->setFixedHeight(200);
    freeStoryTable->setHorizontalHeaderLabels(QStringList{ "Rodzaj urlopu", "Data wzięcia", "Data zakończenia", "Ilość dni" });
    query.exec("SELECT freeType, leaveStartDate, leaveEndDate, daysCount FROM freeLeaves WHERE employeeId = " + employeeId);
    while(query.next()){
        Data::EmployeeInfoRecord record;
        record.type = query.value(0).toString();
        record.startDate = query.value(1).toString();
        record.endDate = query.value(2).toString();
        record.daysCount = query.value(3).toString();
        info.push_back(record);
    }
    freeStoryTable->setRowCount(info.size());

    for(int i = 0; i < info.size(); ++i){
        freeStoryTable->setItem(i, 0, new QTableWidgetItem(info.at(i).type));
        freeStoryTable->setItem(i, 1, new QTableWidgetItem(info.at(i).startDate));
        freeStoryTable->setItem(i, 2, new QTableWidgetItem(info.at(i).endDate));
        freeStoryTable->setItem(i, 3, new QTableWidgetItem(info.at(i).daysCount));
        sum += info.at(i).daysCount.toInt();
    }
    freeSumLabel->setText("Razem: " + QString::number(sum));
    sum = 0;
    info.clear();

    sickStoryTable = new QTableWidget;
    sickStoryTable->setColumnCount(3);
    sickStoryTable->setFixedHeight(200);
    sickStoryTable->setHorizontalHeaderLabels(QStringList{ "Data wzięcia", "Data zakończenia", "Ilość dni" });
    query.exec("SELECT leaveStartDate, leaveEndDate, daysCount FROM sickLeaves WHERE employeeId = " + employeeId);
    while(query.next()){
        Data::EmployeeInfoRecord record;
        record.startDate = query.value(0).toString();
        record.endDate = query.value(1).toString();
        record.daysCount = query.value(2).toString();
        info.push_back(record);
    }
    sickStoryTable->setRowCount(info.size());

    for(int i = 0; i < info.size(); ++i){
        sickStoryTable->setItem(i, 0, new QTableWidgetItem(info.at(i).startDate));
        sickStoryTable->setItem(i, 1, new QTableWidgetItem(info.at(i).endDate));
        sickStoryTable->setItem(i, 2, new QTableWidgetItem(info.at(i).daysCount));
        sum += info.at(i).daysCount.toInt();
    }
    sickSumLabel->setText("Razem: " + QString::number(sum));
    sum = 0;
    info.clear();

    printBtn = new QPushButton("Drukuj");
    printBtn->setFixedSize(Data::defaultButtonSize);
    printBtn->setStyleSheet(Data::defaultButtonSheet);

    cancelBtn = new QPushButton("Wróć");
    cancelBtn->setFixedSize(Data::defaultButtonSize);
    cancelBtn->setStyleSheet(Data::defaultButtonSheet);
}


void EmployeeInfoDialog::addToLayout(){
    QWidget* mainWidget = new QWidget;
    QHBoxLayout* btnLayout = new QHBoxLayout;
    QVBoxLayout* layout = new QVBoxLayout;
    QVBoxLayout* lastLayout = new QVBoxLayout;

    btnLayout->setAlignment(Qt::AlignRight);
    btnLayout->addWidget(printBtn);
    btnLayout->addWidget(cancelBtn);

    layout->addWidget(infoHeader);
    layout->addWidget(empWidget);
    layout->addWidget(freeDaysCountLabel);

    layout->addSpacing(40);
    layout->addWidget(jobHeader);
    layout->addWidget(jobStoryTable);

    layout->addSpacing(40);
    layout->addWidget(freeHeader);
    layout->addWidget(freeStoryTable);
    layout->addWidget(freeSumLabel);

    layout->addSpacing(40);
    layout->addWidget(sickHeader);
    layout->addWidget(sickStoryTable);
    layout->addWidget(sickSumLabel);

    mainWidget->setLayout(layout);
    scroll->setWidget(mainWidget);
    lastLayout->addWidget(scroll);
    lastLayout->addLayout(btnLayout);
    setLayout(lastLayout);
}


void EmployeeInfoDialog::connectUI(){
    connect(printBtn, &QPushButton::clicked, this, &EmployeeInfoDialog::printInfo);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::close);
}


void EmployeeInfoDialog::printInfo(){
    QVector<Data::EmployeeInfoRecord> jobRecords;
    QVector<Data::EmployeeInfoRecord> freeRecords;
    QVector<Data::EmployeeInfoRecord> sickRecords;
    QString freeDays = freeDaysCountLabel->text().at(freeDaysCountLabel->text().lastIndexOf(" ") + 1);

    if(jobStoryTable->rowCount() != 0){
        for(int i = 0 ; i < jobStoryTable->rowCount(); ++i){
            Data::EmployeeInfoRecord record;
            record.jobType = jobStoryTable->item(i, 0)->text();
            record.startDate = jobStoryTable->item(i, 1)->text();
            record.endDate = jobStoryTable->item(i, 2)->text();
            record.type = jobStoryTable->item(i, 3)->text();
            jobRecords.push_back(record);
        }
    }

    if(freeStoryTable->rowCount() != 0){
        for(int i = 0 ; i < freeStoryTable->rowCount(); ++i){
            Data::EmployeeInfoRecord record;
            record.startDate = freeStoryTable->item(i, 0)->text();
            record.endDate = freeStoryTable->item(i, 1)->text();
            record.type = freeStoryTable->item(i, 2)->text();
            record.daysCount = freeStoryTable->item(i, 3)->text();
            freeRecords.push_back(record);
        }
    }

    if(sickStoryTable->rowCount() != 0){
        for(int i = 0 ; i < sickStoryTable->rowCount(); ++i){
            Data::EmployeeInfoRecord record;
            record.startDate = sickStoryTable->item(i, 0)->text();
            record.endDate = sickStoryTable->item(i, 1)->text();
            record.daysCount = sickStoryTable->item(i, 2)->text();
            sickRecords.push_back(record);
        }
    }

    PrintDocuments document;
    document.setEmployeeID(employeeId);
    document.loadDocuments();
    document.printEmployeeInfo(jobRecords, freeRecords, sickRecords, freeDays);
    QMessageBox::information(this, "Sukces!", "Pomyślnie zapisano do pliku historię pracownika.");
}
