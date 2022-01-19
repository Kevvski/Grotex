#include "searchsickdialog.h"
#include <cmath>

SearchSickDialog::SearchSickDialog(const SearchSickDialog::State& state, const QString& id, const QString& docId, bool changeData)
{
    setStyleSheet("background-color: white;");
    this->employeeID = id;
    this->id = docId;
    this->changeData = changeData;

    initializeUI(state);
    addToLayout(state);
    connectUI(state);

    switch(state){
    case SearchSickDialog::State::SICK_LEAVE: setWindowTitle("KadriX - Stwórz ewidencje zwolnień chorobowych"); break;
    case SearchSickDialog::State::FREE_LEAVE: setWindowTitle("KadriX - Stwórz ewidencje urlopów"); break;
    case SearchSickDialog::State::JOB_TIME_EVIDENCE: setWindowTitle("KadriX - Stwórz ewidencje czasu pracy"); break;
    case SearchSickDialog::State::SEARCH_JOB_TIME: setWindowTitle("KadriX - Stwórz miesięczną Ew. czasu pracy"); break;
    case SearchSickDialog::State::CREATE_JOB_BILL_LIST: setWindowTitle("KadriX - Stwórz miesięczną listę płac"); break;
    }
}


void SearchSickDialog::initializeUI(const SearchSickDialog::State& state){
    monthLabel = new QLabel("Miesiąc:");
    monthLabel->setStyleSheet(Data::boldLabel);

    yearLabel = new QLabel("Rok:");
    yearLabel->setStyleSheet(Data::boldLabel);

    typeLabel = new QLabel("Rodzaj umowy:");
    typeLabel->setStyleSheet(Data::boldLabel);

    monthBox = new QComboBox;
    monthBox->setStyleSheet("QComboBox{ font-size: 15px; border: 2px solid #1c45a6; border-radius: 8px; padding: 5px; }");
    monthBox->setFixedWidth(350);
    monthBox->addItem("Styczeń");
    monthBox->addItem("Luty");
    monthBox->addItem("Marzec");
    monthBox->addItem("Kwiecień");
    monthBox->addItem("Maj");
    monthBox->addItem("Czerwiec");
    monthBox->addItem("Lipiec");
    monthBox->addItem("Sierpień");
    monthBox->addItem("Wrzesień");
    monthBox->addItem("Październik");
    monthBox->addItem("Listopad");
    monthBox->addItem("Grudzień");

    typeBox = new QComboBox;
    typeBox->setStyleSheet("QComboBox{ font-size: 15px; border: 2px solid #1c45a6; border-radius: 8px; padding: 5px; }");
    typeBox->setFixedWidth(350);
    typeBox->addItem("Umowa o Pracę");
    typeBox->addItem("Umowa Zlecenie");

    empWidget = new EmployeeDataWidget(employeeID);

    QDate now;
    yearEdit = new QLineEdit(QString::number(now.currentDate().year()));
    yearEdit->setStyleSheet(Data::defaultLineEdit);
    yearEdit->setFixedWidth(350);

    table = new QTableWidget;
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels(QStringList{ "Dzień", "Ilość Godzin" });
    table->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
    table->verticalHeader()->hide();

    searchBtn = new QPushButton("Szukaj");
    searchBtn->setStyleSheet(Data::defaultButtonSheet);
    searchBtn->setFixedSize(Data::defaultButtonSize);

    cancelBtn = new QPushButton("Anuluj");
    cancelBtn->setStyleSheet(Data::defaultButtonSheet);
    cancelBtn->setFixedSize(Data::defaultButtonSize);

    if(state == SearchSickDialog::State::JOB_TIME_EVIDENCE)
        searchBtn->setText("Zapisz");

    if(changeData){
        QSqlQuery query;
        query.exec("SELECT month, year, type FROM jobTimeEvidence WHERE id = " + id);
        if(query.next()){
            monthBox->setCurrentText(query.value(0).toString());
            yearEdit->setText(query.value(1).toString());
            typeBox->setCurrentText(query.value(2).toString());
        }
    }

    if(state == SearchSickDialog::State::JOB_TIME_EVIDENCE)
        refreshTable(monthBox->currentIndex());
}


