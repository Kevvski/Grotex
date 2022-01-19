#include "addjobwidget.h"

AddJobWidget::AddJobWidget(const AppState::ContractState& state, const QString& employeeID,  const QString& docID, bool changeData, QWidget *parent) : QWidget(parent)
{
    contractState = state;
    switch(contractState){
        case AppState::ContractState::ADD_JOB: initializeJobUI(employeeID, docID, changeData); break;
        case AppState::ContractState::ADD_ORDER: initializeOrderUI(employeeID, docID, changeData); break;
        case AppState::ContractState::ADD_WORK: initializeWorkUI(employeeID, docID, changeData); connectUI(); break;
    }

    addToLayout();
}


void AddJobWidget::initializeJobUI(const QString& employeeID, const QString& docID, bool changeData){
    for(int i = 0; i < 9; ++i)
        labels.push_back(new QLabel);

    for(int i = 0; i < 7; ++i)
        lineEdits.push_back(new QLineEdit);

    for(int i = 0; i < labels.size(); ++i){
        labels.at(i)->setStyleSheet(Data::boldLabel);
    }

    for(int i = 0; i < lineEdits.size(); ++i){
        lineEdits.at(i)->setStyleSheet(Data::defaultLineEdit);
        lineEdits.at(i)->setFixedWidth(350);
    }

    labels.at(0)->setText("Rodzaj pracy:");
    labels.at(1)->setText("Miejsce pracy:");
    labels.at(2)->setText("Wynagrodzenie:");
    labels.at(3)->setText("Stawka:");
    labels.at(4)->setText("Wymiar pracy:");
    labels.at(5)->setText("Ilość godzin:");
    labels.at(6)->setText("Stanowisko:");
    labels.at(7)->setText("Urlop w roku(dni):");
    labels.at(8)->setText("Termin zakończenia:");

    QDate now;
    dateEdit = new QDateEdit;
    dateEdit->setDate(now.currentDate());
    dateEdit->setStyleSheet(Data::defaultDateEdit);
    dateEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);

    decision = new QCheckBox("Posiada orzeczenie o niepełnosprawności");
    decision->setStyleSheet("QCheckBox { font-size: 15px; color: #1c45a6; font-weight: bold; margin-top: 10px; }");

    maxDaysBox = new QComboBox;
    maxDaysBox->setFixedWidth(350);
    maxDaysBox->setStyleSheet("QComboBox{ font-size: 15px; border: 2px solid #1c45a6; border-radius: 8px; padding: 5px; }");
    maxDaysBox->addItem("20");
    maxDaysBox->addItem("26");
    maxDaysBox->addItem("30");

    if(changeData){
        QSqlQuery query;
        query.exec("SELECT jobCategory, jobPlace, reward, payForHour, jobDimension, hoursCount, jobEndDate, decision, maxFreeDays, job FROM jobContracts WHERE docId = " + docID);

        if(query.next()){
            for(int i = 0; i < lineEdits.size(); ++i)
                lineEdits.at(i)->setText(query.value(i).toString());

            lineEdits.at(6)->setText(query.value(9).toString());

            dateEdit->setDate(query.value(6).toDate());
            query.value(7).toBool() ? decision->setChecked(true) : decision->setChecked(false);

            for(int i = 0; i < maxDaysBox->count(); ++i){
                if(query.value(8).toString() == maxDaysBox->itemText(i))
                    maxDaysBox->setCurrentIndex(i);
            }
        }
    }

    if(CompanyWidget::isExists()){
        QString companyData;
        QSqlQuery query;
        query.exec("SELECT name, city, cityCode, address FROM mainCompany WHERE id = 1");

        if(query.next()){
            for(int i = 0; i < query.record().count(); ++i){
                companyData += query.value(i).toString() + " ";
                if(i == 2) companyData += "Ul.";
            }
            lineEdits.at(1)->setText(companyData);
        }
    }
}


