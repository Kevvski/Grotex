#include "dbviewdialog.h"

DBViewDialog::DBViewDialog(const State& state, const AppState::ContractState& contractState)
{
    setWindowTitle("KadriX");
    this->contractState = contractState;
    initializeUI(state);

    switch (AppState::getState()) {
        case AppState::State::EMPLOYEES:
            initializeEmployeeUI();
            connectUIEmployee();
            break;

        case AppState::State::CUSTOMERS:
            initializeCompaniesUI();
            connectUICompanies();
            break;
    }

    addToLayout();
}


DBViewDialog::DBViewDialog(const DBViewDialog::State& state, const QString& id, int index)
{
    setWindowTitle("KadriX");
    this->state = state;
    this->empId = id;
    this->certificateIndex = index;
    initializeUI(state);

    switch (AppState::getState()) {
    case AppState::State::EMPLOYEES:
        initializeEmployeeUI();
        connectUIEmployee();
        break;

    case AppState::State::CUSTOMERS:
        initializeCompaniesUI();
        connectUICompanies();
        break;
    }

    addToLayout();
}


DBViewDialog::DBViewDialog(const DBViewDialog::State& state, const FinanceDialog::State& financeState)
{
    setWindowTitle("KadriX");
    this->state = state;
    this->financeState = financeState;
    initializeUI(state);

    switch (AppState::getState()) {
    case AppState::State::EMPLOYEES:
        initializeEmployeeUI();
        connectUIEmployee();
        break;

    case AppState::State::CUSTOMERS:
        initializeCompaniesUI();
        connectUICompanies();
        break;
    }

    addToLayout();
}


void DBViewDialog::initializeUI(const State& state){
    this->state = state;
    setFixedSize(800, 600);
    setStyleSheet("background-color: white");

    headerLabel = new QLabel;
    headerLabel->setAlignment(Qt::AlignCenter);
    headerLabel->setStyleSheet("QLabel{ font-size: 25px; font-weight: bold; }");

    model = new QSqlTableModel;

    view = new QTableView;
    view->setModel(model);
    view->resizeColumnsToContents();
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);

    if(state != DBViewDialog::State::PAY_CERTIFICATE_2 && state != DBViewDialog::State::HOUSE_CERTIFICATE)
        view->setSelectionMode(QAbstractItemView::SingleSelection);

    addBtn = new QPushButton("Dodaj");
    addBtn->setFixedSize(Data::defaultButtonSize);
    addBtn->setStyleSheet(Data::defaultButtonSheet);

    editBtn = new QPushButton("Edytuj");
    editBtn->setFixedSize(Data::defaultButtonSize);
    editBtn->setStyleSheet(Data::defaultButtonSheet);

    deleteBtn = new QPushButton("Usuń");
    deleteBtn->setFixedSize(Data::defaultButtonSize);
    deleteBtn->setStyleSheet(Data::defaultButtonSheet);

    cancelBtn = new QPushButton("Wróć");
    cancelBtn->setFixedSize(Data::defaultButtonSize);
    cancelBtn->setStyleSheet(Data::defaultButtonSheet);

    printBtn = new QPushButton("Drukuj");
    printBtn->setFixedSize(Data::defaultButtonSize);
    printBtn->setStyleSheet(Data::defaultButtonSheet);

    printLPBtn = new QPushButton("Drukuj LP.");
    printLPBtn->setFixedSize(Data::defaultButtonSize);
    printLPBtn->setStyleSheet(Data::defaultButtonSheet);
}


void DBViewDialog::initializeEmployeeUI(){
    switch(state){
    case DBViewDialog::State::EMPLOYEE_LIST:
        headerLabel->setText("Lista Pracowników");
        initializeEmployeeTable();
        break;

    case DBViewDialog::State::JOB_CONTRACTS_LIST:
        headerLabel->setText("Lista Umów o Pracę");
        initializeJobTable();
        break;

    case DBViewDialog::State::ORDER_CONTRACTS_LIST:
        headerLabel->setText("Lista Umów Zlecenie");
        initializeOrderTable();
        break;

    case DBViewDialog::State::WORK_CONTRACTS_LIST:
        headerLabel->setText("Lista Umów o Dzieło");
        initializeWorkTable();
        break;

    case DBViewDialog::State::SICK_LEAVE_LIST:
        headerLabel->setText("Lista Zwolnień Chorobowych");
        initializeSickLeaveTable();
        break;

    case DBViewDialog::State::FREE_LEAVE_LIST:
        headerLabel->setText("Lista Urlopów");
        initializeFreeLeaveTable();
        break;

    case DBViewDialog::State::JOB_TIME_LIST:
        headerLabel->setText("Lista ewidencji czasu pracy");
        initializeJobTimeTable();
        break;

    case DBViewDialog::State::ADD_CONTRACT:
        headerLabel->setText("Wybierz Pracownika");
        initializeEmployeeTable();
        break;

    case DBViewDialog::State::ADD_SICKLEAVE:
        headerLabel->setText("Wybierz Pracownika");
        initializeEmployeeTable();
        break;

    case DBViewDialog::State::ADD_FREELEAVE:
        headerLabel->setText("Wybierz Pracownika");
        initializeEmployeeTable();
        break;

    case DBViewDialog::State::ADD_JOB_TIME_EVIDENCE:
        headerLabel->setText("Wybierz Pracownika");
        initializeEmployeeTable();
        break;

    case DBViewDialog::State::COUNT_ORDER_CONTRACT:
        headerLabel->setText("Wybierz ewidencję czasu pracy");
        initializeJobTimeTable(OrderBillDialog::State::ORDER_BILL);
        break;

    case DBViewDialog::State::BILLS_LIST:
        headerLabel->setText("Lista rachunków");
        initializeCountOrderTable();
        break;

    case DBViewDialog::State::COUNT_WORK_CONTRACT:
        headerLabel->setText("Wybierz umowę o dzieło");
        initializeWorkTable();
        break;

    case DBViewDialog::State::COUNT_JOB_CONTRACT:
        headerLabel->setText("Wybierz ewidencje czasu pracy");
        initializeJobTimeTable(OrderBillDialog::State::JOB_BILL);
        break;

    case DBViewDialog::State::CHOOSE_TO_ANNEX:
        headerLabel->setText("Wybierz umowę");
        switch (contractState) {
            case AppState::ContractState::JOB_ANNEX: initializeJobTable(); break;
            case AppState::ContractState::ORDER_ANNEX: initializeOrderTable(); break;
            case AppState::ContractState::WORK_ANNEX: initializeWorkTable(); break;
        };
        break;

    case DBViewDialog::State::ANNEX_LIST:
        headerLabel->setText("Lista aneksów");
        initializeAnnexTable();
        break;

    case DBViewDialog::State::PAY_CERTIFICATE:
        headerLabel->setText("Wybierz pracownika");
        initializeEmployeeTable();
        break;

    case DBViewDialog::State::PAY_CERTIFICATE_2:
        headerLabel->setText("Wybierz rozliczenia");
        initializeCountOrderTable();
        break;

    case DBViewDialog::State::HOUSE_CERTIFICATE:
        headerLabel->setText("Wybierz rozliczenia");
        initializeCountOrderTable();
        break;

    case DBViewDialog::State::FAMILY_CERTIFICATE:
        headerLabel->setText("Wybierz rozliczenia");
        initializeCountOrderTable();
        break;

    case DBViewDialog::State::MEINTENANCE_CERTIFICATE:
        headerLabel->setText("Wybierz rozliczenia");
        initializeCountOrderTable();
        break;
    }
}


void DBViewDialog::initializeCompaniesUI(){
    switch(state){
    case DBViewDialog::State::ADD_BUY_DOC:
        headerLabel->setText("Wybierz Firmę");
        initializeCompaniesTable();
        break;

    case DBViewDialog::State::COMPENSATION_LIST:
        headerLabel->setText("Kompensaty(Konta)");
        addBtn->setText("Rozliczono");
        initializeAccountTable();
        break;

    case DBViewDialog::State::COMPANIES_LIST:
        headerLabel->setText("Lista Firm");
        initializeCompaniesTable();
        break;

    case DBViewDialog::State::FINANCE_DOC_LIST:
        initializeFinanceDocsTable();
        break;
    }
}