void SearchSickDialog::addToLayout(const SearchSickDialog::State& state){
    QVBoxLayout* layout = new QVBoxLayout;
    QGridLayout* gridLayout = new QGridLayout;
    QHBoxLayout* btnLayout = new QHBoxLayout;
    QHBoxLayout* evLayout = new QHBoxLayout;

    gridLayout->addWidget(monthLabel, 0, 0);
    gridLayout->addWidget(yearLabel, 1, 0);
    gridLayout->addWidget(monthBox, 0, 1);
    gridLayout->addWidget(yearEdit, 1, 1);

    btnLayout->setAlignment(Qt::AlignRight);
    btnLayout->addWidget(searchBtn);
    btnLayout->addWidget(cancelBtn);

    if(state == SearchSickDialog::State::JOB_TIME_EVIDENCE){
        gridLayout->addWidget(typeLabel, 2, 0);
        gridLayout->addWidget(typeBox, 2, 1);

        layout->addWidget(empWidget);
        layout->addLayout(gridLayout);
        layout->addLayout(btnLayout);

        evLayout->addLayout(layout);
        evLayout->addSpacing(20);
        evLayout->addWidget(table);

        setLayout(evLayout);
    }
    else{
        layout->addLayout(gridLayout);
        layout->addLayout(btnLayout);
        setLayout(layout);
    }
}


void SearchSickDialog::connectUI(const SearchSickDialog::State& state){
    switch(state){
        case SearchSickDialog::State::SICK_LEAVE:
            connect(searchBtn, &QPushButton::clicked, this, &SearchSickDialog::searchSickLeave);
            break;

        case SearchSickDialog::State::FREE_LEAVE:
            connect(searchBtn, &QPushButton::clicked, this, &SearchSickDialog::searchFreeLeave);
            break;

        case SearchSickDialog::State::JOB_TIME_EVIDENCE:
            connect(monthBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SearchSickDialog::refreshTable);
            if(changeData)
                connect(searchBtn, &QPushButton::clicked, this, &SearchSickDialog::saveJobTimeEvidence);
            else
                connect(searchBtn, &QPushButton::clicked, this, &SearchSickDialog::createJobTimeEvidence);
            break;

        case SearchSickDialog::State::SEARCH_JOB_TIME:
            connect(searchBtn, &QPushButton::clicked, this, &SearchSickDialog::searchJobTime);
            break;

        case SearchSickDialog::State::CREATE_JOB_BILL_LIST:
            connect(searchBtn, &QPushButton::clicked, this, &SearchSickDialog::searchJobBills);
            break;
    }

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::close);
}


const QString SearchSickDialog::getHourCountAsString() const{
    QString result{""};
    QComboBox* c;

    for(int i = 0; i < table->rowCount(); ++i){
        c = qobject_cast<QComboBox*>(table->cellWidget(i, 1));
        if(!c){
            if(table->item(i, 1)->text() == "sobota")
                result += "sobota,";
            else if(table->item(i, 1)->text() == "niedziela")
                result += "niedziela,";
        }
        else{
            if(i != table->rowCount() - 1)
                result += QString::number(c->currentIndex()) + ",";
            else
                result += QString::number(c->currentIndex());
        }
    }

    return result;
}


const QString SearchSickDialog::getDaysAsString() const{
    QString result{""};

    for(int i = 0; i < table->rowCount(); ++i){
        if(i != table->rowCount() - 1)
            result += table->item(i, 0)->text() + ",";
        else
            result += table->item(i, 0)->text();
    }

    return result;
}


const int SearchSickDialog::getHoursSum(int hours) const{
    int sum{0};
    QComboBox *c;

    for(int i = 0; i < table->rowCount(); ++i){
        c = qobject_cast<QComboBox*>(table->cellWidget(i, 1));
        if(!c){
            continue;
        }
        else{
            if(c->currentIndex() >= 0 && c->currentIndex() <= 6)
                sum += c->currentText().toInt();
        }
    }

    return sum;
}


