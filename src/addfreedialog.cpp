#include "addfreedialog.h"

AddFreeDialog::AddFreeDialog(const AddFreeWidget::State& state, const QString& employeeID, const QString& sickId, bool changeData)
{
    setStyleSheet("background-color: white;");
    this->state = state;
    this->employeeID = employeeID;
    this->sickId = sickId;
    initializeUI(changeData);
    addTolayout();
    connectUI(changeData);

    switch (state) {
        case AddFreeWidget::State::SICK_LEAVE: setWindowTitle("KadriX - Nowe Zwolnienie Chorobowe"); break;
        case AddFreeWidget::State::FREE_LEAVE: setWindowTitle("KadriX - Dodaj Urlop"); break;
    }
}


void AddFreeDialog::initializeUI(bool changeData){
    empDataWidget = new EmployeeDataWidget(employeeID);
    freeDataWidget = new AddFreeWidget(state, sickId, changeData);

    saveBtn = new QPushButton("Zapisz");
    saveBtn->setStyleSheet(Data::defaultButtonSheet);
    saveBtn->setFixedSize(Data::defaultButtonSize);

    cancelBtn = new QPushButton("Anuluj");
    cancelBtn->setStyleSheet(Data::defaultButtonSheet);
    cancelBtn->setFixedSize(Data::defaultButtonSize);
}


void AddFreeDialog::addTolayout(){
    QVBoxLayout* layout = new QVBoxLayout;
    QHBoxLayout* btnLayout = new QHBoxLayout;

    layout->addWidget(empDataWidget);
    layout->addWidget(freeDataWidget);

    btnLayout->setAlignment(Qt::AlignRight);
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(cancelBtn);

    layout->addLayout(btnLayout);

    setLayout(layout);
}


void AddFreeDialog::connectUI(bool changeData){
    switch(state){
        case AddFreeWidget::State::SICK_LEAVE:
            if(!changeData)
                connect(saveBtn, &QPushButton::clicked, this, &AddFreeDialog::addSickLeave);
            else
                connect(saveBtn, &QPushButton::clicked, this, &AddFreeDialog::changeSickLeave);
            break;

        case AddFreeWidget::State::FREE_LEAVE:
            if(!changeData)
                connect(saveBtn, &QPushButton::clicked, this, &AddFreeDialog::addFreeLeave);
            else
                connect(saveBtn, &QPushButton::clicked, this, &AddFreeDialog::changeFreeLeave);
            break;
    }

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::close);
}


void AddFreeDialog::addSickLeave(){
    qint64 freeDaysCount = freeDataWidget->getEndDate()->date().toJulianDay() - freeDataWidget->getStartDate()->date().toJulianDay();

    if(freeDaysCount > 0){
        QString tableVariables = "(employeeId, leaveStartDate, leaveEndDate, daysCount)";
        QString bindVariables = "(:employeeId, :leaveStartDate, :leaveEndDate, :daysCount)";

        QSqlQuery query;
        query.prepare("INSERT INTO sickLeaves " + tableVariables + " VALUES " + bindVariables);
        query.bindValue(":employeeId", employeeID);
        query.bindValue(":leaveStartDate", freeDataWidget->getStartDate()->date());
        query.bindValue(":leaveEndDate", freeDataWidget->getEndDate()->date());
        query.bindValue(":daysCount", freeDaysCount);
        query.exec();

        close();
        QMessageBox::information(this, "Sukces!", "Pomyślnie dodano zwolnienie chorobowe.");
    }
    else{
        QMessageBox::warning(this, "Błąd", "Różnica dni musi być większa od zera.");
    }
}