void DBViewDialog::initializeEmployeeTable(){
    model->setTable("employees");
    model->select();

    model->setHeaderData(0, Qt::Horizontal, "Numer Pracownika");
    model->setHeaderData(1, Qt::Horizontal, "Imię");
    model->setHeaderData(2, Qt::Horizontal, "Drugie Imię");
    model->setHeaderData(3, Qt::Horizontal, "Nazwisko");
    model->setHeaderData(4, Qt::Horizontal, "Data Urodzenia");
    model->setHeaderData(5, Qt::Horizontal, "Miejsce Urodzenia");
    model->setHeaderData(6, Qt::Horizontal, "Kod Pocztowy");
    model->setHeaderData(7, Qt::Horizontal, "Miejsce Zamieszkania");
    model->setHeaderData(8, Qt::Horizontal, "Ulica");
    model->setHeaderData(9, Qt::Horizontal, "Nr. Domu");
    model->setHeaderData(10, Qt::Horizontal, "Nr. Lokalu");
    model->setHeaderData(11, Qt::Horizontal, "Pesel");
    model->setHeaderData(12, Qt::Horizontal, "Imiona Rodziców");
    model->setHeaderData(13, Qt::Horizontal, "Nazwisko Rodowe");
    model->setHeaderData(14, Qt::Horizontal, "Nr. Dowodu Osobistego");
    model->setHeaderData(15, Qt::Horizontal, "Czy Pracuję");
    model->setHeaderData(20, Qt::Horizontal, "Dostępne dni wolne");

    view->hideColumn(16);
    view->hideColumn(17);
    view->hideColumn(18);
    view->hideColumn(19);

    printBtn->setText("Historia");
}


void DBViewDialog::initializeJobTable(){
    model->setTable("jobContracts");
    model->select();

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Numer Pracownika");
    model->setHeaderData(2, Qt::Horizontal, "Numer umowy");
    model->setHeaderData(3, Qt::Horizontal, "Rodzaj Pracy");
    model->setHeaderData(4, Qt::Horizontal, "Miejsce Pracy");
    model->setHeaderData(5, Qt::Horizontal, "Wynagrodzenie");
    model->setHeaderData(6, Qt::Horizontal, "Stawka");
    model->setHeaderData(7, Qt::Horizontal, "Wymiar Pracy");
    model->setHeaderData(8, Qt::Horizontal, "Ilość godzin(dzień)");
    model->setHeaderData(9, Qt::Horizontal, "Data zatrudnienia");
    model->setHeaderData(10, Qt::Horizontal, "Data zwolnienia");
    model->setHeaderData(12, Qt::Horizontal, "Orz. o Niepełnosprawności");
    model->setHeaderData(13, Qt::Horizontal, "Stanowisko");
    model->setHeaderData(14, Qt::Horizontal, "Urlop w roku(dni)");

    view->hideColumn(11);
    view->hideColumn(15);
    view->hideColumn(16);
}


void DBViewDialog::initializeOrderTable(){
    model->setTable("orderContracts");
    model->select();

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Numer Pracownika");
    model->setHeaderData(2, Qt::Horizontal, "Numer umowy");
    model->setHeaderData(3, Qt::Horizontal, "Wynagrodzenie");
    model->setHeaderData(4, Qt::Horizontal, "Stawka");
    model->setHeaderData(5, Qt::Horizontal, "Ilośc godzin(dzień)");
    model->setHeaderData(6, Qt::Horizontal, "Data zatrudnienia");
    model->setHeaderData(7, Qt::Horizontal, "Data zakończenia");
    model->setHeaderData(8, Qt::Horizontal, "Wymiar pracy");
    model->setHeaderData(9, Qt::Horizontal, "Stanowisko");

    view->setColumnWidth(0, 80);
    view->setColumnWidth(1, 80);
    view->setColumnWidth(2, 135);
    view->setColumnWidth(3, 200);
    view->setColumnWidth(4, 120);
    view->setColumnWidth(5, 120);
    view->setColumnWidth(6, 120);
    view->setColumnWidth(7, 120);
    view->setColumnWidth(8, 120);
    view->setColumnWidth(9, 120);

    view->hideColumn(10);
    view->hideColumn(11);
}


void DBViewDialog::initializeWorkTable(){
    model->setTable("workContracts");
    model->select();

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Numer Pracownika");
    model->setHeaderData(2, Qt::Horizontal, "Numer umowy");
    model->setHeaderData(3, Qt::Horizontal, "Wynagrodzenie");
    model->setHeaderData(6, Qt::Horizontal, "Data zatrudnienia");
    model->setHeaderData(7, Qt::Horizontal, "Data zakończenia");
    model->setHeaderData(8, Qt::Horizontal, "Wymiar pracy");

    view->hideColumn(4);
    view->hideColumn(5);
    view->hideColumn(9);
    view->hideColumn(10);
    view->hideColumn(11);

    view->setColumnWidth(0, 80);
    view->setColumnWidth(1, 120);
    view->setColumnWidth(2, 135);
    view->setColumnWidth(3, 200);
    view->setColumnWidth(6, 120);
    view->setColumnWidth(7, 120);
    view->setColumnWidth(8, 120);
}


void DBViewDialog::initializeSickLeaveTable(){
    model->setTable("sickLeaves");
    model->select();

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Numer Pracownika");
    model->setHeaderData(2, Qt::Horizontal, "Data rozpoczęcia");
    model->setHeaderData(3, Qt::Horizontal, "Data Zakończenia");
    model->setHeaderData(4, Qt::Horizontal, "Ilość dni");

    printBtn->setText("Ewidencjonuj");

    view->setColumnWidth(0, 100);
    view->setColumnWidth(1, 135);
    view->setColumnWidth(2, 180);
    view->setColumnWidth(3, 180);
    view->setColumnWidth(4, 170);
}


void DBViewDialog::initializeFreeLeaveTable(){
    model->setTable("freeLeaves");
    model->select();

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Numer Pracownika");
    model->setHeaderData(2, Qt::Horizontal, "Data rozpoczęcia");
    model->setHeaderData(3, Qt::Horizontal, "Data Zakończenia");
    model->setHeaderData(4, Qt::Horizontal, "Ilość dni");
    model->setHeaderData(5, Qt::Horizontal, "Rodzaj urlopu");

    printBtn->setText("Ewidencjonuj");

    view->setColumnWidth(0, 100);
    view->setColumnWidth(1, 135);
    view->setColumnWidth(2, 120);
    view->setColumnWidth(3, 120);
    view->setColumnWidth(4, 135);
    view->setColumnWidth(5, 150);
}


void DBViewDialog::initializeJobTimeTable(const OrderBillDialog::State& state){
    model->setTable("jobTimeEvidence");
    model->select();

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Numer Pracownika");
    model->setHeaderData(2, Qt::Horizontal, "Imię");
    model->setHeaderData(3, Qt::Horizontal, "Nazwisko");
    model->setHeaderData(4, Qt::Horizontal, "Miesiąc");
    model->setHeaderData(5, Qt::Horizontal, "Rok");
    model->setHeaderData(8, Qt::Horizontal, "Suma godzin");
    model->setHeaderData(9, Qt::Horizontal, "Suma Dni");
    model->setHeaderData(10, Qt::Horizontal, "Suma Dni(Urlop)");
    model->setHeaderData(11, Qt::Horizontal, "Suma Dni(Chorobowe)");
    model->setHeaderData(12, Qt::Horizontal, "Rodzaj Umowy");

    printBtn->setText("Ewidencjonuj");

    view->hideColumn(6);
    view->hideColumn(7);
    view->hideColumn(13);
    view->hideColumn(14);
    view->hideColumn(15);

    if(state == OrderBillDialog::State::ORDER_BILL)
        model->setFilter("type = 'Umowa Zlecenie'");
    else if(state == OrderBillDialog::State::JOB_BILL)
        model->setFilter("type = 'Umowa o Pracę'");
}


void DBViewDialog::initializeAnnexTable(){
    model->setTable("annexes");
    model->select();

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Numer Aneksu");
    model->setHeaderData(2, Qt::Horizontal, "Numer umowy");
    model->setHeaderData(3, Qt::Horizontal, "Rodzaj umowy");
    model->setHeaderData(4, Qt::Horizontal, "Data utworzenia");

    view->hideColumn(5);
    view->hideColumn(6);

    view->setColumnWidth(0, 100);
    view->setColumnWidth(1, 160);
    view->setColumnWidth(2, 160);
    view->setColumnWidth(3, 160);
    view->setColumnWidth(4, 185);
}