const int SearchSickDialog::getDaysSum() const{
    int sum{0};
    QComboBox* c;

    for(int i = 0; i < table->rowCount(); ++i){
        c = qobject_cast<QComboBox*>(table->cellWidget(i, 1));
        if(!c){
            continue;
        }
        else{
            if(c->currentIndex() >= 0 && c->currentIndex() <= 6)
                sum++;
        }
    }

    return sum;
}


const int SearchSickDialog::getFreeDaysSum() const{
    int sum{0};
    QComboBox* c;

    for(int i = 0; i < table->rowCount(); ++i){
        c = qobject_cast<QComboBox*>(table->cellWidget(i, 1));
        if(!c){
            continue;
        }
        else{
            if(c->currentIndex() == 7)
                sum++;
        }
    }

    return sum;
}


const int SearchSickDialog::getSickDaysSum() const{
    int sum{0};
    QComboBox* c;

    for(int i = 0; i < table->rowCount(); ++i){
        c = qobject_cast<QComboBox*>(table->cellWidget(i, 1));
        if(!c){
            continue;
        }
        else{
            if(c->currentIndex() == 10)
                sum++;
        }
    }

    return sum;
}


int SearchSickDialog::searchSickLeave(){
    //const int everyIsDigit = std::count_if(yearEdit->text().begin(), yearEdit->text().end(), [](QChar& character){ return character.isDigit(); });
    if(yearEdit->text().size() > 3 && yearEdit->text().size() < 5){
        QVector<Data::EvidenceRecord> records;
        QVector<Data::EvidenceRecord> findedRecords;

        QSqlQuery query;
        query.exec("SELECT id, employeeId, daysCount, leaveStartDate, leaveEndDate FROM sickLeaves");
        while(query.next()){
            records.push_back(Data::EvidenceRecord{
                query.value(0).toInt(),
                query.value(1).toInt(),
                query.value(2).toInt(),
                "",
                "",
                "",
                "",
                query.value(3).toDate(),
                query.value(4).toDate()});
        }

        for(auto& record : records){
            if(record.startDate.month() == monthBox->currentIndex() + 1 && record.startDate.year() == yearEdit->text().toInt())
                findedRecords.push_back(record);
        }

        if(!findedRecords.empty()){
            DataTableDialog* dialog = new DataTableDialog(DataTableDialog::State::SICK_LEAVE, findedRecords, monthBox->currentText() + ", " + yearEdit->text());
            close();
            return dialog->exec();
        }
        else{
            QMessageBox::information(this, "Informacja", "Nie znaleziono zwolnień chorowbowych w wybranym miesiąciu");
        }
    }
    else{
        QMessageBox::warning(this, "Błąd", "Błędnie wprowadzony rok.");
    }

    return 0;
}


int SearchSickDialog::searchFreeLeave(){
    //const int everyIsDigit = std::count_if(yearEdit->text().begin(), yearEdit->text().end(), [](QChar& character){ return character.isDigit(); });
    if(yearEdit->text().size() > 3 && yearEdit->text().size() < 5){
        QVector<Data::EvidenceRecord> records;
        QVector<Data::EvidenceRecord> findedRecords;

        QSqlQuery query;
        query.exec("SELECT id, employeeId, daysCount, freeType, leaveStartDate, leaveEndDate FROM freeLeaves");
        while(query.next()){
            records.push_back(Data::EvidenceRecord{
                query.value(0).toInt(),
                query.value(1).toInt(),
                query.value(2).toInt(),
                "",
                "",
                "",
                query.value(3).toString(),
                query.value(4).toDate(),
                query.value(5).toDate()});
        }

        for(auto& record : records){
            if(record.startDate.month() == monthBox->currentIndex() + 1 && record.startDate.year() == yearEdit->text().toInt())
                findedRecords.push_back(record);
        }

        if(!findedRecords.empty()){
            DataTableDialog* dialog = new DataTableDialog(DataTableDialog::State::FREE_LEAVE, findedRecords, monthBox->currentText() + ", " + yearEdit->text());
            close();
            return dialog->exec();
        }
        else{
            QMessageBox::information(this, "Informacja", "Nie znaleziono urlopów w wybranym miesiąciu");
        }
    }
    else{
        QMessageBox::warning(this, "Błąd", "Błędnie wprowadzony rok.");
    }

    return 0;
}