void AddFreeDialog::addFreeLeave(){
    qint64 freeDaysCount = freeDataWidget->getEndDate()->date().toJulianDay() - freeDataWidget->getStartDate()->date().toJulianDay();
    QSqlQuery query;
    int days{0};
    bool hasUOP{false};

    if(freeDaysCount > 0){
        query.exec("SELECT freeDaysCount, uop FROM employees WHERE id = " + employeeID);
        if(query.next()){
            days = query.value(0).toInt();
        }

        query.exec("SELECT docId FROM jobContracts WHERE active = true AND employeeId = " + employeeID);
        if(query.next()){
            hasUOP = true;
        }

        if(hasUOP){
            if(freeDaysCount <= days){
                QString tableVariables = "(employeeId, leaveStartDate, leaveEndDate, daysCount, freeType)";
                QString bindVariables = "(:employeeId, :leaveStartDate, :leaveEndDate, :daysCount, :freeType)";

                query.prepare("INSERT INTO freeLeaves " + tableVariables + " VALUES " + bindVariables);
                query.bindValue(":employeeId", employeeID);
                query.bindValue(":leaveStartDate", freeDataWidget->getStartDate()->date());
                query.bindValue(":leaveEndDate", freeDataWidget->getEndDate()->date());
                query.bindValue(":daysCount", freeDaysCount);
                query.bindValue(":freeType", freeDataWidget->getFreeComboBox()->currentText());
                query.exec();

                close();
                QMessageBox::information(this, "Sukces!", "Pomyślnie dodano urlop.");
            }
            else{
                QMessageBox::information(this, "Ups...", "Pracownik nie posiada wystarczającej ilości dni do wybrania.");
            }
        }
        else{
            QMessageBox::information(this, "Ups...", "Pracownik nie posiada ważnej umowy o pracę.");
        }
    }
    else{
        QMessageBox::warning(this, "Błąd", "Różnica dni musi być większa od zera.");
    }
}


void AddFreeDialog::changeSickLeave(){
    qint64 freeDaysCount = freeDataWidget->getEndDate()->date().toJulianDay() - freeDataWidget->getStartDate()->date().toJulianDay();

    if(freeDaysCount > 0){
        QSqlQuery query;
        query.prepare("UPDATE sickLeaves SET leaveStartDate = :start, leaveEndDate = :end, daysCount = :count WHERE id = " + sickId);
        query.bindValue(":start", freeDataWidget->getStartDate()->date());
        query.bindValue(":end", freeDataWidget->getEndDate()->date());
        query.bindValue(":count", freeDaysCount);
        query.exec();

        close();
        QMessageBox::information(this, "Sukces!", "Pomyślnie zaktualizowano zwolnienie chorobowe.");
    }
    else{
        QMessageBox::warning(this, "Błąd", "Różnica dni musi być większa od zera.");
    }
}


void AddFreeDialog::changeFreeLeave(){
    qint64 freeDaysCount = freeDataWidget->getEndDate()->date().toJulianDay() - freeDataWidget->getStartDate()->date().toJulianDay();
    QSqlQuery query;
    int days{0};

    if(freeDaysCount > 0){
        query.exec("SELECT freeDaysCount FROM employees WHERE id = " + employeeID);
        if(query.next()){
            days = query.value(0).toInt();
        }

        if(freeDaysCount <= days){
            QSqlQuery query;
            query.prepare("UPDATE freeLeaves SET leaveStartDate = :start, leaveEndDate = :end, daysCount = :count, freeType = :type WHERE id = " + sickId);
            query.bindValue(":start", freeDataWidget->getStartDate()->date());
            query.bindValue(":end", freeDataWidget->getEndDate()->date());
            query.bindValue(":count", freeDaysCount);
            query.bindValue(":type", freeDataWidget->getFreeComboBox()->currentText());
            query.exec();

            close();
            QMessageBox::information(this, "Sukces!", "Pomyślnie dodano urlop.");
        }
        else{
            QMessageBox::information(this, "Ups...", "Pracownik nie posiada wystarczającej iloścni dni do wybrania.");
        }
    }
    else{
        QMessageBox::warning(this, "Błąd", "Różnica dni musi być większa od zera.");
    }
}