void DBViewDialog::initializeCountOrderTable(){
    model->setTable("bills");
    model->select();

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Numer rachunku");
    model->setHeaderData(2, Qt::Horizontal, "Kwota brutto(zł)");

    model->setHeaderData(3, Qt::Horizontal, "Ubez. Emerytalne(zł)(pracownik)");
    model->setHeaderData(4, Qt::Horizontal, "Ubez. Rentowne(zł)(pracownik)");
    model->setHeaderData(5, Qt::Horizontal, "Ubez. Chorobowe(zł)(pracownik)");
    model->setHeaderData(6, Qt::Horizontal, "Składki Ubez. Społ(zł)(pracownik)");

    model->setHeaderData(7, Qt::Horizontal, "Ubez. Emerytalne(zł)(pracodawca)");
    model->setHeaderData(8, Qt::Horizontal, "Ubez. Rentowne(zł)(pracodawca)");
    model->setHeaderData(9, Qt::Horizontal, "Ubez. Wypadkowe(zł)(pracodawca)");
    model->setHeaderData(10, Qt::Horizontal, "Składki Ubez. Społ(zł)(pracodawca)");

    model->setHeaderData(11, Qt::Horizontal, "Koszt przychodu(zł)");
    model->setHeaderData(12, Qt::Horizontal, "Dochód do opod.(zł)");
    model->setHeaderData(13, Qt::Horizontal, "Podatek");
    model->setHeaderData(14, Qt::Horizontal, "Ubez. zdro. 9%(zł)");
    model->setHeaderData(15, Qt::Horizontal, "Ubez. zdro. 7,75%(zł)");
    model->setHeaderData(16, Qt::Horizontal, "Ubez. zdro. 1,25%(zł)");
    model->setHeaderData(17, Qt::Horizontal, "Fundusz Pracy");
    model->setHeaderData(18, Qt::Horizontal, "FGŚP");

    model->setHeaderData(19, Qt::Horizontal, "Kwota Netto(zł)");
    model->setHeaderData(20, Qt::Horizontal, "Sposób płatności");
    model->setHeaderData(21, Qt::Horizontal, "Data utworzenia");

    model->setHeaderData(26, Qt::Horizontal, "Wyna. Zasadnicze(zł)");
    model->setHeaderData(27, Qt::Horizontal, "Wyna. Urlopowe(zł)");
    model->setHeaderData(28, Qt::Horizontal, "Zasiłek Chorobowy(zł)");

    view->hideColumn(22);
    view->hideColumn(23);
    view->hideColumn(24);
    view->hideColumn(25);
    view->hideColumn(29);

    if(state == DBViewDialog::State::PAY_CERTIFICATE_2 && state == DBViewDialog::State::HOUSE_CERTIFICATE &&
        state == DBViewDialog::State::FAMILY_CERTIFICATE && state == DBViewDialog::State::MEINTENANCE_CERTIFICATE){
        model->setFilter("employeeId = " + empId);
    }
}


void DBViewDialog::initializeCompaniesTable(){
    model->setTable("companies");
    model->select();

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nazwa Firmy");
    model->setHeaderData(2, Qt::Horizontal, "Przedstawiciel");
    model->setHeaderData(3, Qt::Horizontal, "NIP");
    model->setHeaderData(4, Qt::Horizontal, "REGON");
    model->setHeaderData(5, Qt::Horizontal, "Miasto");
    model->setHeaderData(6, Qt::Horizontal, "Kod Pocztowy");
    model->setHeaderData(7, Qt::Horizontal, "Adres");
    model->setHeaderData(8, Qt::Horizontal, "E-mail");
    model->setHeaderData(9, Qt::Horizontal, "Nr. Telefonu");
    model->setHeaderData(10, Qt::Horizontal, "Nr. Konta Bankowego");

    view->setColumnWidth(0, 80);
    view->setColumnWidth(1, 120);
    view->setColumnWidth(2, 120);
    view->setColumnWidth(3, 120);
    view->setColumnWidth(4, 120);
    view->setColumnWidth(5, 120);
    view->setColumnWidth(6, 120);
    view->setColumnWidth(7, 120);
    view->setColumnWidth(8, 120);
    view->setColumnWidth(9, 120);
    view->setColumnWidth(10, 300);
}


void DBViewDialog::initializeFinanceDocsTable(){
    model->setTable("finance");
    model->select();

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Numer Dokumentu");
    model->setHeaderData(2, Qt::Horizontal, "Data Utworzenia");
    model->setHeaderData(3, Qt::Horizontal, "Termin Zapłaty");
    model->setHeaderData(4, Qt::Horizontal, "Sposób zapłaty");
    model->setHeaderData(15, Qt::Horizontal, "Suma Netto");
    model->setHeaderData(16, Qt::Horizontal, "Suma VAT");
    model->setHeaderData(17, Qt::Horizontal, "Suma Brutto");

    view->hideColumn(5);
    view->hideColumn(6);
    view->hideColumn(7);
    view->hideColumn(8);
    view->hideColumn(9);
    view->hideColumn(10);
    view->hideColumn(11);
    view->hideColumn(12);
    view->hideColumn(13);
    view->hideColumn(14);
    view->hideColumn(18);
    view->hideColumn(19);
    view->hideColumn(20);
    view->hideColumn(21);

    printLPBtn->setText("Wystaw");

    switch(financeState){
        case FinanceDialog::State::FZ:
            model->setFilter("type = 'FZ'");
            headerLabel->setText("Lista Faktur Zakupu");
            break;

        case FinanceDialog::State::FS:
            model->setFilter("type = 'FS'");
            headerLabel->setText("Lista Faktur Sprzedaży");
            break;

        case FinanceDialog::State::WZ:
            model->setFilter("type = 'WZ'");
            headerLabel->setText("Lista Wydań Zewnętrznych");
            break;

        case FinanceDialog::State::PZ:
            model->setFilter("type = 'PZ'");
            headerLabel->setText("Lista Przyjęć Zewnętrznych");
            break;
    }
}


void DBViewDialog::initializeAccountTable(){
    model->setTable("account");
    model->select();

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Firma");
    model->setHeaderData(2, Qt::Horizontal, "Saldo");

    view->hideColumn(3);
    view->hideColumn(4);
    view->hideColumn(5);
    view->hideColumn(6);
    view->hideColumn(7);
    view->hideColumn(8);

    view->setColumnWidth(1, 340);
    view->setColumnWidth(2, 335);
}