int SearchSickDialog::searchJobTime(){
    if(yearEdit->text().size() > 3 && yearEdit->text().size() < 5){
        QVector<Data::JobEvidenceRecord> records;
        QVector<Data::JobEvidenceRecord> findedRecords;
        QSqlQuery query;
        query.exec("SELECT id, employeeId, sum, daysSum, name, surname, month, year, days, hoursCount, type, employeeHours FROM jobTimeEvidence");
        while(query.next()){
            records.push_back(Data::JobEvidenceRecord{
                query.value(0).toInt(),
                query.value(1).toInt(),
                query.value(2).toInt(),
                query.value(3).toInt(),
                query.value(4).toString(),
                query.value(5).toString(),
                query.value(6).toString(),
                query.value(7).toString(),
                query.value(8).toString(),
                query.value(9).toString(),
                query.value(10).toString(),
                query.value(11).toInt()
            });
        }

        for(auto& record : records){
            if(record.month == monthBox->currentText() && record.year == yearEdit->text())
                findedRecords.push_back(record);
        }

        if(!findedRecords.empty()){
            DataTableDialog* dialog = new DataTableDialog(DataTableDialog::State::JOB_TIME_EVIDENCE, findedRecords, monthBox->currentText() + ", " + yearEdit->text());
            close();
            return dialog->exec();
        }
        else{
            QMessageBox::information(this, "Informacja", "Nie znaleziono ewidencji czasu pracy w wybranym miesiąciu");
        }
    }
    else{
        QMessageBox::warning(this, "Błąd", "Błędnie wprowadzony rok.");
    }

    return 0;
}


int SearchSickDialog::searchJobBills(){
    if(yearEdit->text().size() > 3 && yearEdit->text().size() < 5){
        QVector<Data::BillRecord> records;
        QVector<Data::BillRecord> findedRecords;
        QSqlQuery query;
        query.exec("SELECT id, docId, rewardBrutto, basicBrutto, freeBrutto, sickBrutto, insuranceOld, insuranceRent, insuranceDisease, zus, insuranceOldP, insuranceRentP, insuranceAccident,"
                   "zusP, incomeCost, incomeCostAfter, rate, insurance, insuranceAdd, insuranceSub, jobFund, fgsp, rewardNetto, payType, createDate FROM bills WHERE type = 'uop'");
        while(query.next()){
            records.push_back(Data::BillRecord{
                query.value(0).toInt(),
                query.value(1).toString(),
                query.value(2).toFloat(),
                query.value(3).toFloat(),
                query.value(4).toFloat(),
                query.value(5).toFloat(),
                query.value(6).toFloat(),
                query.value(7).toFloat(),
                query.value(8).toFloat(),
                query.value(9).toFloat(),
                query.value(10).toFloat(),
                query.value(11).toFloat(),
                query.value(12).toFloat(),
                query.value(13).toFloat(),
                query.value(14).toFloat(),
                query.value(15).toFloat(),
                query.value(16).toFloat(),
                query.value(17).toFloat(),
                query.value(18).toFloat(),
                query.value(19).toFloat(),
                query.value(20).toFloat(),
                query.value(21).toFloat(),
                query.value(22).toFloat(),
                query.value(23).toString(),
                query.value(24).toDate(),
                ""
            });
        }

        for(auto& record : records){
            if(record.date.month() == monthBox->currentIndex() + 1 && record.date.year() == yearEdit->text().toInt())
                findedRecords.push_back(record);
        }

        for(auto& record : findedRecords){
            QString docId{""};
            QString employeeId{""};
            query.exec("SELECT contractId FROM bills WHERE id = " + QString::number(record.id));

            if(query.next()){
                docId = query.value(0).toString();
                query.exec("SELECT employeeId FROM jobContracts WHERE docId = " + docId);

                if(query.next()){
                    employeeId = query.value(0).toString();
                    query.exec("SELECT name, surname FROM employees WHERE id = " + employeeId);

                    if(query.next()){
                        record.employeeName = query.value(0).toString() + "\n" + query.value(1).toString();
                    }
                }
            }
        }

        if(!findedRecords.empty()){
            DataTableDialog* dialog = new DataTableDialog(DataTableDialog::State::JOB_BILL_LIST, findedRecords, monthBox->currentText() + ", " + yearEdit->text());
            close();
            return dialog->exec();
        }
        else{
            QMessageBox::information(this, "Informacja", "Nie znaleziono indywidualnych list płac w wybranym miesiąciu");
        }
    }
    else{
        QMessageBox::warning(this, "Błąd", "Błędnie wprowadzony rok.");
    }

    return 0;
}