void AddJobWidget::initializeOrderUI(const QString& employeeID, const QString& docID, bool changeData){
    for(int i = 0; i < 6; ++i){
        labels.push_back(new QLabel);
        if(i > 0) lineEdits.push_back(new QLineEdit);
    }

    for(auto& label : labels)
        label->setStyleSheet(Data::boldLabel);

    for(auto& lineEdit : lineEdits){
        lineEdit->setStyleSheet(Data::defaultLineEdit);
        lineEdit->setFixedWidth(350);
    }

    QDate now;
    dateEdit = new QDateEdit(now.currentDate());
    dateEdit->setStyleSheet("QDateEdit{ font-size: 15px; border: 2px solid #1c45a6; border-radius: 8px; padding: 5px; }");
    dateEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);

    labels.at(0)->setText("Wynagrodzenie:");
    labels.at(1)->setText("Ilość godzin:");
    labels.at(2)->setText("Stawka:");
    labels.at(3)->setText("Wymiar pracy:");
    labels.at(4)->setText("Stanowisko:");
    labels.at(5)->setText("Termin zakończenia:");

    if(changeData){
        QSqlQuery query;
        query.exec("SELECT reward, hoursCount, payForHour, jobDimension, jobEndDate, job FROM orderContracts WHERE docId = " + docID);

        if(query.next()){
            lineEdits.at(0)->setText(query.value(0).toString());
            lineEdits.at(1)->setText(query.value(1).toString());
            lineEdits.at(2)->setText(query.value(2).toString());
            lineEdits.at(3)->setText(query.value(3).toString());
            lineEdits.at(4)->setText(query.value(5).toString());
            dateEdit->setDate(query.value(4).toDate());
        }
    }
}


void AddJobWidget::initializeWorkUI(const QString &employeeID, const QString& docID, bool changeData){
    for(int i = 0; i < 5; ++i){
        labels.push_back(new QLabel);
    }

    for(int i = 0; i < 4; ++i){
        lineEdits.push_back(new QLineEdit);
    }

    for(auto& label : labels)
        label->setStyleSheet(Data::boldLabel);

    for(auto& lineEdit : lineEdits){
        lineEdit->setStyleSheet(Data::defaultLineEdit);
        lineEdit->setFixedWidth(250);
    }

    labels.at(0)->setText("Wynagrodzenie:");
    labels.at(1)->setText("Wymiar pracy:");
    labels.at(2)->setText("Termin zakończenia:");
    labels.at(3)->setText("Prace do wykonania:");
    labels.at(4)->setText("Powierza asortyment:");

    lineEdits.at(1)->setText("Brak");

    QDate now;
    dateEdit = new QDateEdit(now.currentDate());
    dateEdit->setStyleSheet("QDateEdit{ font-size: 15px; border: 2px solid #1c45a6; border-radius: 8px; padding: 5px; }");
    dateEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);

    addJobBtn = new QPushButton("Dodaj");
    addJobBtn->setStyleSheet(Data::defaultButtonSheet);
    addJobBtn->setFixedSize(Data::defaultButtonSize);

    addEntrustsBtn = new QPushButton("Dodaj");
    addEntrustsBtn->setStyleSheet(Data::defaultButtonSheet);
    addEntrustsBtn->setFixedSize(Data::defaultButtonSize);

    const QSize listSize { 550, 300 };
    jobsList = new QListWidget;
    jobsList->setFixedSize(listSize);

    entrustsList = new QListWidget;
    entrustsList->setFixedSize(listSize);

    if(changeData){
        QSqlQuery query;
        query.exec("SELECT reward, jobs, entrusts, jobDimension, jobEndDate FROM workContracts WHERE docId = " + docID);

        if(query.next()){
            lineEdits.at(0)->setText(query.value(0).toString());
            lineEdits.at(1)->setText(query.value(3).toString());
            dateEdit->setDate(query.value(4).toDate());

            const QString jobsData = query.value(1).toString();
            const QString entrustsData = query.value(2).toString();
            QStringList jobList = jobsData.split(",");
            QStringList entrustList = entrustsData.split(",");

            for(int i = 0; i < jobList.size(); ++i){
                jobsList->addItem(jobList.at(i));
            }

            for(int i = 0; i < entrustList.size(); ++i){
                entrustsList->addItem(entrustList.at(i));
            }
        }
    }
}


