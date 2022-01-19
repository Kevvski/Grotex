#include "cadrewidget.h"

CadreWidget::CadreWidget(QWidget *parent) : QWidget(parent)
{
    initializeUI();
    addToLayout();
    connectUI();
}


void CadreWidget::initializeUI(){
    const QSize buttonSize {240, 150};

    addEmployeeBtn = new ImageButton("Dodaj Pracownika");
    addEmployeeBtn->setButtonImage("employeeImg.png");
    addEmployeeBtn->setFixedSize(buttonSize);
    addEmployeeBtn->create();

    addJobBtn = new ImageButton("Dodaj Umowę o Pracę");
    addJobBtn->setButtonImage("contractImg.png");
    addJobBtn->setFixedSize(buttonSize);
    addJobBtn->create();

    addOrderBtn = new ImageButton("Dodaj Umowę Zlecenie");
    addOrderBtn->setButtonImage("contractImg.png");
    addOrderBtn->setFixedSize(buttonSize);
    addOrderBtn->create();

    addWorkBtn = new ImageButton("Dodaj Umowę o Dzieło");
    addWorkBtn->setButtonImage("contractImg.png");
    addWorkBtn->setFixedSize(buttonSize);
    addWorkBtn->create();

    jobTimeBtn = new ImageButton("Dodaj Ew. Czasu Pracy");
    jobTimeBtn->setButtonImage("timeListImg.png");
    jobTimeBtn->setFixedSize(buttonSize);
    jobTimeBtn->create();

    addLeaveBtn = new ImageButton("Dodaj zwolnienie chorobowe");
    addLeaveBtn->setButtonImage("medicalDocImg.png");
    addLeaveBtn->setFixedSize(buttonSize);
    addLeaveBtn->create()
;
    addVacationBtn = new ImageButton("Dodaj Urlop");
    addVacationBtn->setButtonImage("freeTimeImg.png");
    addVacationBtn->setFixedSize(buttonSize);
    addVacationBtn->create();

    createAnnexBtn = new ImageButton("Stwórz Aneks");
    createAnnexBtn->setButtonImage("timeListImg.png");
    createAnnexBtn->setFixedSize(buttonSize);
    createAnnexBtn->create();

    countOrderBtn = new ImageButton("Rozlicz Umowę Zlecenie");
    countOrderBtn->setButtonImage("payListImg.png");
    countOrderBtn->setFixedSize(buttonSize);
    countOrderBtn->create();

    countWorkBtn = new ImageButton("Rozlicz Umowę o Dzieło");
    countWorkBtn->setButtonImage("payListImg.png");
    countWorkBtn->setFixedSize(buttonSize);
    countWorkBtn->create();

    createPaymentListBtn = new ImageButton("Rozlicz Umowę o Pracę");
    createPaymentListBtn->setButtonImage("payListImg.png");
    createPaymentListBtn->setFixedSize(buttonSize);
    createPaymentListBtn->create();

    employeesListBtn = new ImageButton("Spis Dokumentów");
    employeesListBtn->setButtonImage("employeesListImg.png");
    employeesListBtn->setFixedSize(buttonSize);
    employeesListBtn->create();
}


void CadreWidget::addToLayout(){
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(0);

    QHBoxLayout* firstRowLayout = new QHBoxLayout;
    firstRowLayout->addWidget(addEmployeeBtn);
    firstRowLayout->addWidget(addJobBtn);
    firstRowLayout->addWidget(addOrderBtn);
    firstRowLayout->addWidget(addWorkBtn);

    QHBoxLayout* secondRowLayout = new QHBoxLayout;
    secondRowLayout->addWidget(jobTimeBtn);
    secondRowLayout->addWidget(addLeaveBtn);
    secondRowLayout->addWidget(addVacationBtn);
    secondRowLayout->addWidget(createAnnexBtn);

    QHBoxLayout* thirdRowLayout = new QHBoxLayout;
    thirdRowLayout->addWidget(countOrderBtn);
    thirdRowLayout->addWidget(countWorkBtn);
    thirdRowLayout->addWidget(createPaymentListBtn);
    thirdRowLayout->addWidget(employeesListBtn);

    layout->addLayout(firstRowLayout);
    layout->addLayout(secondRowLayout);
    layout->addLayout(thirdRowLayout);

    setLayout(layout);
}