void DBViewDialog::connectUIEmployee(){
    switch(state){
        case DBViewDialog::State::EMPLOYEE_LIST:
            connect(printBtn, &QPushButton::clicked, this, &DBViewDialog::openEmployeeStory);
            connect(editBtn, &QPushButton::clicked, this, &DBViewDialog::changeEmployeeData);
            connect(deleteBtn, &QPushButton::clicked, this, &DBViewDialog::deleteEmployeeData);
            break;

        case DBViewDialog::State::JOB_CONTRACTS_LIST:
            if(CompanyWidget::isExists())
                connect(printBtn, &QPushButton::clicked, this, &DBViewDialog::printJobContract);
            else
                connect(printBtn, &QPushButton::clicked, this, [this](){ QMessageBox::information(this, "Informacja", "Brak danych firmy."); });

            connect(editBtn, &QPushButton::clicked, this, &DBViewDialog::changeJobContractData);
            connect(deleteBtn, &QPushButton::clicked, this, &DBViewDialog::deleteJobContractData);
            break;

        case DBViewDialog::State::ORDER_CONTRACTS_LIST:
            if(CompanyWidget::isExists())
                connect(printBtn, &QPushButton::clicked, this, &DBViewDialog::printOrderContract);
            else
                connect(printBtn, &QPushButton::clicked, this, [this](){ QMessageBox::information(this, "Informacja", "Brak danych firmy."); });

            connect(editBtn, &QPushButton::clicked, this, &DBViewDialog::changeOrderContractData);
            connect(deleteBtn, &QPushButton::clicked, this, &DBViewDialog::deleteOrderContractData);
            break;

        case DBViewDialog::State::WORK_CONTRACTS_LIST:
            if(CompanyWidget::isExists())
                connect(printBtn, &QPushButton::clicked, this, &DBViewDialog::printWorkContract);
            else
                connect(printBtn, &QPushButton::clicked, this, [this](){ QMessageBox::information(this, "Informacja", "Brak danych firmy."); });

            connect(editBtn, &QPushButton::clicked, this, &DBViewDialog::changeWorkContractData);
            connect(deleteBtn, &QPushButton::clicked, this, &DBViewDialog::deleteWorkContractData);
            break;

        case DBViewDialog::State::SICK_LEAVE_LIST:
            connect(printBtn, &QPushButton::clicked, this, &DBViewDialog::createSickEvidence);
            connect(editBtn, &QPushButton::clicked, this, &DBViewDialog::changeSickLeaveData);
            connect(deleteBtn, &QPushButton::clicked, this, &DBViewDialog::deleteSickLeaveData);
            break;

        case DBViewDialog::State::FREE_LEAVE_LIST:
            connect(printBtn, &QPushButton::clicked, this, &DBViewDialog::createFreeEvidence);
            connect(editBtn, &QPushButton::clicked, this, &DBViewDialog::changeFreeLeaveData);
            connect(deleteBtn, &QPushButton::clicked, this, &DBViewDialog::deleteFreeLeaveData);
            break;

        case DBViewDialog::State::JOB_TIME_LIST:
            connect(printBtn, &QPushButton::clicked, this, &DBViewDialog::searchJobTimeEvidences);
            connect(editBtn, &QPushButton::clicked, this, &DBViewDialog::changeJobTimeData);
            connect(deleteBtn, &QPushButton::clicked, this, &DBViewDialog::deleteJobTimeData);
            break;

        case DBViewDialog::State::ADD_CONTRACT:
            if(CompanyWidget::isExists())
                connect(addBtn, &QPushButton::clicked, this, &DBViewDialog::addContract);
            else
                connect(addBtn, &QPushButton::clicked, this, [this](){ QMessageBox::information(this, "Informacja", "Brak danych firmy"); });
            break;

        case DBViewDialog::State::ADD_SICKLEAVE:
            connect(addBtn, &QPushButton::clicked, this, &DBViewDialog::addSickLeave);
            break;

        case DBViewDialog::State::ADD_FREELEAVE:
            connect(addBtn, &QPushButton::clicked, this, &DBViewDialog::addFreeLeave);
            break;

        case DBViewDialog::State::ADD_JOB_TIME_EVIDENCE:
            connect(addBtn, &QPushButton::clicked, this, &DBViewDialog::createJobTimeEvidence);
            break;

        case DBViewDialog::State::CHOOSE_TO_ANNEX:
            if(CompanyWidget::isExists())
                connect(addBtn, &QPushButton::clicked, this, &DBViewDialog::createAnnex);
            else
                connect(addBtn, &QPushButton::clicked, this, [this](){ QMessageBox::information(this, "Informacja", "Brak danych firmy"); });
            break;

        case DBViewDialog::State::ANNEX_LIST:
            if(CompanyWidget::isExists())
                connect(printBtn, &QPushButton::clicked, this, &DBViewDialog::printAnnex);
            else
                connect(printBtn, &QPushButton::clicked, this, [this](){ QMessageBox::information(this, "Informacja", "Brak danych firmy."); });
            connect(editBtn, &QPushButton::clicked, this, &DBViewDialog::changeAnnexData);
            connect(deleteBtn, &QPushButton::clicked, this, &DBViewDialog::deleteAnnexData);
            break;

        case DBViewDialog::State::COUNT_ORDER_CONTRACT:
            connect(addBtn, &QPushButton::clicked, this, &DBViewDialog::createOrderBill);
            break;

        case DBViewDialog::State::COUNT_WORK_CONTRACT:
            connect(addBtn, &QPushButton::clicked, this, &DBViewDialog::createWorkBill);
            break;

        case DBViewDialog::State::COUNT_JOB_CONTRACT:
            connect(addBtn, &QPushButton::clicked, this, &DBViewDialog::createJobBill);
            break;

        case DBViewDialog::State::BILLS_LIST:
            if(CompanyWidget::isExists()){
                connect(printLPBtn, &QPushButton::clicked, this, &DBViewDialog::createJobBillList);
                connect(printBtn, &QPushButton::clicked, this, &DBViewDialog::printOrderBill);
            }
            else{
                connect(printLPBtn, &QPushButton::clicked, this, [this](){ QMessageBox::information(this, "Informacja", "Brak danych firmy."); });
                connect(printBtn, &QPushButton::clicked, this, [this](){ QMessageBox::information(this, "Informacja", "Brak danych firmy."); });
            }
            connect(editBtn, &QPushButton::clicked, this, &DBViewDialog::changeOrderBillData);
            connect(deleteBtn, &QPushButton::clicked, this, &DBViewDialog::deleteOrderBillData);
            break;

        case DBViewDialog::State::PAY_CERTIFICATE:
            if(CompanyWidget::isExists()){
                connect(addBtn, &QPushButton::clicked, this, &DBViewDialog::createPayCertificate);
            }
            else{
                connect(addBtn, &QPushButton::clicked, this, [this](){ QMessageBox::information(this, "Informacja", "Brak danych firmy."); });
            }
            break;

        case DBViewDialog::State::PAY_CERTIFICATE_2:
            if(CompanyWidget::isExists()){
                connect(printBtn, &QPushButton::clicked, this, &DBViewDialog::printPayCertificate);
            }
            else{
                connect(printBtn, &QPushButton::clicked, this, [this](){ QMessageBox::information(this, "Informacja", "Brak danych firmy."); });
            }
            break;

        case DBViewDialog::State::HOUSE_CERTIFICATE:
            if(CompanyWidget::isExists()){
                connect(printBtn, &QPushButton::clicked, this, &DBViewDialog::printPayCertificate);
            }
            else{
                connect(printBtn, &QPushButton::clicked, this, [this](){ QMessageBox::information(this, "Informacja", "Brak danych firmy."); });
            }
            break;

        case DBViewDialog::State::FAMILY_CERTIFICATE:
            if(CompanyWidget::isExists()){
                connect(printBtn, &QPushButton::clicked, this, &DBViewDialog::printPayCertificate);
            }
            else{
                connect(printBtn, &QPushButton::clicked, this, [this](){ QMessageBox::information(this, "Informacja", "Brak danych firmy."); });
            }
            break;

        case DBViewDialog::State::MEINTENANCE_CERTIFICATE:
            if(CompanyWidget::isExists()){
                connect(printBtn, &QPushButton::clicked, this, &DBViewDialog::printPayCertificate);
            }
            else{
                connect(printBtn, &QPushButton::clicked, this, [this](){ QMessageBox::information(this, "Informacja", "Brak danych firmy."); });
            }
            break;
    }

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::close);
}


void DBViewDialog::connectUICompanies(){
    switch(state){
        case DBViewDialog::State::ADD_BUY_DOC:
            connect(addBtn, &QPushButton::clicked, this, &DBViewDialog::addBuyDoc);
            break;

        case DBViewDialog::State::COMPENSATION_LIST:
            connect(addBtn, &QPushButton::clicked, this, &DBViewDialog::resetAccount);

            if(CompanyWidget::isExists()){
                connect(printBtn, &QPushButton::clicked, this, &DBViewDialog::printCompensationDoc);
            }
            else{
                connect(printBtn, &QPushButton::clicked, this, [this](){ QMessageBox::information(this, "Informacja", "Brak danych firmy."); });
            }
            break;

        case DBViewDialog::State::COMPANIES_LIST:
            connect(editBtn, &QPushButton::clicked, this, &DBViewDialog::changeCompaniesData);
            connect(deleteBtn, &QPushButton::clicked, this, &DBViewDialog::deleteCompaniesData);
            break;

        case DBViewDialog::State::FINANCE_DOC_LIST:
            if(CompanyWidget::isExists()){
                connect(printBtn, &QPushButton::clicked, this, &DBViewDialog::printFinanceDoc);
            }
            else{
                connect(printBtn, &QPushButton::clicked, this, [this](){ QMessageBox::information(this, "Informacja", "Brak danych firmy."); });
            }

            connect(printLPBtn, &QPushButton::clicked, this, &DBViewDialog::chooseBaseDoc);
            connect(editBtn, &QPushButton::clicked, this, &DBViewDialog::changeFinanceDocsData);
            connect(deleteBtn, &QPushButton::clicked, this, &DBViewDialog::deleteFinanceDocsData);
            break;
    }

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::close);
}