void SearchSickDialog::createJobTimeEvidence(){
    bool hasUOP{false};
    bool hasUZ{false};
    QDate start;
    QDate end;
    int hoursCount{0};
    int docId{0};
    QString name{""};
    QString surname{""};
    float freeDaysCount{0.0f};
    float maxFreeDaysCount{0.0f};

    QSqlQuery query;
    query.exec("SELECT uop, uz, name, surname, freeDaysCount FROM employees WHERE id = " + employeeID);
    if(query.next()){
        hasUOP = query.value(0).toBool();
        hasUZ = query.value(1).toBool();
        name = query.value(2).toString();
        surname = query.value(3).toString();
        freeDaysCount = query.value(4).toFloat();
    }

    if(typeBox->currentIndex() == 0){
        if(hasUOP){
            query.exec("SELECT jobStartDate, jobEndDate, hoursCount, docId, maxFreeDays FROM jobContracts WHERE active = true AND employeeId = " + employeeID);
            if(query.next()){
                start = query.value(0).toDate();
                end = query.value(1).toDate();
                hoursCount = query.value(2).toInt();
                docId = query.value(3).toInt();
                maxFreeDaysCount = query.value(4).toInt();
            }

            if(monthBox->currentIndex() + 1 >= start.month() && monthBox->currentIndex() + 1 <= end.month()){
                QString tableVariables = "(employeeId, name, surname, month, year, days, hoursCount, sum, daysSum, freeDaysSum, sickDaysSum, type, employeeHours, contractId)";
                QString bindVariables = "(:employeeId, :name, :surname, :month, :year, :days, :hoursCount, :sum, :daysSum, :freeDaysSum, :sickDaysSum, :type, :empHours, :contractId)";

                const int FD = getFreeDaysSum();
                const int FS = getSickDaysSum();
                query.prepare("INSERT INTO jobTimeEvidence " + tableVariables + " VALUES " + bindVariables);
                query.bindValue(":employeeId", employeeID);
                query.bindValue(":name", name);
                query.bindValue(":surname", surname);
                query.bindValue(":month", monthBox->currentText());
                query.bindValue(":year", yearEdit->text());
                query.bindValue(":days", getDaysAsString());
                query.bindValue(":hoursCount", getHourCountAsString());
                query.bindValue(":sum", getHoursSum(hoursCount));
                query.bindValue(":daysSum", getDaysSum());
                query.bindValue(":freeDaysSum", FD);
                query.bindValue(":sickDaysSum", FS);
                query.bindValue(":type", typeBox->currentText());
                query.bindValue(":empHours", hoursCount);
                query.bindValue(":contractId", docId);
                query.exec();

                if(FD == 0 && FS == 0){
                    freeDaysCount += std::ceil(maxFreeDaysCount / 12.0f);
                    query.exec("UPDATE employees SET freeDaysCount = " + QString::number(freeDaysCount) + " WHERE id = " + employeeID);
                }

                close();
                QMessageBox::information(this, "Sukces!", "Pomyślnie dodano nową ewidencję czasu pracy.");
            }
            else{
                QMessageBox::information(this, "Informacja", "Umowa pracownika nie obejmuję wybranego miesiąca.");
            }
        }
        else{
            QMessageBox::information(this, "Ups...", "Pracownik nie posiada umowy o pracę.");
        }
    }
    else if(typeBox->currentIndex() == 1){
        if(hasUZ){
            query.exec("SELECT jobStartDate, jobEndDate, hoursCount, docId FROM orderContracts WHERE active = true AND employeeId = " + employeeID);
            if(query.next()){
                start = query.value(0).toDate();
                end = query.value(1).toDate();
                hoursCount = query.value(2).toInt();
                docId = query.value(3).toInt();
            }

            if(monthBox->currentIndex() + 1 >= start.month() && monthBox->currentIndex() + 1 <= end.month()){
                QString tableVariables = "(employeeId, name, surname, month, year, days, hoursCount, sum, daysSum, freeDaysSum, sickDaysSum, type, employeeHours, contractId)";
                QString bindVariables = "(:employeeId, :name, :surname, :month, :year, :days, :hoursCount, :sum, :daysSum, :freeDaysSum, :sickDaysSum, :type, :empHours, :contractId)";

                query.prepare("INSERT INTO jobTimeEvidence " + tableVariables + " VALUES " + bindVariables);
                query.bindValue(":employeeId", employeeID);
                query.bindValue(":name", name);
                query.bindValue(":surname", surname);
                query.bindValue(":month", monthBox->currentText());
                query.bindValue(":year", yearEdit->text());
                query.bindValue(":days", getDaysAsString());
                query.bindValue(":hoursCount", getHourCountAsString());
                query.bindValue(":sum", getHoursSum(hoursCount));
                query.bindValue(":daysSum", getDaysSum());
                query.bindValue(":freeDaysSum", getFreeDaysSum());
                query.bindValue(":sickDaysSum", getSickDaysSum());
                query.bindValue(":type", typeBox->currentText());
                query.bindValue(":empHours", hoursCount);
                query.bindValue(":contractId", docId);
                query.exec();

                close();
                QMessageBox::information(this, "Sukces!", "Pomyślnie dodano nową ewidencję czasu pracy.");
            }
            else{
                QMessageBox::information(this, "Informacja", "Umowa pracownika nie obejmuję wybranego miesiąca.");
            }
        }
        else{
            QMessageBox::information(this, "Ups...", "Pracownik nie posiada umowy zlecenie.");
        }
    }
    else{
        QMessageBox::information(this, "Ups...", "Pracownik nie posiada żadnej umowy.");
    }
}