void CadreWidget::connectUI(){
    connect(addEmployeeBtn, &QPushButton::clicked, this, &CadreWidget::addUser);
    connect(addJobBtn, &QPushButton::clicked, this, &CadreWidget::addJobContract);
    connect(addOrderBtn, &QPushButton::clicked, this, &CadreWidget::addOrderContract);
    connect(addWorkBtn, &QPushButton::clicked, this, &CadreWidget::addWorkContract);
    connect(employeesListBtn, &QPushButton::clicked, this, &CadreWidget::showEmployeesList);
    connect(addLeaveBtn, &QPushButton::clicked, this, &CadreWidget::addSickLeave);
    connect(addVacationBtn, &QPushButton::clicked, this, &CadreWidget::addFreeLeave);
    connect(createAnnexBtn, &QPushButton::clicked, this, &CadreWidget::createAnnex);
    connect(countOrderBtn, &QPushButton::clicked, this, &CadreWidget::countOrderContract);
    connect(countWorkBtn, &QPushButton::clicked, this, &CadreWidget::countWorkContract);
    connect(createPaymentListBtn, &QPushButton::clicked, this, &CadreWidget::countJobContract);
    connect(jobTimeBtn, &QPushButton::clicked, this, &CadreWidget::addJobTimeEvidence);
}


//Connecting Slots--------------------------------------------------
int CadreWidget::addUser(){
    if(DBManager::isConnected()){
        AddUserDialog* dialog = new AddUserDialog;
        return dialog->exec();
    }
    else{
        QMessageBox::warning(this, "Informacja", "Proszę najpierw otworzyć bazę danych.");
    }

    return 0;
}


int CadreWidget::addJobContract(){
    if(DBManager::isConnected()){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::ADD_CONTRACT, AppState::ContractState::ADD_JOB);
        return dialog->exec();
    }
    else{
        QMessageBox::warning(this, "Informacja", "Proszę najpierw otworzyć bazę danych.");
    }

    return 0;
}


int CadreWidget::addOrderContract(){
    if(DBManager::isConnected()){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::ADD_CONTRACT, AppState::ContractState::ADD_ORDER);
        return dialog->exec();
    }
    else{
        QMessageBox::warning(this, "Informacja", "Proszę najpierw otworzyć bazę danych.");
    }

    return 0;
}

int CadreWidget::addWorkContract(){
    if(DBManager::isConnected()){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::ADD_CONTRACT, AppState::ContractState::ADD_WORK);
        return dialog->exec();
    }
    else{
        QMessageBox::warning(this, "Informacja", "Proszę najpierw otworzyć bazę danych.");
    }

    return 0;
}


int CadreWidget::addSickLeave(){
    if(DBManager::isConnected()){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::ADD_SICKLEAVE);
        return dialog->exec();
    }
    else{
        QMessageBox::warning(this, "Informacja", "Proszę najpierw otworzyć bazę danych.");
    }

    return 0;
}


int CadreWidget::addFreeLeave(){
    if(DBManager::isConnected()){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::ADD_FREELEAVE);
        return dialog->exec();
    }
    else{
        QMessageBox::warning(this, "Informacja", "Proszę najpierw otworzyć bazę danych.");
    }

    return 0;
}


int CadreWidget::createAnnex(){
    if(DBManager::isConnected()){
        ChooseFromDialog* dialog = new ChooseFromDialog;
        return dialog->exec();
    }
    else{
        QMessageBox::warning(this, "Informacja", "Proszę najpierw otworzyć bazę danych.");
    }

    return 0;
}


int CadreWidget::addJobTimeEvidence(){
    if(DBManager::isConnected()){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::ADD_JOB_TIME_EVIDENCE);
        return dialog->exec();
    }
    else{
        QMessageBox::warning(this, "Informacja", "Proszę najpierw otworzyć bazę danych.");
    }

    return 0;
}


int CadreWidget::showEmployeesList(){
    if(DBManager::isConnected()){
        DocListDialog* dialog = new DocListDialog;
        return dialog->exec();
    }
    else{
        QMessageBox::warning(this, "Informacja", "Proszę najpierw otworzyć bazę danych.");
    }

    return 0;
}


int CadreWidget::countOrderContract(){
    if(DBManager::isConnected()){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::COUNT_ORDER_CONTRACT);
        return dialog->exec();
    }
    else{
        QMessageBox::warning(this, "Informacja", "Proszę najpierw otworzyć bazę danych.");
    }

    return 0;
}


int CadreWidget::countWorkContract(){
    if(DBManager::isConnected()){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::COUNT_WORK_CONTRACT);
        return dialog->exec();
    }
    else{
        QMessageBox::warning(this, "Informacja", "Proszę najpierw otworzyć bazę danych.");
    }

    return 0;
}


int CadreWidget::countJobContract(){
    if(DBManager::isConnected()){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::COUNT_JOB_CONTRACT);
        return dialog->exec();
    }
    else{
        QMessageBox::warning(this, "Informacja", "Proszę najpierw otworzyć bazę danych.");
    }

    return 0;
}