void DBViewDialog::addToLayout(){
    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->setAlignment(Qt::AlignRight);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(headerLabel);
    layout->addWidget(view);

    switch(state){
        case DBViewDialog::State::ADD_CONTRACT:
            btnLayout->addWidget(addBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::ADD_SICKLEAVE:
            btnLayout->addWidget(addBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::ADD_FREELEAVE:
            btnLayout->addWidget(addBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::ADD_JOB_TIME_EVIDENCE:
            btnLayout->addWidget(addBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::EMPLOYEE_LIST:
            btnLayout->addWidget(printBtn);
            btnLayout->addWidget(editBtn);
            btnLayout->addWidget(deleteBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::JOB_CONTRACTS_LIST:
            btnLayout->addWidget(printBtn);
            btnLayout->addWidget(editBtn);
            btnLayout->addWidget(deleteBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::ORDER_CONTRACTS_LIST:
            btnLayout->addWidget(printBtn);
            btnLayout->addWidget(editBtn);
            btnLayout->addWidget(deleteBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::WORK_CONTRACTS_LIST:
            btnLayout->addWidget(printBtn);
            btnLayout->addWidget(editBtn);
            btnLayout->addWidget(deleteBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::SICK_LEAVE_LIST:
            btnLayout->addWidget(printBtn);
            btnLayout->addWidget(editBtn);
            btnLayout->addWidget(deleteBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::FREE_LEAVE_LIST:
            btnLayout->addWidget(printBtn);
            btnLayout->addWidget(editBtn);
            btnLayout->addWidget(deleteBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::JOB_TIME_LIST:
            btnLayout->addWidget(printBtn);
            btnLayout->addWidget(editBtn);
            btnLayout->addWidget(deleteBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::CHOOSE_TO_ANNEX:
            btnLayout->addWidget(addBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::ANNEX_LIST:
            btnLayout->addWidget(printBtn);
            btnLayout->addWidget(editBtn);
            btnLayout->addWidget(deleteBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::COUNT_ORDER_CONTRACT:
            btnLayout->addWidget(addBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::BILLS_LIST:
            btnLayout->addWidget(printLPBtn);
            btnLayout->addWidget(printBtn);
            btnLayout->addWidget(editBtn);
            btnLayout->addWidget(deleteBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::COUNT_WORK_CONTRACT:
            btnLayout->addWidget(addBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::COUNT_JOB_CONTRACT:
            btnLayout->addWidget(addBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::PAY_CERTIFICATE:
            btnLayout->addWidget(addBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::PAY_CERTIFICATE_2:
            btnLayout->addWidget(printBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::HOUSE_CERTIFICATE:
            btnLayout->addWidget(printBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::FAMILY_CERTIFICATE:
            btnLayout->addWidget(printBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::MEINTENANCE_CERTIFICATE:
            btnLayout->addWidget(printBtn);
            btnLayout->addWidget(cancelBtn);
            break;



        case DBViewDialog::State::ADD_BUY_DOC:
            btnLayout->addWidget(addBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::COMPENSATION_LIST:
            btnLayout->addWidget(printBtn);
            btnLayout->addWidget(addBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::COMPANIES_LIST:
            btnLayout->addWidget(editBtn);
            btnLayout->addWidget(deleteBtn);
            btnLayout->addWidget(cancelBtn);
            break;

        case DBViewDialog::State::FINANCE_DOC_LIST:
            if(financeState == FinanceDialog::State::WZ || financeState == FinanceDialog::State::FZ){
                btnLayout->addWidget(printLPBtn);
            }

            if(financeState != FinanceDialog::State::FZ){
                btnLayout->addWidget(printBtn);
            }

            btnLayout->addWidget(editBtn);
            btnLayout->addWidget(deleteBtn);
            btnLayout->addWidget(cancelBtn);
            break;
    }

    layout->addLayout(btnLayout);
    setLayout(layout);
}


void DBViewDialog::checkEmployeeIsWorking(const QString& id){
    QSqlQuery query;
    query.exec("SELECT uop, uz, uod FROM employees WHERE id = " + id);
    if(query.next()){
        if(!query.value(0).toBool() && !query.value(1).toBool() && !query.value(2).toBool()){
            query.exec("UPDATE employees SET activeWork = false, isWorking = 'Nie' WHERE id = " + id);
        }
    }
}



//Connecting Employee Slots--------------------------------------------------
int DBViewDialog::addContract(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        if(contractState == AppState::ContractState::ADD_JOB){
            AddJobDialog* dialog = new AddJobDialog(view->model()->index(view->currentIndex().row(), 0).data().toString(),
                                                    view->model()->index(view->currentIndex().row(), 0).data().toString(),
                                                    AppState::ContractState::ADD_JOB);
            close();
            return dialog->exec();
        }
        else if(contractState == AppState::ContractState::ADD_ORDER){
            AddJobDialog* dialog = new AddJobDialog(view->model()->index(view->currentIndex().row(), 0).data().toString(),
                                                    "",
                                                    AppState::ContractState::ADD_ORDER);
            close();
            return dialog->exec();
        }
        else if(contractState == AppState::ContractState::ADD_WORK){
            AddJobDialog* dialog = new AddJobDialog(view->model()->index(view->currentIndex().row(), 0).data().toString(),
                                                    "",
                                                    AppState::ContractState::ADD_WORK);
            close();
            return dialog->exec();
        }
    }

    QMessageBox::information(this, "Informacja", "Proszę o wybranie pracownika.");
    return 0;
}


int DBViewDialog::addSickLeave(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        AddFreeDialog* dialog = new AddFreeDialog(AddFreeWidget::State::SICK_LEAVE,
                                                  view->model()->index(view->currentIndex().row(), 0).data().toString());
        close();
        return dialog->exec();
    }

    QMessageBox::information(this, "Informacja", "Proszę o wybranie pracownika.");
    return 0;
}


int DBViewDialog::addFreeLeave(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        AddFreeDialog* dialog = new AddFreeDialog(AddFreeWidget::State::FREE_LEAVE,
                                                  view->model()->index(view->currentIndex().row(), 0).data().toString());
        close();
        return dialog->exec();
    }

    QMessageBox::information(this, "Informacja", "Proszę o wybranie pracownika.");
    return 0;
}


int DBViewDialog::changeEmployeeData(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        AddUserDialog* dialog = new AddUserDialog(view->model()->index(view->currentIndex().row(), 0).data().toString());
        close();
        return dialog->exec();
    }

    QMessageBox::information(this, "Informacja", "Proszę o wybranie pracownika.");
    return 0;
}


int DBViewDialog::changeJobContractData(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        AddJobDialog* dialog = new AddJobDialog(view->model()->index(view->currentIndex().row(), 1).data().toString(),
                                                view->model()->index(view->currentIndex().row(), 0).data().toString(),
                                                AppState::ContractState::ADD_JOB, true);
        close();
        return dialog->exec();
    }

    QMessageBox::information(this, "Informacja", "Proszę o wybranie umowy.");
    return 0;
}


int DBViewDialog::changeOrderContractData(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        AddJobDialog* dialog = new AddJobDialog(view->model()->index(view->currentIndex().row(), 1).data().toString(),
                                                view->model()->index(view->currentIndex().row(), 0).data().toString(),
                                                AppState::ContractState::ADD_ORDER, true);
        close();
        return dialog->exec();
    }

    QMessageBox::information(this, "Informacja", "Proszę o wybranie umowy.");
    return 0;
}


int DBViewDialog::changeWorkContractData(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        AddJobDialog* dialog = new AddJobDialog(view->model()->index(view->currentIndex().row(), 1).data().toString(),
                                                view->model()->index(view->currentIndex().row(), 0).data().toString(),
                                                AppState::ContractState::ADD_WORK, true);
        close();
        return dialog->exec();
    }

    QMessageBox::information(this, "Informacja", "Proszę o wybranie umowy.");
    return 0;
}


int DBViewDialog::changeSickLeaveData(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        AddFreeDialog* dialog = new AddFreeDialog(AddFreeWidget::State::SICK_LEAVE,
                                                  view->model()->index(view->currentIndex().row(), 1).data().toString(),
                                                  view->model()->index(view->currentIndex().row(), 0).data().toString(),
                                                  true);
        close();
        return dialog->exec();
    }

    QMessageBox::information(this, "Informacja", "Proszę o wybranie zwolnienia chorobowego.");
    return 0;
}


int DBViewDialog::changeFreeLeaveData(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        AddFreeDialog* dialog = new AddFreeDialog(AddFreeWidget::State::FREE_LEAVE,
                                                  view->model()->index(view->currentIndex().row(), 1).data().toString(),
                                                  view->model()->index(view->currentIndex().row(), 0).data().toString(),
                                                  true);
        close();
        return dialog->exec();
    }

    QMessageBox::information(this, "Informacja", "Proszę o wybranie urlopu.");
    return 0;
}


int DBViewDialog::changeJobTimeData(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        SearchSickDialog* dialog = new SearchSickDialog(SearchSickDialog::State::JOB_TIME_EVIDENCE,
                                                        view->model()->index(view->currentIndex().row(), 1).data().toString(),
                                                        view->model()->index(view->currentIndex().row(), 0).data().toString(),
                                                        true);
        close();
        return dialog->exec();
    }

    QMessageBox::information(this, "Informacja", "Proszę o wybranie ewidencji czasu pracy.");
    return 0;
}


int DBViewDialog::changeOrderBillData(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        OrderBillDialog* dialog = new OrderBillDialog(view->model()->index(view->currentIndex().row(), 0).data().toString(), "", OrderBillDialog::State::ORDER_BILL, true);
        close();
        return dialog->exec();
    }

    QMessageBox::information(this, "Informacja", "Proszę o wybranie rachunku do umowy zlecenie.");
    return 0;
}


int DBViewDialog::changeAnnexData(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        const QString id = view->model()->index(view->currentIndex().row(), 0).data().toString();
        const QString docId = view->model()->index(view->currentIndex().row(), 2).data().toString();
        const QString type = view->model()->index(view->currentIndex().row(), 3).data().toString();
        QString employeeId{};
        QString contractId{};

        if(type == "Umowa o pracę")
            contractState = AppState::ContractState::JOB_ANNEX;
        else if(type == "Umowa zlecenie")
            contractState = AppState::ContractState::ORDER_ANNEX;
        else if(type == "Umowa o dzieło")
            contractState = AppState::ContractState::WORK_ANNEX;

        QSqlQuery query;
        switch(contractState){
            case AppState::ContractState::JOB_ANNEX: query.exec("SELECT employeeId, id FROM jobContracts WHERE docId = " + docId); break;
            case AppState::ContractState::ORDER_ANNEX: query.exec("SELECT employeeId, id FROM orderContracts WHERE docId = " + docId); break;
            case AppState::ContractState::WORK_ANNEX: query.exec("SELECT employeeId, id FROM workContracts WHERE docId = " + docId); break;
        }

        if(query.next()){
            employeeId = query.value(0).toString();
            contractId = query.value(1).toString();
        }

        AddAnnexDialog* dialog = new AddAnnexDialog(contractState, id, employeeId, contractId, true);
        close();
        return dialog->exec();
    }

    QMessageBox::information(this, "Informacja", "Proszę o wybranie aneksu.");
    return 0;
}


void DBViewDialog::deleteEmployeeData(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        const QString employeeID = view->model()->index(view->currentIndex().row(), 0).data().toString();

        QSqlQuery query;
        query.exec("DELETE FROM employees WHERE id = " + employeeID);
        query.exec("DELETE FROM jobContracts WHERE employeeId = " + employeeID);
        query.exec("DELETE FROM orderContracts WHERE employeeId = " + employeeID);
        query.exec("DELETE FROM workContracts WHERE employeeId = " + employeeID);
        query.exec("DELETE FROM sickLeaves WHERE employeeId = " + employeeID);
        query.exec("DELETE FROM freeLeaves WHERE employeeId = " + employeeID);
        query.exec("DELETE FROM jobTimeEvidence WHERE employeeId = " + employeeID);

        view->model()->removeRow(view->currentIndex().row());
        model->select();
        view->setModel(model);

        QMessageBox::information(this, "Sukces!", "Pomyślnie usunięto pracownika.");
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie pracownika.");
    }
}


void DBViewDialog::deleteJobContractData(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        const QString employeeID = view->model()->index(view->currentIndex().row(), 1).data().toString();
        const QString docID = view->model()->index(view->currentIndex().row(), 0).data().toString();

        QSqlQuery query;
        query.exec("SELECT activeWork FROM employees WHERE id = " + employeeID);

        if(query.next()){
            query.exec("DELETE FROM jobContracts WHERE docId = " + docID);
            query.exec("DELETE FROM jobTimeEvidence WHERE employeeId = " + employeeID);
            query.exec("UPDATE employees SET uop = false WHERE id = " + employeeID);
            checkEmployeeIsWorking(employeeID);

            view->model()->removeRow(view->currentIndex().row());
            model->select();
            view->setModel(model);

            QMessageBox::information(this, "Sukces!", "Pomyślnie usunięto umowę o pracę.");
        }
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie pracownika.");
    }
}


void DBViewDialog::deleteOrderContractData(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        const QString employeeID = view->model()->index(view->currentIndex().row(), 1).data().toString();
        const QString docID = view->model()->index(view->currentIndex().row(), 0).data().toString();

        QSqlQuery query;
        query.exec("SELECT activeWork FROM employees WHERE id = " + employeeID);

        if(query.next()){
            query.exec("DELETE FROM orderContracts WHERE docId = " + docID);
            query.exec("DELETE FROM jobTimeEvidence WHERE employeeId = " + employeeID);
            query.exec("DELETE FROM bills WHERE type = 'uz' AND contractId = " + docID);
            query.exec("UPDATE employees SET uz = false WHERE id = " + employeeID);
            checkEmployeeIsWorking(employeeID);

            view->model()->removeRow(view->currentIndex().row());
            model->select();
            view->setModel(model);

            QMessageBox::information(this, "Sukces!", "Pomyślnie usunięto umowę zlecenie.");
        }
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie pracownika.");
    }
}


void DBViewDialog::deleteWorkContractData(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        const QString employeeID = view->model()->index(view->currentIndex().row(), 1).data().toString();
        const QString docID = view->model()->index(view->currentIndex().row(), 0).data().toString();

        QSqlQuery query;
        query.exec("SELECT activeWork FROM employees WHERE id = " + employeeID);

        if(query.next()){
            query.exec("DELETE FROM workContracts WHERE docId = " + docID);
            query.exec("DELETE FROM bills WHERE type = 'ud' AND contractId = " + docID);
            query.exec("UPDATE employees SET uod = false WHERE id = " + employeeID);
            checkEmployeeIsWorking(employeeID);

            view->model()->removeRow(view->currentIndex().row());
            model->select();
            view->setModel(model);

            QMessageBox::information(this, "Sukces!", "Pomyślnie usunięto umowę zlecenie.");
        }
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie pracownika.");
    }
}


void DBViewDialog::deleteSickLeaveData(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        QSqlQuery query;
        query.exec("DELETE FROM sickLeaves WHERE id = " + view->model()->index(view->currentIndex().row(), 0).data().toString());

        view->model()->removeRow(view->currentIndex().row());
        model->select();
        view->setModel(model);

        QMessageBox::information(this, "Sukces!", "Pomyślnie usunięto zwolnienie chorobowe.");
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie zwolnienia chorobowego.");
    }
}


void DBViewDialog::deleteFreeLeaveData(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        QSqlQuery query;
        query.exec("DELETE FROM freeLeaves WHERE id = " + view->model()->index(view->currentIndex().row(), 0).data().toString());

        view->model()->removeRow(view->currentIndex().row());
        model->select();
        view->setModel(model);

        QMessageBox::information(this, "Sukces!", "Pomyślnie usunięto urlop.");
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie urlopu.");
    }
}


void DBViewDialog::deleteJobTimeData(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        QSqlQuery query;
        query.exec("DELETE FROM bills WHERE type = 'uz' AND jobEvidenceId = " + view->model()->index(view->currentIndex().row(), 0).data().toString());
        query.exec("DELETE FROM jobTimeEvidence WHERE id = " + view->model()->index(view->currentIndex().row(), 0).data().toString());

        view->model()->removeRow(view->currentIndex().row());
        model->select();
        view->setModel(model);

        QMessageBox::information(this, "Sukces!", "Pomyślnie usunięto ewidencje czasu pracy.");
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie ewidencji czasu pracy.");
    }
}


void DBViewDialog::deleteAnnexData(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        QSqlQuery query;
        query.exec("DELETE FROM annexes WHERE id = " + view->model()->index(view->currentIndex().row(), 0).data().toString());

        view->model()->removeRow(view->currentIndex().row());
        model->select();
        view->setModel(model);

        QMessageBox::information(this, "Sukces!", "Pomyślnie usunięto aneks.");
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie aneksu.");
    }
}


void DBViewDialog::deleteOrderBillData(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        QSqlQuery query;
        query.exec("SELECT type FROM bills WHERE id = " + view->model()->index(view->currentIndex().row(), 0).data().toString());
        if(query.next()){
            if(query.value(0).toString() != "ud")
                query.exec("UPDATE jobTimeEvidence SET hasBill = false WHERE id = " + view->model()->index(view->currentIndex().row(), 23).data().toString());
            else if(query.value(0).toString() == "ud")
                query.exec("UPDATE workContracts SET hasBill = false WHERE docId = " + view->model()->index(view->currentIndex().row(), 22).data().toString());
        }

        query.exec("DELETE FROM bills WHERE id = " + view->model()->index(view->currentIndex().row(), 0).data().toString());
        view->model()->removeRow(view->currentIndex().row());
        model->select();
        view->setModel(model);

        QMessageBox::information(this, "Sukces!", "Pomyślnie usunięto rachunek do umowy zlecenie.");
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie rachunku.");
    }
}


void DBViewDialog::printJobContract(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        const QString employeeID = view->model()->index(view->currentIndex().row(), 1).data().toString();
        const QString docID = view->model()->index(view->currentIndex().row(), 0).data().toString();

        PrintDocuments document;
        document.setEmployeeID(employeeID);
        document.setDocumentID(docID);
        document.loadDocuments();
        document.printJobContract();

        QMessageBox::information(this, "Sukces!", "Pomyślnie zapisano umowę do pliku.");
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie pracownika.");
    }
}


void DBViewDialog::printOrderContract(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        const QString employeeID = view->model()->index(view->currentIndex().row(), 1).data().toString();
        const QString docID = view->model()->index(view->currentIndex().row(), 0).data().toString();

        PrintDocuments document;
        document.setEmployeeID(employeeID);
        document.setDocumentID(docID);
        document.loadDocuments();
        document.printOrderContract();

        QMessageBox::information(this, "Sukces!", "Pomyślnie zapisano umowę do pliku.");
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie pracownika.");
    }
}


