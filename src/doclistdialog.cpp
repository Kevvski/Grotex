#include "doclistdialog.h"

DocListDialog::DocListDialog()
{
    setWindowTitle("KadriX - Spisy dokumentów");
    setStyleSheet("background-color: white;");
    initializeUI();
    addToLayout();
    connectUI();
}


void DocListDialog::initializeUI(){
    const QSize size { 200, 60 };

    headerLabel = new QLabel("Spisy:");
    headerLabel->setStyleSheet("QLabel{ font-size: 25px; font-weight: bold; }");
    headerLabel->setAlignment(Qt::AlignCenter);

    docsBox = new QComboBox;
    docsBox->setFixedWidth(350);
    docsBox->setStyleSheet("QComboBox{ font-size: 15px; border: 2px solid #1c45a6; border-radius: 8px; padding: 5px; }");
    if(AppState::getState() == AppState::State::EMPLOYEES){
        docsBox->addItem("Pracowników");
        docsBox->addItem("Umów o pracę");
        docsBox->addItem("Umów zlecenie");
        docsBox->addItem("Umów o dzieło");
        docsBox->addItem("Zwolnień chorobowych");
        docsBox->addItem("Urlopów");
        docsBox->addItem("Ew. czasu pracy");
        docsBox->addItem("Aneksów");
        docsBox->addItem("Rozliczenia");
        docsBox->addItem("Drukuj zaś. o zatrudnieniu");
        docsBox->addItem("Drukuj zaś. o zar. (Rodzinne)");
        docsBox->addItem("Drukuj zaś. o zar. (Fundusz Alimentacyjny)");
        docsBox->addItem("Drukuj zaś. o zar. (Dodatek Mieszkaniowy)");
    }
    else if(AppState::getState() == AppState::State::CUSTOMERS){
        docsBox->addItem("Firmy");
        docsBox->addItem("Faktur zakupu");
        docsBox->addItem("Faktur sprzedaży");
        docsBox->addItem("Wydań zewnętrznych");
        docsBox->addItem("Przyjęć zewnętrznych");
    }

    openBtn = new QPushButton("Otwórz");
    openBtn->setStyleSheet(Data::defaultButtonSheet);
    openBtn->setFixedSize(Data::defaultButtonSize);

    cancelBtn = new QPushButton("Wróć");
    cancelBtn->setStyleSheet(Data::defaultButtonSheet);
    cancelBtn->setFixedSize(Data::defaultButtonSize);
}


void DocListDialog::addToLayout(){
    QVBoxLayout* layout = new QVBoxLayout;
    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->setAlignment(Qt::AlignRight);
    btnLayout->addWidget(openBtn);
    btnLayout->addWidget(cancelBtn);

    layout->addWidget(headerLabel);
    layout->addWidget(docsBox);
    layout->addLayout(btnLayout);

    setLayout(layout);
}


void DocListDialog::connectUI(){
    if(AppState::getState() == AppState::State::EMPLOYEES)
        connect(openBtn, &QPushButton::clicked, this, &DocListDialog::openListEmployees);
    else if(AppState::getState() == AppState::State::CUSTOMERS)
        connect(openBtn, &QPushButton::clicked, this, &DocListDialog::openListCustomers);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::close);
}



//Connecting Slots--------------------------------------------------
int DocListDialog::openListEmployees(){
    if(docsBox->currentIndex() == 0){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::EMPLOYEE_LIST);
        close();
        return dialog->exec();
    }
    else if(docsBox->currentIndex() == 1){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::JOB_CONTRACTS_LIST);
        close();
        return dialog->exec();
    }
    else if(docsBox->currentIndex() == 2){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::ORDER_CONTRACTS_LIST);
        close();
        return dialog->exec();
    }
    else if(docsBox->currentIndex() == 3){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::WORK_CONTRACTS_LIST);
        close();
        return dialog->exec();
    }
    else if(docsBox->currentIndex() == 4){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::SICK_LEAVE_LIST);
        close();
        return dialog->exec();
    }
    else if(docsBox->currentIndex() == 5){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::FREE_LEAVE_LIST);
        close();
        return dialog->exec();
    }
    else if(docsBox->currentIndex() == 6){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::JOB_TIME_LIST);
        close();
        return dialog->exec();
    }
    else if(docsBox->currentIndex() == 7){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::ANNEX_LIST);
        close();
        return dialog->exec();
    }
    else if(docsBox->currentIndex() == 8){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::BILLS_LIST);
        close();
        return dialog->exec();
    }
    else if(docsBox->currentIndex() >= 9 && docsBox->currentIndex() <= 12){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::PAY_CERTIFICATE, "", docsBox->currentIndex() - 9);
        close();
        return dialog->exec();
    }

    return 0;
}


int DocListDialog::openListCustomers(){
    if(docsBox->currentIndex() == 0){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::COMPANIES_LIST);
        close();
        return dialog->exec();
    }
    if(docsBox->currentIndex() == 1){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::FINANCE_DOC_LIST, FinanceDialog::State::FZ);
        close();
        return dialog->exec();
    }
    if(docsBox->currentIndex() == 2){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::FINANCE_DOC_LIST, FinanceDialog::State::FS);
        close();
        return dialog->exec();
    }
    if(docsBox->currentIndex() == 3){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::FINANCE_DOC_LIST, FinanceDialog::State::WZ);
        close();
        return dialog->exec();
    }
    if(docsBox->currentIndex() == 4){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::FINANCE_DOC_LIST, FinanceDialog::State::PZ);
        close();
        return dialog->exec();
    }

    return 0;
}