void SearchSickDialog::saveJobTimeEvidence(){
    bool hasUOP{false};
    bool hasUZ{false};
    QDate start;
    QDate end;
    int hoursCount{0};
    QString name{""};
    QString surname{""};

    QSqlQuery query;
    query.exec("SELECT uop, uz, name, surname FROM employees WHERE id = " + employeeID);
    if(query.next()){
        hasUOP = query.value(0).toBool();
        hasUZ = query.value(1).toBool();
        name = query.value(2).toString();
        surname = query.value(3).toString();
    }

    if(typeBox->currentIndex() == 0){
        if(hasUOP){
            query.exec("SELECT jobStartDate, jobEndDate, hoursCount FROM jobContracts WHERE active = true AND employeeId = " + employeeID);
            if(query.next()){
                start = query.value(0).toDate();
                end = query.value(1).toDate();
            }

            query.exec("SELECT employeeHours FROM jobTimeEvidence WHERE employeeId = " + employeeID);
            if(query.next()){
                hoursCount = query.value(0).toInt();
            }

            if(monthBox->currentIndex() + 1 >= start.month() && monthBox->currentIndex() + 1 <= end.month()){
                query.prepare("UPDATE jobTimeEvidence SET month = :month, year = :year, days = :days, hoursCount = :hoursCount, sum = :sum, daysSum = :daysSum, freeDaysSum = :freeDaysDum,"
                              "sickDaysSum = :sickDaysSum, type = :type WHERE id = " + id);
                query.bindValue(":month", monthBox->currentText());
                query.bindValue(":year", yearEdit->text());
                query.bindValue(":days", getDaysAsString());
                query.bindValue(":hoursCount", getHourCountAsString());
                query.bindValue(":sum", getHoursSum(hoursCount));
                query.bindValue(":daysSum", getDaysSum());
                query.bindValue(":freeDaysSum", getFreeDaysSum());
                query.bindValue(":sickDaysSum", getSickDaysSum());
                query.bindValue(":type", typeBox->currentText());
                query.exec();

                close();
                QMessageBox::information(this, "Sukces!", "Pomyślnie zaktualizowano ewidencję czasu pracy.");
            }
            else{
                QMessageBox::information(this, "Informacja", "Umowa pracownika nie obejmuję wybranego miesiąca.");
            }
        }
        else{
            QMessageBox::information(this, "Ups...", "Pracownik nie posiada umowy o pracę.");
        }
    }
    else if(typeBox->currentIndex() == 1){
        if(hasUZ){
            query.exec("SELECT jobStartDate, jobEndDate, hoursCount FROM orderContracts WHERE active = true AND employeeId = " + employeeID);
            if(query.next()){
                start = query.value(0).toDate();
                end = query.value(1).toDate();
            }

            query.exec("SELECT employeeHours FROM jobTimeEvidence WHERE employeeId = " + employeeID);
            if(query.next()){
                hoursCount = query.value(0).toInt();
            }

            if(monthBox->currentIndex() + 1 >= start.month() && monthBox->currentIndex() + 1 <= end.month()){
                query.prepare("UPDATE jobTimeEvidence SET month = :month, year = :year, days = :days, hoursCount = :hoursCount, sum = :sum, daysSum = :daysSum, freeDaysSum = :freeDaysSum,"
                              "sickDaysSum = :sickDaysSum, type = :type WHERE id = " + id);
                query.bindValue(":month", monthBox->currentText());
                query.bindValue(":year", yearEdit->text());
                query.bindValue(":days", getDaysAsString());
                query.bindValue(":hoursCount", getHourCountAsString());
                query.bindValue(":sum", getHoursSum(hoursCount));
                query.bindValue(":daysSum", getDaysSum());
                query.bindValue(":freeDaysSum", getFreeDaysSum());
                query.bindValue(":sickDaysSum", getSickDaysSum());
                query.bindValue(":type", typeBox->currentText());
                query.exec();

                close();
                QMessageBox::information(this, "Sukces!", "Pomyślnie zaktualizowano ewidencję czasu pracy.");
            }
            else{
                QMessageBox::information(this, "Informacja", "Umowa pracownika nie obejmuję wybranego miesiąca.");
            }
        }
        else{
            QMessageBox::information(this, "Ups...", "Pracownik nie posiada umowy zlecenie.");
        }
    }
    else{
        QMessageBox::information(this, "Ups...", "Pracownik nie posiada żadnej umowy.");
    }
}