void DBViewDialog::printWorkContract(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        const QString employeeID = view->model()->index(view->currentIndex().row(), 1).data().toString();
        const QString docID = view->model()->index(view->currentIndex().row(), 0).data().toString();

        PrintDocuments document;
        document.setEmployeeID(employeeID);
        document.setDocumentID(docID);
        document.loadDocuments();
        document.printWorkContract();

        QMessageBox::information(this, "Sukces!", "Pomyślnie zapisano umowę do pliku.");
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie pracownika.");
    }
}


void DBViewDialog::printAnnex(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        const QString docID = view->model()->index(view->currentIndex().row(), 0).data().toString();
        const QString contractID = view->model()->index(view->currentIndex().row(), 2).data().toString();
        const QString type = view->model()->index(view->currentIndex().row(), 3).data().toString();
        QString employeeId{};
        QString contractId{};
        QSqlQuery query;

        if(type == "Umowa o pracę")
            query.exec("SELECT employeeId FROM jobContracts WHERE docId = " + contractID);
        else if(type == "Umowa zlecenie")
            query.exec("SELECT employeeId FROM orderContracts WHERE docId = " + contractID);
        else if(type == "Umowa o dzieło")
            query.exec("SELECT employeeId FROM workContracts WHERE docId = " + contractID);

        if(query.next())
            employeeId = query.value(0).toString();

        PrintDocuments document;
        document.setDocumentID(docID);
        document.setEmployeeID(employeeId);
        document.loadDocuments();
        document.printAnnex();

        QMessageBox::information(this, "Sukces!", "Pomyślnie zapisano aneks do pliku.");
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie aneksu.");
    }
}


