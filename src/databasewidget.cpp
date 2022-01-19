#include "databasewidget.h"

DatabaseWidget::DatabaseWidget(QWidget *parent) : QWidget(parent), openedDirectory{ false }
{
    initializeUI();
    addToLayout();
    loadFromDirectory();
    connectUI();
}


void DatabaseWidget::initializeUI(){
    headerLabel = new QLabel("Bazy Danych");
    headerLabel->setAlignment(Qt::AlignCenter);
    headerLabel->setStyleSheet("QLabel{ font-size: 25px; }");

    statusLabel = new QLabel;
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("QLabel{ font-size: 25px; color: green; }");
    DBManager::isConnected() ? statusLabel->setText("Baza: " + DBManager::getDBName()) : statusLabel->setText("Baza: brak");

    dbListView = new QListWidget;

    addDbBtn = new QPushButton("Dodaj");
    addDbBtn->setStyleSheet(Data::defaultButtonSheet);
    addDbBtn->setFixedSize(Data::defaultButtonSize);

    deleteDbBtn = new QPushButton("Usuń");
    deleteDbBtn->setStyleSheet(Data::defaultButtonSheet);
    deleteDbBtn->setFixedSize(Data::defaultButtonSize);

    openDbBtn = new QPushButton("Otwórz");
    openDbBtn->setStyleSheet(Data::defaultButtonSheet);
    openDbBtn->setFixedSize(Data::defaultButtonSize);

    closeDbBtn = new QPushButton("Zamknij");
    closeDbBtn->setStyleSheet(Data::defaultButtonSheet);
    closeDbBtn->setFixedSize(Data::defaultButtonSize);
}


void DatabaseWidget::addToLayout(){
    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(statusLabel, 0, Qt::AlignLeft);
    buttonsLayout->addWidget(addDbBtn);
    buttonsLayout->addWidget(deleteDbBtn);
    buttonsLayout->addWidget(openDbBtn);
    buttonsLayout->addWidget(closeDbBtn);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(headerLabel);
    mainLayout->addWidget(dbListView);
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);
}


void DatabaseWidget::loadFromDirectory(){
    QDir currentDirectory;
    QString pathName { currentDirectory.currentPath() + "/Dane/Bazy" };
    dbDirectory.setPath(pathName);

    if(dbDirectory.exists()){
        openedDirectory = true;
        QStringList databases = dbDirectory.entryList(QStringList() << "*.db", QDir::Files);

        for(auto& fileName : databases){
            filesPaths.push_back(pathName + "/" + fileName);

            QListWidgetItem* item = new QListWidgetItem;
            item->setText(fileName);
            dbListView->addItem(item);
        }
    }
    else{
        QMessageBox::warning(this, "Brak ścieżki", "Nie można otworzyć ścieżki /Dane/Bazy. Proszę sprawdzić czy owy folder istnieje.");
    }
}


void DatabaseWidget::connectUI(){
    if(openedDirectory){
        connect(addDbBtn, &QPushButton::clicked, this, &DatabaseWidget::addDB);
        connect(deleteDbBtn, &QPushButton::clicked, this, &DatabaseWidget::removeDB);
        connect(openDbBtn, &QPushButton::clicked, this, &DatabaseWidget::connectToDB);
        connect(closeDbBtn, &QPushButton::clicked, this, &DatabaseWidget::closeConnectionToDB);
    }
}


void DatabaseWidget::addEmployeeVariables(){
    DBManager::addEmployeeVariables(QStringList{
        "Nr Pracownika",
        "Imię",
        "Drugie Imię",
        "Nazwisko",
        "Data Urodzenia",
        "Miejsce Urodzenia",
        "Kod Pocztowy",
        "Miejsce Zamieszkania",
        "Ulica",
        "Nr. Domu",
        "Nr. Lokalu",
        "Pesel",
        "Imiona Rodziców",
        "Nazwisko Rodowe",
        "Nr. Dowodu Osobistego",
        "Stanowisko"
    });

    DBManager::addJobContractVariables(QStringList{
       "Rodzaj Pracy",
       "Wynagrodzenie",
       "Wymiar Pracy",
       "Ilość godzin",
       "Termin rozpoczęcia",
       "Termin zakończenia"
    });
}



