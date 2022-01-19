#include "customerswidget.h"

CustomersWidget::CustomersWidget(QWidget *parent) : QWidget(parent)
{
    initializeUI();
    addToLayout();
    connectUI();
}


void CustomersWidget::initializeUI(){
    const QString btnSheet = "QPushButton {"
                       "color: white;"
                       "font-size: 15px;"
                       "font-weight: bold;"
                       "border: 1px solid #1c45a6;"
                       "background-color: #1c45a6;"
                       "border-radius: 10px;"
                       "text-align: bottom;"
                       "padding: 5px;"
                       "}"
                       "QPushButton:hover:!pressed{"
                       "background-color: #2667ff;"
                       "}";

    const QString countBtnSheet = "QPushButton {"
                       "color: white;"
                       "font-size: 15px;"
                       "font-weight: bold;"
                       "border: 1px solid #1dad00;"
                       "background-color: #1dad00;"
                       "border-radius: 10px;"
                       "text-align: bottom;"
                       "padding: 5px;"
                       "}"
                       "QPushButton:hover:!pressed{"
                       "background-color: #2bff00;"
                       "}";

    const QSize buttonSize {230, 150};

    addCompanyBtn = new QPushButton("Dodaj Firmę");
    addCompanyBtn->setFixedSize(buttonSize);
    addCompanyBtn->setStyleSheet(btnSheet);

    addWZBtn = new QPushButton("Dodaj WZ");
    addWZBtn->setFixedSize(buttonSize);
    addWZBtn->setStyleSheet(btnSheet);

    addPZBtn = new QPushButton("Dodaj PZ");
    addPZBtn->setFixedSize(buttonSize);
    addPZBtn->setStyleSheet(btnSheet);

    addSellBtn = new QPushButton("Dodaj Sprzedaż");
    addSellBtn->setFixedSize(buttonSize);
    addSellBtn->setStyleSheet(btnSheet);

    addBuyBtn = new QPushButton("Dodaj Zakup");
    addBuyBtn->setFixedSize(buttonSize);
    addBuyBtn->setStyleSheet(btnSheet);

    addCompensationBtn = new QPushButton("Kompensaty");
    addCompensationBtn->setFixedSize(buttonSize);
    addCompensationBtn->setStyleSheet(btnSheet);

    receivablesBtn = new QPushButton("Należności");
    receivablesBtn->setFixedSize(buttonSize);
    receivablesBtn->setStyleSheet(btnSheet);

    commitmentBtn = new QPushButton("Zobowiązania");
    commitmentBtn->setFixedSize(buttonSize);
    commitmentBtn->setStyleSheet(btnSheet);

    countVATBtn = new QPushButton("Rozlicz VAT");
    countVATBtn->setFixedSize(buttonSize);
    countVATBtn->setStyleSheet(countBtnSheet);

    countZUSBtn = new QPushButton("Rozlicz ZUS");
    countZUSBtn->setFixedSize(buttonSize);
    countZUSBtn->setStyleSheet(countBtnSheet);

    countPIT4Btn = new QPushButton("Rozlicz PIT4");
    countPIT4Btn->setFixedSize(buttonSize);
    countPIT4Btn->setStyleSheet(countBtnSheet);

    docListBtn = new QPushButton("Spis Dokumentów");
    docListBtn->setFixedSize(buttonSize);
    docListBtn->setStyleSheet(btnSheet);
}


void CustomersWidget::addToLayout(){
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(0);

    QHBoxLayout* firstRowLayout = new QHBoxLayout;
    firstRowLayout->addWidget(addCompanyBtn);
    firstRowLayout->addWidget(addWZBtn);
    firstRowLayout->addWidget(addPZBtn);
    firstRowLayout->addWidget(addSellBtn);

    QHBoxLayout* secondRowLayout = new QHBoxLayout;
    secondRowLayout->addWidget(addBuyBtn);
    secondRowLayout->addWidget(addCompensationBtn);
    secondRowLayout->addWidget(receivablesBtn);
    secondRowLayout->addWidget(commitmentBtn);

    QHBoxLayout* thirdRowLayout = new QHBoxLayout;
    thirdRowLayout->addWidget(countVATBtn);
    thirdRowLayout->addWidget(countZUSBtn);
    thirdRowLayout->addWidget(countPIT4Btn);
    thirdRowLayout->addWidget(docListBtn);

    layout->addLayout(firstRowLayout);
    layout->addLayout(secondRowLayout);
    layout->addLayout(thirdRowLayout);

    setLayout(layout);
}


