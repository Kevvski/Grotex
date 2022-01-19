#include "addjobdialog.h"

AddJobDialog::AddJobDialog(const QString& employeeID, const QString& docId, const AppState::ContractState& state, bool changeData)
{
    this->employeeID = employeeID;
    this->docID = docId;
    contractState = state;
    setStyleSheet("background-color: white");
    initializeUI(changeData);
    addToLayout();
    connectUI(changeData);

    switch(contractState){
        case AppState::ContractState::ADD_JOB: setWindowTitle("KadriX - Nowa Umowa o Pracę"); break;
        case AppState::ContractState::ADD_ORDER: setWindowTitle("KadriX - Nowa Umowa Zlecenie"); break;
        case AppState::ContractState::ADD_WORK: setWindowTitle("KadriX - Nowa Umowa o Dzieło"); break;
    }
}


void AddJobDialog::initializeUI(bool changeData){
    if(contractState != AppState::ContractState::ADD_WORK)
        empDataWidget = new EmployeeDataWidget(employeeID);
    else
        empDataWidget = new EmployeeDataWidget(employeeID);

    if(changeData)
        addJobWidget = new AddJobWidget(contractState, employeeID, docID, true);
    else
        addJobWidget = new AddJobWidget(contractState);

    saveBtn = new QPushButton("Zapisz");
    saveBtn->setStyleSheet(Data::defaultButtonSheet);
    saveBtn->setFixedSize(Data::defaultButtonSize);

    cancelBtn = new QPushButton("Anuluj");
    cancelBtn->setStyleSheet(Data::defaultButtonSheet);
    cancelBtn->setFixedSize(Data::defaultButtonSize);
}


void AddJobDialog::addToLayout(){
    QVBoxLayout* layout = new QVBoxLayout;
    QHBoxLayout* otherLayout = new QHBoxLayout;

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->setAlignment(Qt::AlignRight);
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(cancelBtn);

    if(contractState != AppState::ContractState::ADD_WORK){
        layout->addWidget(empDataWidget);
        layout->addWidget(addJobWidget);
        layout->addLayout(btnLayout);
    }
    else{
        otherLayout->addWidget(empDataWidget);
        otherLayout->addSpacing(20);
        otherLayout->addWidget(addJobWidget);

        layout->addLayout(otherLayout);
        layout->addLayout(btnLayout);
    }

    setLayout(layout);
}


void AddJobDialog::connectUI(bool changeData){
    switch(contractState){
        case AppState::ContractState::ADD_JOB:
            if(changeData)
                connect(saveBtn, &QPushButton::clicked, this, &AddJobDialog::editJobContract);
            else
                connect(saveBtn, &QPushButton::clicked, this, &AddJobDialog::saveJobContract);
            break;

        case AppState::ContractState::ADD_ORDER:
            if(changeData)
                connect(saveBtn, &QPushButton::clicked, this, &AddJobDialog::editOrderContract);
            else
                connect(saveBtn, &QPushButton::clicked, this, &AddJobDialog::saveOrderContract);
            break;

        case AppState::ContractState::ADD_WORK:
            if(changeData)
                connect(saveBtn, &QPushButton::clicked, this, &AddJobDialog::editWorkContract);
            else
                connect(saveBtn, &QPushButton::clicked, this, &AddJobDialog::saveWorkContract);
            break;
    }

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::close);
}