//Connecting Slots--------------------------------------------------
void DatabaseWidget::addDB(){
    bool accept{};
    QString fileName = QInputDialog::getText(this, "Nowa Baza Danych", "Nazwa bazy:", QLineEdit::Normal, QString(), &accept);

    if(accept && !fileName.isEmpty()){

        if(!DBManager::isConnected()){
            QString filePath(dbDirectory.path() + "/" + fileName + ".db");

            if(QFileInfo::exists(filePath)){
                QMessageBox::warning(this, "Błąd", "Baza danych o podanej nazwie już istnieje");
            }
            else{
                QFile file(filePath);
                file.open(QIODevice::ReadWrite);
                file.close();

                QListWidgetItem* item = new QListWidgetItem;
                item->setText(fileName + ".db");
                dbListView->addItem(item);
                filesPaths.push_back(filePath);

                DBManager::setDBName(filePath, fileName + ".db");
                if(DBManager::connectToDB()){
                    QSqlQuery query;
                    query.exec("create table mainCompany("
                               "id int,"
                               "name text,"
                               "director text,"
                               "nip text,"
                               "regon text,"
                               "city text,"
                               "cityCode text,"
                               "address text,"
                               "email text,"
                               "phone text,"
                               "office text,"
                               "balance float)");

                    query.exec("create table employees("
                            "id integer primary key autoincrement,"
                            "name text,"
                            "secondName text,"
                            "surname text,"
                            "birthDate date,"
                            "birthPlace text,"
                            "cityCode text,"
                            "city text,"
                            "address text,"
                            "houseNumber text,"
                            "localNumber text,"
                            "pesel text,"
                            "parentName text,"
                            "parentSurname text,"
                            "evidenceNumber text,"
                            "isWorking text,"
                            "activeWork boolean,"
                            "uop boolean,"
                            "uz boolean,"
                            "uod boolean,"
                            "freeDaysCount float)");

                    query.exec("create table jobContracts("
                               "docId integer primary key autoincrement,"
                               "employeeId int,"
                               "id text,"
                               "jobCategory text,"
                               "jobPlace text,"
                               "reward float,"
                               "payForHour float,"
                               "jobDimension text,"
                               "hoursCount int,"
                               "jobStartDate date,"
                               "jobEndDate date,"
                               "decision boolean,"
                               "decisionText text,"
                               "job text,"
                               "maxFreeDays int,"
                               "numberId int,"
                               "active boolean)");

                    query.exec("create table orderContracts("
                               "docId integer primary key autoincrement,"
                               "employeeId int,"
                               "id text,"
                               "reward float,"
                               "payForHour float,"
                               "hoursCount int,"
                               "jobStartDate date,"
                               "jobEndDate date,"
                               "jobDimension text,"
                               "job text,"
                               "numberId int,"
                               "active boolean)");

                    query.exec("create table workContracts("
                               "docId integer primary key autoincrement,"
                               "employeeId int,"
                               "id text,"
                               "reward float,"
                               "jobs text,"
                               "entrusts text,"
                               "jobStartDate date,"
                               "jobEndDate date,"
                               "jobDimension text,"
                               "numberId int,"
                               "active boolean,"
                               "hasBill boolean)");

                    query.exec("create table sickLeaves("
                               "id integer primary key autoincrement,"
                               "employeeId int,"
                               "leaveStartDate date,"
                               "leaveEndDate date,"
                               "daysCount int)");

                    query.exec("create table freeLeaves("
                               "id integer primary key autoincrement,"
                               "employeeId int,"
                               "leaveStartDate date,"
                               "leaveEndDate date,"
                               "daysCount int,"
                               "freeType text)");

                    query.exec("create table jobTimeEvidence("
                               "id integer primary key autoincrement,"
                               "employeeId int,"
                               "name text,"
                               "surname text,"
                               "month int,"
                               "year text,"
                               "days text,"
                               "hoursCount text,"
                               "sum int,"
                               "daysSum int,"
                               "freeDaysSum int,"
                               "sickDaysSum int,"
                               "type text,"
                               "employeeHours int,"
                               "contractId int,"
                               "hasBill boolean)");

                    query.exec("create table annexes("
                               "id integer primary key autoincrement,"
                               "docId text,"
                               "contractId int,"
                               "type text,"
                               "createDate date,"
                               "numberId int,"
                               "changeList text)");

                    query.exec("create table bills("
                               "id integer primary key autoincrement,"
                               "docId text,"
                               "rewardBrutto float,"
                               "insuranceOld float,"
                               "insuranceRent float,"
                               "insuranceDisease float,"
                               "zus float,"
                               "insuranceOldP float,"
                               "insuranceRentP float,"
                               "insuranceAccident float,"
                               "zusP float,"
                               "incomeCost float,"
                               "incomeCostAfter float,"
                               "rate float,"
                               "insurance float,"
                               "insuranceAdd float,"
                               "insuranceSub float,"
                               "jobFund float,"
                               "fgsp float,"
                               "rewardNetto float,"
                               "payType text,"
                               "createDate date,"
                               "contractId int,"
                               "jobEvidenceId int,"
                               "numberId int,"
                               "type text,"
                               "basicBrutto float,"
                               "freeBrutto float,"
                               "sickBrutto float,"
                               "employeeId int)");

                    query.exec("create table companies("
                               "id integer primary key autoincrement,"
                               "name text,"
                               "director text,"
                               "nip text,"
                               "regon text,"
                               "city text,"
                               "cityCode text,"
                               "address text,"
                               "email text,"
                               "phone text,"
                               "account text)");

                    query.exec("create table account("
                               "id integer primary key autoincrement,"
                               "companyName text,"
                               "balance float,"
                               "sellDocId text,"
                               "sellDocDate text,"
                               "sellDocBrutto text,"
                               "buyDocId text,"
                               "buyDocDate text,"
                               "buyDocBrutto text)");

                    query.exec("create table finance("
                               "id integer primary key autoincrement,"
                               "docId text,"
                               "createDate date,"
                               "payDate date,"
                               "payType text,"
                               "itemName text,"
                               "unit text,"
                               "count int,"
                               "discount int,"
                               "costNetto float,"
                               "costNettoDiscount float,"
                               "netto float,"
                               "vat int,"
                               "costVat float,"
                               "brutto float,"
                               "sumNetto float,"
                               "sumVat float,"
                               "sumBrutto float,"
                               "companyId int,"
                               "type text,"
                               "numberId int,"
                               "status text)");

                    query.exec("create table incomes("
                               "id integer primary key autoincrement,"
                               "docId text,"
                               "createDate date,"
                               "info text,"
                               "cost text,"
                               "sum float,"
                               "numberId int,"
                               "companyId int)");

                    query.exec("create table leaves("
                               "id integer primary key autoincrement,"
                               "docId text,"
                               "createDate date,"
                               "info text,"
                               "cost text,"
                               "sum float,"
                               "numberId int,"
                               "companyId int)");

                    query.finish();
                    DBManager::closeDB();
                }

                QMessageBox::information(this, "Sukces!", "Stworzono nową bazę danych.");
            }
        }
        else{
            QMessageBox::warning(this, "Informacja", "Aktualnie otworzona baza danych to: " + DBManager::getDBName() + ". Proszę zamknąć bazę danych.");
        }
    }
    else{
        QMessageBox::warning(this, "Brak nazwy", "Proszę podać nazwę pliku.");
    }
}