void CustomersWidget::connectUI(){
    connect(addCompanyBtn, &QPushButton::clicked, this, &CustomersWidget::addCompany);
    connect(addBuyBtn, &QPushButton::clicked, this, &CustomersWidget::addFZ);
    connect(addSellBtn, &QPushButton::clicked, this, &CustomersWidget::addFS);
    connect(addWZBtn, &QPushButton::clicked, this, &CustomersWidget::addWZ);
    connect(addPZBtn, &QPushButton::clicked, this, &CustomersWidget::addPZ);
    connect(addCompensationBtn, &QPushButton::clicked, this, &CustomersWidget::addCompensation);
    connect(receivablesBtn, &QPushButton::clicked, this, &CustomersWidget::openReceivables);
    connect(commitmentBtn, &QPushButton::clicked, this, &CustomersWidget::openCommitment);
    connect(docListBtn, &QPushButton::clicked, this, &CustomersWidget::openDocList);
}


int CustomersWidget::addCompany(){
    if(DBManager::isConnected()){
        CompanyDialog* dialog = new CompanyDialog;
        return dialog->exec();
    }
    else{
        QMessageBox::warning(this, "Informacja", "Proszę najpierw otworzyć bazę danych.");
    }

    return 0;
}


int CustomersWidget::addFZ(){
    if(DBManager::isConnected()){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::ADD_BUY_DOC, FinanceDialog::State::FZ);
        return dialog->exec();
    }
    else{
        QMessageBox::warning(this, "Informacja", "Proszę najpierw otworzyć bazę danych.");
    }

    return 0;
}


int CustomersWidget::addFS(){
    if(DBManager::isConnected()){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::ADD_BUY_DOC, FinanceDialog::State::FS);
        return dialog->exec();
    }
    else{
        QMessageBox::warning(this, "Informacja", "Proszę najpierw otworzyć bazę danych.");
    }

    return 0;
}


int CustomersWidget::addWZ(){
    if(DBManager::isConnected()){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::ADD_BUY_DOC, FinanceDialog::State::WZ);
        return dialog->exec();
    }
    else{
        QMessageBox::warning(this, "Informacja", "Proszę najpierw otworzyć bazę danych.");
    }

    return 0;
}


int CustomersWidget::addPZ(){
    if(DBManager::isConnected()){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::ADD_BUY_DOC, FinanceDialog::State::PZ);
        return dialog->exec();
    }
    else{
        QMessageBox::warning(this, "Informacja", "Proszę najpierw otworzyć bazę danych.");
    }

    return 0;
}


int CustomersWidget::addCompensation(){
    if(DBManager::isConnected()){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::COMPENSATION_LIST);
        return dialog->exec();
    }
    else{
        QMessageBox::warning(this, "Informacja", "Proszę najpierw otworzyć bazę danych.");
    }

    return 0;
}


int CustomersWidget::openReceivables(){
    if(DBManager::isConnected()){
        SettlementsDialog* dialog = new SettlementsDialog(SettlementsDialog::State::RECEIVABLES);
        return dialog->exec();
    }
    else{
        QMessageBox::warning(this, "Informacja", "Proszę najpierw otworzyć bazę danych.");
    }

    return 0;
}


int CustomersWidget::openCommitment(){
    if(DBManager::isConnected()){
        SettlementsDialog* dialog = new SettlementsDialog(SettlementsDialog::State::COMMITMENT);
        return dialog->exec();
    }
    else{
        QMessageBox::warning(this, "Informacja", "Proszę najpierw otworzyć bazę danych.");
    }

    return 0;
}


int CustomersWidget::openDocList(){
    if(DBManager::isConnected()){
        DocListDialog* dialog = new DocListDialog;
        return dialog->exec();
    }
    else{
        QMessageBox::warning(this, "Informacja", "Proszę najpierw otworzyć bazę danych.");
    }

    return 0;
}