void DBViewDialog::printOrderBill(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        QString employeeId;
        const QString contractId = view->model()->index(view->currentIndex().row(), 22).data().toString();
        const QString jobEvidenceId = view->model()->index(view->currentIndex().row(), 23).data().toString();
        const QString billId = view->model()->index(view->currentIndex().row(), 0).data().toString();
        int hoursSum{0}, daysSum{0}, freeDaysSum{0}, sickDaysSum{0};

        QSqlQuery query;
        query.exec("SELECT type FROM bills WHERE id = " + billId);
        if(query.next()){
            if(query.value(0).toString() == "uz"){
                query.exec("SELECT employeeId FROM orderContracts WHERE docId = " + contractId);
                if(query.next()){
                    employeeId = query.value(0).toString();
                }
            }
            else if(query.value(0).toString() == "ud"){
                query.exec("SELECT employeeId FROM workContracts WHERE docId = " + contractId);
                if(query.next()){
                    employeeId = query.value(0).toString();
                }
            }
            else if(query.value(0).toString() == "uop"){
                query.exec("SELECT employeeId FROM jobContracts WHERE docId = " + contractId);
                if(query.next()){
                    employeeId = query.value(0).toString();
                }
            }
        }

        query.exec("SELECT sum, daysSum, freeDaysSum, sickDaysSum FROM jobTimeEvidence WHERE id = " + jobEvidenceId);
        if(query.next()){
            hoursSum = query.value(0).toInt();
            daysSum = query.value(1).toInt();
            freeDaysSum = query.value(2).toInt();
            sickDaysSum = query.value(3).toInt();
        }

        PrintDocuments document;
        document.setEmployeeID(employeeId);
        document.setDocumentID(contractId);
        document.setOrderBillID(billId);
        document.loadDocuments();
        document.printOrderBill(hoursSum, daysSum, freeDaysSum, sickDaysSum);

        QMessageBox::information(this, "Sukces!", "Pomyślnie zapisano rachunek do pliku.");
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie rachunku do druku.");
    }
}


void DBViewDialog::printPayCertificate(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        const QString contractId = view->model()->index(view->currentIndex().row(), 22).data().toString();
        QSqlQuery query;
        QVector<Data::BillRecord> records;
        QModelIndexList list = select->selectedIndexes();

        for(int i = 0; i < list.size(); ++i){
            Data::BillRecord record;
            record.rewardBrutto = view->model()->index(list.at(i).row(), 2).data().toFloat();
            record.iSum = view->model()->index(list.at(i).row(), 6).data().toFloat();

            if(state != DBViewDialog::State::HOUSE_CERTIFICATE)
                record.rate = view->model()->index(list.at(i).row(), 13).data().toFloat();
            else if(state == DBViewDialog::State::HOUSE_CERTIFICATE)
                record.rate = view->model()->index(list.at(i).row(), 11).data().toFloat();

            if(state == DBViewDialog::State::PAY_CERTIFICATE_2)
                record.rewardNetto = view->model()->index(list.at(i).row(), 19).data().toFloat();
            else
                record.rewardNetto = view->model()->index(list.at(i).row(), 12).data().toFloat();

            record.payType = view->model()->index(list.at(i).row(), 25).data().toString();
            record.date = view->model()->index(list.at(i).row(), 21).data().toDate();
            record.insurance = view->model()->index(list.at(i).row(), 14).data().toFloat();
            record.incomeCost = view->model()->index(list.at(i).row(), 11).data().toFloat();

            if(record.payType != "ud"){
                const QString jobEvidenceId = view->model()->index(list.at(i).row(), 24).data().toString();
                query.exec("SELECT month, year FROM jobTimeEvidence WHERE id = " + jobEvidenceId);
                if(query.next()){
                    record.docId = query.value(0).toString() + ", " + query.value(1).toString();
                }
            }
            else{
                QDate createDate = view->model()->index(list.at(i).row(), 21).data().toDate();
                record.docId = createDate.toString("MMMM yyyy");
            }

            records.push_back(record);
        }

        PrintDocuments document;
        document.setEmployeeID(empId);
        document.setDocumentID(contractId);
        document.loadDocuments();

        if(state == DBViewDialog::State::PAY_CERTIFICATE_2)
            document.printPayCertificate(records);
        else if(state == DBViewDialog::State::HOUSE_CERTIFICATE)
            document.printHouseCertificate(records);
        else if(state == DBViewDialog::State::FAMILY_CERTIFICATE)
            document.printFamilyOrMaintanceCertificate(records.at(0), 0);
        else if(state == DBViewDialog::State::MEINTENANCE_CERTIFICATE)
            document.printFamilyOrMaintanceCertificate(records.at(0), 1);

        QMessageBox::information(this, "Sukces!", "Pomyślnie zapisano zaświadczenie o zatrudnieniu do pliku.");
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie rozliczeń do druku.");
    }
}


int DBViewDialog::createSickEvidence(){
    if(view->model()->rowCount() != 0){
        SearchSickDialog* dialog = new SearchSickDialog(SearchSickDialog::State::SICK_LEAVE);
        close();
        return dialog->exec();
    }

    QMessageBox::information(this, "Informacja", "Brak zwolnień chorobowych.");
    return 0;
}


int DBViewDialog::createFreeEvidence(){
    if(view->model()->rowCount() != 0){
        SearchSickDialog* dialog = new SearchSickDialog(SearchSickDialog::State::FREE_LEAVE);
        close();
        return dialog->exec();
    }

    QMessageBox::information(this, "Informacja", "Brak urlopów.");
    return 0;
}


int DBViewDialog::createJobTimeEvidence(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        SearchSickDialog* dialog = new SearchSickDialog(SearchSickDialog::State::JOB_TIME_EVIDENCE,
                                                        view->model()->index(view->currentIndex().row(), 0).data().toString());
        close();
        return dialog->exec();
    }

    QMessageBox::information(this, "Informacja", "Proszę o wybranie pracownika.");
    return 0;
}


int DBViewDialog::searchJobTimeEvidences(){
    if(view->model()->rowCount() != 0){
        SearchSickDialog* dialog = new SearchSickDialog(SearchSickDialog::State::SEARCH_JOB_TIME);
        close();
        return dialog->exec();
    }

    QMessageBox::information(this, "Informacja", "Brak ewidencji czasu pracy.");
    return 0;
}


int DBViewDialog::createAnnex(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        const QString docId = view->model()->index(view->currentIndex().row(), 0).data().toString();
        const QString employeeId = view->model()->index(view->currentIndex().row(), 1).data().toString();
        const QString textId = view->model()->index(view->currentIndex().row(), 2).data().toString();

        AddAnnexDialog* dialog = new AddAnnexDialog(contractState, docId, employeeId, textId);
        close();
        return dialog->exec();
    }

    QMessageBox::information(this, "Informacja", "Proszę o wybranie umowy.");
    return 0;
}


int DBViewDialog::createOrderBill(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        bool hasBill {false};
        QSqlQuery query;
        query.exec("SELECT hasBill FROM jobTimeEvidence WHERE id = " + view->model()->index(view->currentIndex().row(), 0).data().toString());
        if(query.next()){
            hasBill = query.value(0).toBool();
        }

        if(!hasBill){
            const QString evidenceId = view->model()->index(view->currentIndex().row(), 0).data().toString();
            const QString docId = view->model()->index(view->currentIndex().row(), 14).data().toString();

            OrderBillDialog* dialog = new OrderBillDialog(docId, evidenceId, OrderBillDialog::State::ORDER_BILL);
            close();
            return dialog->exec();
        }
        else{
            QMessageBox::information(this, "Informacja", "Wybrana ewidencja czasu pracy posiada już rozliczenie.");
        }
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie ewidencji czasu pracy.");
        return 0;
    }
    return 0;
}


int DBViewDialog::createWorkBill(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        bool hasBill {false};
        QSqlQuery query;
        query.exec("SELECT hasBill FROM workContracts WHERE docId = " + view->model()->index(view->currentIndex().row(), 0).data().toString());
        if(query.next()){
            hasBill = query.value(0).toBool();
        }

        if(!hasBill){
            const QString docId = view->model()->index(view->currentIndex().row(), 0).data().toString();

            OrderBillDialog* dialog = new OrderBillDialog(docId, "NONE", OrderBillDialog::State::WORK_BILL);
            close();
            return dialog->exec();
        }
        else{
            QMessageBox::information(this, "Informacja", "Wybrana umowa o dzieło posiada już rozliczenie.");
        }
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie umowa o dzieło.");
        return 0;
    }
    return 0;
}