void AddJobWidget::addToLayout(){
    QGridLayout* layout = new QGridLayout;

    if(contractState != AppState::ContractState::ADD_WORK){
        for(int i = 1; i < labels.size() + 1; ++i){
            layout->addWidget(labels.at(i - 1), i, 0);
        }

        for(int i = 1; i < lineEdits.size() + 1; ++i){
            layout->addWidget(lineEdits.at(i - 1), i, 1);
        }

        if(contractState == AppState::ContractState::ADD_JOB){
            layout->addWidget(maxDaysBox, lineEdits.size() + 1, 1);
            layout->addWidget(decision, 10, 0);
            layout->addWidget(dateEdit, lineEdits.size() + 2, 1);
        }
        else
            layout->addWidget(dateEdit, lineEdits.size() + 1, 1);

        setLayout(layout);
    }
    else{
        QVBoxLayout* mainLayout = new QVBoxLayout;
        QVBoxLayout* vLayout1 = new QVBoxLayout;
        QHBoxLayout* hLayout1 = new QHBoxLayout;
        QVBoxLayout* vLayout2 = new QVBoxLayout;
        QHBoxLayout* hLayout2 = new QHBoxLayout;

        layout->addWidget(labels.at(0), 0, 0);
        layout->addWidget(labels.at(1), 1, 0);
        layout->addWidget(labels.at(2), 2, 0);

        layout->addWidget(lineEdits.at(0), 0, 1);
        layout->addWidget(lineEdits.at(1), 1, 1);
        layout->addWidget(dateEdit, 2, 1);

        mainLayout->addLayout(layout);

        hLayout1->addWidget(labels.at(3));
        hLayout1->addWidget(lineEdits.at(2));
        hLayout1->addWidget(addJobBtn);

        vLayout1->addLayout(hLayout1);
        vLayout1->addWidget(jobsList);

        mainLayout->addLayout(vLayout1);
        mainLayout->addSpacing(20);

        hLayout2->addWidget(labels.at(4));
        hLayout2->addWidget(lineEdits.at(3));
        hLayout2->addWidget(addEntrustsBtn);

        vLayout2->addLayout(hLayout2);
        vLayout2->addWidget(entrustsList);

        mainLayout->addLayout(vLayout2);

        setLayout(mainLayout);
    }
}


void AddJobWidget::connectUI(){
    connect(addJobBtn, &QPushButton::clicked, this, &AddJobWidget::addToJobList);
    connect(addEntrustsBtn, &QPushButton::clicked, this, &AddJobWidget::addToEntrustsList);

    connect(jobsList, &QListWidget::itemDoubleClicked, this, [this](){
        delete jobsList->takeItem(jobsList->currentRow());
    });

    connect(entrustsList, &QListWidget::itemDoubleClicked, this, [this](){
        delete entrustsList->takeItem(entrustsList->currentRow());
    });
}


const QString AddJobWidget::getLineEditText(int index) const{
    if(index >= 0 && index < lineEdits.size())
        return lineEdits.at(index)->text();
}


const QString AddJobWidget::getJobsText() const{
    QString result;
    for(int i = 0; i < jobsList->count(); ++i){
        if(i != jobsList->count() - 1)
            result += jobsList->item(i)->text() + ",";
        else
            result += jobsList->item(i)->text();
    }

    return result;
}


const QString AddJobWidget::getEntrustsText() const{
    QString result;
    for(int i = 0; i < entrustsList->count(); ++i){
        if(i != entrustsList->count() - 1)
            result += entrustsList->item(i)->text() + ",";
        else
            result += entrustsList->item(i)->text();
    }

    return result;
}


const QDateEdit* AddJobWidget::getDateEdit(){
    return dateEdit;
}


const QComboBox* AddJobWidget::getMaxDays() const{
    return maxDaysBox;
}


const bool AddJobWidget::hasDecision() const{
    return decision->isChecked();
}


const bool AddJobWidget::isEmptyLineEdits() const{
    const int emptyCount = std::count_if(lineEdits.begin(), lineEdits.end(), [](QLineEdit* lineEdit){
        return lineEdit->text().isEmpty();
    });

    return emptyCount == 0 ? false : true;
}


const bool AddJobWidget::isEmptyLists() const{
    return jobsList->count() == 0 && entrustsList->count() == 0;
}


void AddJobWidget::addToJobList(){
    if(!lineEdits.at(2)->text().isEmpty()){
        QListWidgetItem* item = new QListWidgetItem;
        item->setText(lineEdits.at(2)->text());
        jobsList->addItem(item);
        lineEdits.at(2)->clear();
    }
}


void AddJobWidget::addToEntrustsList(){
    if(!lineEdits.at(3)->text().isEmpty()){
        QListWidgetItem* item = new QListWidgetItem;
        item->setText(lineEdits.at(3)->text());
        entrustsList->addItem(item);
        lineEdits.at(3)->clear();
    }
}