//Connecting Slots--------------------------------------------------
void AddJobDialog::saveJobContract(){
    if(!addJobWidget->isEmptyLineEdits()){
        QString tableVariables = "(employeeId, id, jobCategory, jobPlace, reward, payForHour, jobDimension, hoursCount, jobStartDate, jobEndDate, decision, decisionText, job, maxFreeDays, numberId, active)";
        QString bindVariables = "(:employeeId, :id, :jobCategory, :jobPlace, :reward, :payForHour, :jobDimension, :hoursCount, :jobStartDate, :jobEndDate, :decision, :decisionText, :job, :maxFreeDays, :numberId, :active)";
        QDate now;
        int contractID{};
        bool hasUOP { false };

        QSqlQuery query;
        query.exec("SELECT uop FROM employees WHERE id = " + employeeID);
        if(query.next())
            hasUOP = query.value(0).toBool();

        if(!hasUOP){
            if(now.currentDate() != addJobWidget->getDateEdit()->date()){
                query.exec("SELECT numberId, jobStartDate FROM jobContracts");

                if(query.isSelect() && query.last()){
                    if(query.value(1).toDate().month() == now.currentDate().month())
                        contractID = query.value(0).toInt() + 1;
                    else
                        contractID = 1;
                }
                else{
                    contractID = 1;
                }

                query.exec("UPDATE employees SET activeWork = true, isWorking = 'Tak', uop = true WHERE id = " + employeeID);

                query.prepare("INSERT INTO jobContracts " + tableVariables + " VALUES " + bindVariables);
                query.bindValue(":employeeId", employeeID);
                query.bindValue(":id", "UP/" + QString::number(contractID) + "/" + QString::number(now.currentDate().month()) + "/" + QString::number(now.currentDate().year()));
                query.bindValue(":jobCategory", addJobWidget->getLineEditText(0));
                query.bindValue(":jobPlace", addJobWidget->getLineEditText(1));
                query.bindValue(":reward", addJobWidget->getLineEditText(2));
                query.bindValue(":payForHour", addJobWidget->getLineEditText(3));
                query.bindValue(":jobDimension", addJobWidget->getLineEditText(4));
                query.bindValue(":hoursCount", addJobWidget->getLineEditText(5));
                query.bindValue(":jobStartDate", now.currentDate());
                query.bindValue(":jobEndDate", addJobWidget->getDateEdit()->date());
                query.bindValue(":decision", addJobWidget->hasDecision());
                query.bindValue(":decisionText", addJobWidget->hasDecision() ? "Tak" : "Nie");
                query.bindValue(":job", addJobWidget->getLineEditText(6));
                query.bindValue(":maxFreeDays", addJobWidget->getMaxDays()->currentText().toInt());
                query.bindValue(":numberId", contractID);
                query.bindValue(":active", true);
                query.exec();

                close();
                QMessageBox::information(this, "Sukces!", "Pomyślnie dodano nową Umowę o Pracę.");
            }
            else{
                QMessageBox::warning(this, "Błąd", "Terminy rozpoczęcia i zakończenia na umowię muszą się różnić.");
            }
        }
        else{
            QMessageBox::warning(this, "Błąd", "Wybrany pracownik posiada już Umowę o Pracę.");
        }
    }
    else{
        QMessageBox::warning(this, "Błąd", "Proszę uzupełnić wszystkie pola.");
    }
}