int DBViewDialog::createJobBill(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        bool hasBill {false};
        QSqlQuery query;
        query.exec("SELECT hasBill FROM jobTimeEvidence WHERE id = " + view->model()->index(view->currentIndex().row(), 0).data().toString());
        if(query.next()){
            hasBill = query.value(0).toBool();
        }

        if(!hasBill){
            const QString evidenceId = view->model()->index(view->currentIndex().row(), 0).data().toString();
            const QString docId = view->model()->index(view->currentIndex().row(), 14).data().toString();

            OrderBillDialog* dialog = new OrderBillDialog(docId, evidenceId, OrderBillDialog::State::JOB_BILL);
            close();
            return dialog->exec();
        }
        else{
            QMessageBox::information(this, "Informacja", "Wybrana ewidencja czasu pracy posiada już rozliczenie.");
        }
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie ewidencji czasu pracy.");
        return 0;
    }
    return 0;
}


int DBViewDialog::createJobBillList(){
    if(view->model()->rowCount() != 0){
        bool hasLP{false};
        QSqlQuery query;

        query.exec("SELECT id FROM bills WHERE type = 'uop'");
        if(query.next()){
            hasLP = true;
        }

        if(hasLP){
            SearchSickDialog* dialog = new SearchSickDialog(SearchSickDialog::State::CREATE_JOB_BILL_LIST);
            close();
            return dialog->exec();
        }
        else{
            QMessageBox::information(this, "Ups...", "Brak indywidualnych list płac.");
            return 0;
        }
    }

    QMessageBox::information(this, "Informacja", "Brak rozliczeń.");
    return 0;
}


int DBViewDialog::createPayCertificate(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        qDebug() << certificateIndex;

        if(certificateIndex == 0){
            DBViewDialog* dialog;
            dialog = new DBViewDialog(DBViewDialog::State::PAY_CERTIFICATE_2,
                                      view->model()->index(view->currentIndex().row(), 0).data().toString());
            close();
            return dialog->exec();
        }
        else if(certificateIndex == 1){
            DBViewDialog* dialog;
            dialog = new DBViewDialog(DBViewDialog::State::FAMILY_CERTIFICATE,
                                      view->model()->index(view->currentIndex().row(), 0).data().toString());
            close();
            return dialog->exec();
        }
        else if(certificateIndex == 2){
            DBViewDialog* dialog;
            dialog = new DBViewDialog(DBViewDialog::State::MEINTENANCE_CERTIFICATE,
                                      view->model()->index(view->currentIndex().row(), 0).data().toString());
            close();
            return dialog->exec();
        }
        else if(certificateIndex == 3){
            DBViewDialog* dialog;
            dialog = new DBViewDialog(DBViewDialog::State::HOUSE_CERTIFICATE,
                                      view->model()->index(view->currentIndex().row(), 0).data().toString());
            close();
            return dialog->exec();
        }

        return 0;
    }

    QMessageBox::information(this, "Informacja", "Proszę o wybranie pracownika.");
    return 0;
}


int DBViewDialog::openEmployeeStory(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        EmployeeInfoDialog* dialog = new EmployeeInfoDialog(view->model()->index(view->currentIndex().row(), 0).data().toString());
        close();
        return dialog->exec();
    }

    QMessageBox::information(this, "Informacja", "Proszę o wybranie pracownika.");
    return 0;
}



//Connecting Customers Slots--------------------------------------------------
int DBViewDialog::addBuyDoc(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        FinanceDialog* dialog = new FinanceDialog(financeState,
                                                  view->model()->index(view->currentIndex().row(), 0).data().toString());
        close();
        return dialog->exec();
    }

    QMessageBox::information(this, "Informacja", "Proszę o wybranie firmy.");
    return 0;
}


int DBViewDialog::chooseBaseDoc(){
    QItemSelectionModel* select = view->selectionModel();
    if(financeState == FinanceDialog::State::FZ && select->hasSelection()){
        FinanceDialog* dialog = new FinanceDialog(FinanceDialog::State::PZ,
                                                  view->model()->index(view->currentIndex().row(), 0).data().toString(),
                                                  false,
                                                  true);
        close();
        return dialog->exec();
    }
    else if(financeState == FinanceDialog::State::WZ && select->hasSelection()){
        FinanceDialog* dialog = new FinanceDialog(FinanceDialog::State::FS,
                                                  view->model()->index(view->currentIndex().row(), 0).data().toString(),
                                                  false,
                                                  true);
        close();
        return dialog->exec();
    }
    else
        QMessageBox::information(this, "Informacja", "Proszę o wybranie dokumentu bazowego.");

    return 0;
}


int DBViewDialog::changeCompaniesData(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        const QString companyId = view->model()->index(view->currentIndex().row(), 0).data().toString();

        CompanyDialog* dialog = new CompanyDialog(true, companyId);
        close();
        return dialog->exec();
    }

    QMessageBox::information(this, "Informacja", "Proszę o wybranie firmy.");
    return 0;
}


int DBViewDialog::changeFinanceDocsData(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        FinanceDialog* dialog = new FinanceDialog(financeState, view->model()->index(view->currentIndex().row(), 0).data().toString(), true);
        close();
        return dialog->exec();
    }

    QMessageBox::information(this, "Informacja", "Proszę o wybranie firmy.");
    return 0;
}


void DBViewDialog::deleteCompaniesData(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        QSqlQuery query;
        query.exec("DELETE FROM companies WHERE id = " + view->model()->index(view->currentIndex().row(), 0).data().toString());
        query.exec("DELETE FROM finance WHERE companyId = " + view->model()->index(view->currentIndex().row(), 0).data().toString());
        query.exec("DELETE FROM account WHERE id = " + view->model()->index(view->currentIndex().row(), 0).data().toString());

        view->model()->removeRow(view->currentIndex().row());
        model->select();
        view->setModel(model);

        QMessageBox::information(this, "Sukces!", "Pomyślnie usunięto firmę.");
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie firmy.");
    }
}


void DBViewDialog::deleteFinanceDocsData(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        QSqlQuery query;
        query.exec("DELETE FROM finance WHERE id = " + view->model()->index(view->currentIndex().row(), 0).data().toString());

        view->model()->removeRow(view->currentIndex().row());
        model->select();
        view->setModel(model);

        QMessageBox::information(this, "Sukces!", "Pomyślnie usunięto dokument.");
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie dokumentu.");
    }
}


void DBViewDialog::printFinanceDoc(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        const QString id = view->model()->index(view->currentIndex().row(), 0).data().toString();
        const QString compId = view->model()->index(view->currentIndex().row(), 18).data().toString();

        PrintDocuments document;
        document.setDocumentID(id);
        document.setEmployeeID(compId);
        document.loadDocuments();
        document.printSellDoc(financeState);

        QMessageBox::information(this, "Sukces!", "Pomyślnie zapisano dokument do pliku.");
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie dokumentu.");
    }
}


void DBViewDialog::printCompensationDoc(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        const QString id = view->model()->index(view->currentIndex().row(), 0).data().toString();
        float balance{};
        QSqlQuery query;
        query.exec("SELECT balance FROM account WHERE id = " + id);
        if(query.next()){
            balance = query.value(0).toFloat();
        }

        if(balance != 0.0f){
            PrintDocuments document;
            document.setDocumentID(id);
            document.loadDocuments();
            document.printCompensationDoc();

            QMessageBox::information(this, "Sukces!", "Pomyślnie zapisano Kompensatę do pliku.");
        }
        else{
            QMessageBox::information(this, "Ups!", "Druk kompensaty jest niemożliwy, gdy saldo firmy wynosi 0zł.");
        }
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie dokumentu.");
    }
}


void DBViewDialog::resetAccount(){
    QItemSelectionModel* select = view->selectionModel();
    if(select->hasSelection()){
        const QString id = view->model()->index(view->currentIndex().row(), 0).data().toString();
        QSqlQuery query;
        query.prepare("UPDATE account SET balance = :balance, sellDocId = :sellDocId, sellDocDate = :sellDocDate, sellDocBrutto = :sellDocBrutto, buyDocId = :buyDocId, buyDocDate = :buyDocDate, buyDocBrutto = :buyDocBrutto WHERE id = " + id);
        query.bindValue(":balance", 0.0f);
        query.bindValue(":sellDocId", " ");
        query.bindValue(":sellDocDate", " ");
        query.bindValue(":sellDocBrutto", " ");
        query.bindValue(":buyDocId", " ");
        query.bindValue(":buyDocDate", " ");
        query.bindValue(":buyDocBrutto", " ");
        query.exec();

        model->select();
        view->setModel(model);

        QMessageBox::information(this, "Informacja", "Pomyślnie zresetowano konto.");
    }
    else{
        QMessageBox::information(this, "Informacja", "Proszę o wybranie konta.");
    }
}