void SearchSickDialog::refreshTable(int index){
    QDate now(2020, index + 1, 1);
    QStringList days, hours;
    if(changeData){
        QSqlQuery query;
        query.exec("SELECT days, hoursCount FROM jobTimeEvidence WHERE id = " + id);
        if(query.next()){
            days = query.value(0).toString().split(",");
            hours = query.value(1).toString().split(",");
        }
    }

    table->clearContents();
    table->setRowCount(now.daysInMonth());

    for(int i = 0; i < table->rowCount(); ++i){
        if(changeData)
            table->setItem(i, 0, new QTableWidgetItem(days.at(i)));
        else
            table->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));

        now.setDate(yearEdit->text().toInt(), monthBox->currentIndex() + 1, i + 1);
        table->setItem(i, 1, new QTableWidgetItem(now.toString("dddd")));

        if(table->item(i, 1)->text() != "sobota" && table->item(i, 1)->text() != "niedziela"){
            QComboBox* t = new QComboBox;
            t->setStyleSheet("QComboBox{ border: 2px solid #1c45a6; border-radius: 8px; padding: 5px; }");
            t->addItem("2");
            t->addItem("4");
            t->addItem("6");
            t->addItem("7");
            t->addItem("8");
            t->addItem("9");
            t->addItem("10");
            t->addItem("Urlop wyp.");
            t->addItem("Urlop rodz.");
            t->addItem("Urlop bez.");
            t->addItem("Chorobowe");

            table->setCellWidget(i, 1, t);
        }
    }

    if(changeData){
        QComboBox* c;
        for(int i = 0; i < table->rowCount(); ++i){
         c = qobject_cast<QComboBox*>(table->cellWidget(i, 1));
            if(!c){
                continue;
            }
            else{
                c->setCurrentIndex(hours.at(i).toInt());
            }
        }
    }
}