void AddJobDialog::saveOrderContract(){
    if(!addJobWidget->isEmptyLineEdits()){
        QString tableVariables = "(employeeId, id, reward, hoursCount, payForHour, jobDimension, jobStartDate, jobEndDate, numberId, active, job)";
        QString bindVariables = "(:employeeId, :id, :reward, :hoursCount, :payForHour, :jobDimension, :jobStartDate, :jobEndDate, :numberId, :active, :job)";
        QDate now;
        int contractID{};
        bool hasUZ { false };

        QSqlQuery query;
        query.exec("SELECT uz FROM employees WHERE id = " + employeeID);
        if(query.next())
            hasUZ = query.value(0).toBool();

        if(!hasUZ){
            if(now.currentDate() != addJobWidget->getDateEdit()->date()){
                query.exec("SELECT numberId, jobStartDate FROM orderContracts");

                if(query.isSelect() && query.last()){
                    if(query.value(1).toDate().month() == now.currentDate().month())
                        contractID = query.value(0).toInt() + 1;
                    else
                        contractID = 1;
                }
                else{
                    contractID = 1;
                }

                query.exec("UPDATE employees SET activeWork = true, isWorking = 'Tak', uz = true WHERE id = " + employeeID);

                query.prepare("INSERT INTO orderContracts " + tableVariables + " VALUES " + bindVariables);
                query.bindValue(":employeeId", employeeID);
                query.bindValue(":id", "UZ/" + QString::number(contractID) + "/" + QString::number(now.currentDate().month()) + "/" + QString::number(now.currentDate().year()));
                query.bindValue(":reward", addJobWidget->getLineEditText(0));
                query.bindValue(":hoursCount", addJobWidget->getLineEditText(1).toInt());
                query.bindValue(":payForHour", addJobWidget->getLineEditText(2).toFloat());
                query.bindValue(":jobDimension", addJobWidget->getLineEditText(3));
                query.bindValue(":jobStartDate", now.currentDate());
                query.bindValue(":jobEndDate", addJobWidget->getDateEdit()->date());
                query.bindValue(":numberId", contractID);
                query.bindValue(":active", true);
                query.bindValue(":job", addJobWidget->getLineEditText(4));
                query.exec();

                close();
                QMessageBox::information(this, "Sukces", "Pomyślnie dodano nową Umowę Zlecenie.");
            }
            else{
                QMessageBox::warning(this, "Błąd", "Terminy rozpoczęcia i zakończenia na umowię muszą się różnić.");
            }
        }
        else{
            QMessageBox::warning(this, "Błąd", "Wybrany pracownik posiada już Umowę Zlecenie.");
        }
    }
    else{
        QMessageBox::warning(this, "Błąd", "Proszę uzupełnić wszystkie pola.");
    }
}


void AddJobDialog::saveWorkContract(){
    if(!addJobWidget->getLineEditText(0).isEmpty()){
        if(!addJobWidget->isEmptyLists()){
            QString tableVariables = "(employeeId, id, reward, jobs, entrusts, jobStartDate, jobEndDate, jobDimension, numberId, active)";
            QString bindVariables = "(:employeeId, :id, :reward, :jobs, :entrusts, :jobStartDate, :jobEndDate, :jobDimension, :numberId, :active)";
            QDate now;
            int contractID{};
            bool hasUOD { false };

            QSqlQuery query;
            query.exec("SELECT uod FROM employees WHERE id = " + employeeID);
            if(query.next())
                hasUOD = query.value(0).toBool();

            if(!hasUOD){
                if(now.currentDate() != addJobWidget->getDateEdit()->date()){
                    query.exec("SELECT numberId, jobStartDate FROM workContracts");
                    if(query.isSelect() && query.last()){
                        if(query.value(1).toDate().month() == now.currentDate().month())
                            contractID = query.value(0).toInt() + 1;
                        else
                            contractID = 1;
                    }
                    else{
                        contractID = 1;
                    }

                    query.exec("UPDATE employees SET activeWork = true, isWorking = 'Tak', uod = true WHERE id = " + employeeID);

                    query.prepare("INSERT INTO workContracts " + tableVariables + " VALUES " + bindVariables);
                    query.bindValue(":employeeId", employeeID);
                    query.bindValue(":id", "UD/" + QString::number(contractID) + "/" + QString::number(now.currentDate().month()) + "/" + QString::number(now.currentDate().year()));
                    query.bindValue(":reward", addJobWidget->getLineEditText(0));
                    query.bindValue(":jobs", addJobWidget->getJobsText());
                    query.bindValue(":entrusts", addJobWidget->getEntrustsText());
                    query.bindValue(":jobStartDate", now.currentDate());
                    query.bindValue(":jobEndDate", addJobWidget->getDateEdit()->date());
                    query.bindValue(":jobDimension", addJobWidget->getLineEditText(1));
                    query.bindValue(":numberId", contractID);
                    query.bindValue(":active", true);
                    query.exec();

                    close();
                    QMessageBox::information(this, "Sukces", "Pomyślnie dodano nową Umowę o Dzieło.");
                }
                else{
                    QMessageBox::warning(this, "Błąd", "Terminy rozpoczęcia i zakończenia na umowię muszą się różnić.");
                }
            }
            else{
                QMessageBox::warning(this, "Błąd", "Wybrany pracownik posiada już Umowę o Dzieło.");
            }
        }
        else{
            QMessageBox::warning(this, "Błąd", "Proszę uzupełnić listy.");
        }
    }
    else{
        QMessageBox::warning(this, "Błąd", "Proszę uzupełnić wszystkie pola.");
    }
}