void DatabaseWidget::removeDB(){
    if(dbListView->selectedItems().size() != 0){
        if(!DBManager::isConnected()){
            const int currentIndex = dbListView->currentRow();
            QString fileName = filesPaths.at(currentIndex);
            QMessageBox::StandardButton response = QMessageBox::question(this, "Usunięcie bazy", "Czy na pewno chcesz usunąc bazę danych " + fileName + "?");

            if(response == QMessageBox::Yes){
                QFile file(fileName);
                file.remove();
                dbListView->takeItem(currentIndex);

                QMessageBox::information(this, "Sukces!", "Pomyślnie usunięto bazę danych.");
            }
        }
        else{
            QMessageBox::warning(this, "Informacja", "Aktualnie otworzona baza danych to: " + DBManager::getDBName() + ". Proszę zamknąć bazę danych.");
        }
    }
    else{
        QMessageBox::warning(this, "Błąd", "Proszę o wybranie bazy danych do usunięcia.");
    }
}


void DatabaseWidget::connectToDB(){
    int currentIndex = 0;
    QString dbShowName = "";

    if(DBManager::isConnected()){
        QMessageBox::warning(this, "Informacja", "Aktualnie otworzona baza danych to: " + DBManager::getDBName());
    }
    else if(dbListView->selectedItems().size() != 0 && !DBManager::isConnected()){
        currentIndex = dbListView->currentRow();
        dbShowName = dbListView->currentItem()->text();

        DBManager::setDBName(filesPaths.at(currentIndex), dbShowName);

        if(DBManager::connectToDB()){
            addEmployeeVariables();
            statusLabel->setText("Baza: " + DBManager::getDBName());
            QMessageBox::information(this, "Informacja", "Pomyślnie otwarto bazę danych!");
        }
    }
    else if(dbListView->selectedItems().size() == 0){
        QMessageBox::warning(this, "Błąd", "Proszę o wybranie bazy danych do połączenia.");
    }
}


void DatabaseWidget::closeConnectionToDB(){
    if(DBManager::isConnected()){
        DBManager::closeDB();
        statusLabel->setText("Baza: brak");
        QMessageBox::information(this, "Informacja", "Pomyślnie zamknięto bazę danych.");
    }
    else{
        QMessageBox::information(this, "Informacja", "Aktualnie żadna baza danych nie jest otwarta.");
    }
}