void AddJobDialog::editJobContract(){
    if(!addJobWidget->isEmptyLineEdits()){
        QSqlQuery query;
        query.prepare("UPDATE jobContracts SET jobCategory = :1, jobPlace = :2, reward = :3, payForHour = :4, jobDimension = :5, hoursCount = :6, jobEndDate = :7, maxFreeDays = :8,"
                   "decision = :9, decisionText = :10, job = :11 WHERE docId = " + docID);
        query.bindValue(":1", addJobWidget->getLineEditText(0));
        query.bindValue(":2", addJobWidget->getLineEditText(1));
        query.bindValue(":3", addJobWidget->getLineEditText(2));
        query.bindValue(":4", addJobWidget->getLineEditText(3));
        query.bindValue(":5", addJobWidget->getLineEditText(4));
        query.bindValue(":6", addJobWidget->getLineEditText(5));
        query.bindValue(":7", addJobWidget->getDateEdit()->date());
        query.bindValue(":8", addJobWidget->getMaxDays()->currentText().toInt());
        query.bindValue(":11", addJobWidget->getLineEditText(6));

        if(addJobWidget->hasDecision()){
            query.bindValue(":9", true);
            query.bindValue(":10", "Tak");
        }
        else{
            query.bindValue(":9", false);
            query.bindValue(":10", "Nie");
        }
        query.exec();

        close();
        QMessageBox::information(this, "Sukces!", "Pomyślnie zaktualizowano Umowę o Pracę.");
    }
    else{
        QMessageBox::warning(this, "Błąd", "Proszę uzupełnić wszystkie pola.");
    }
}


void AddJobDialog::editOrderContract(){
    if(!addJobWidget->isEmptyLineEdits()){
        QSqlQuery query;
        query.prepare("UPDATE orderContracts SET reward = :1, hoursCount = :2, payForHour = :3, jobDimension = :4, jobEndDate = :5, job = :6 WHERE docId = " + docID);
        query.bindValue(":1", addJobWidget->getLineEditText(0));
        query.bindValue(":2", addJobWidget->getLineEditText(1));
        query.bindValue(":3", addJobWidget->getLineEditText(2));
        query.bindValue(":4", addJobWidget->getLineEditText(3));
        query.bindValue(":5", addJobWidget->getDateEdit()->date());
        query.bindValue(":6", addJobWidget->getLineEditText(4));
        query.exec();

        close();
        QMessageBox::information(this, "Sukces!", "Pomyślnie zaktualizowano Umowę Zlecenie.");
    }
    else{
        QMessageBox::warning(this, "Błąd", "Proszę uzupełnić wszystkie pola.");
    }
}


void AddJobDialog::editWorkContract(){
    if(!addJobWidget->getLineEditText(0).isEmpty()){
        if(!addJobWidget->isEmptyLists()){
            QSqlQuery query;
            query.prepare("UPDATE workContracts SET reward = :1, jobs = :2, entrusts = :3, jobEndDate = :4, jobDimension = :5 WHERE docId = " + docID);
            query.bindValue(":1", addJobWidget->getLineEditText(0));
            query.bindValue(":2", addJobWidget->getJobsText());
            query.bindValue(":3", addJobWidget->getEntrustsText());
            query.bindValue(":4", addJobWidget->getDateEdit()->date());
            query.bindValue(":5", addJobWidget->getLineEditText(1));
            query.exec();

            close();
            QMessageBox::information(this, "Sukces!", "Pomyślnie zaktualizowano Umowę o Dzieło.");
        }
        else{
            QMessageBox::warning(this, "Błąd", "Proszę uzupełnić listy.");
        }
    }
    else{
        QMessageBox::warning(this, "Błąd", "Proszę uzupełnić wszystkie pola.");
    }
}
