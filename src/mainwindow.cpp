#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(QSize(1000, 700));
    setStyleSheet("background-color: white;");
    setWindowTitle("KadriX");

    menuWidget = new MenuAppWidget(this);
    connect(menuWidget->getEmplyeesBtn(), &QPushButton::clicked, this, &MainWindow::toEmployeeWidget);
    connect(menuWidget->getCustomersBtn(), &QPushButton::clicked, this, &MainWindow::toCustomersWidget);
    connect(menuWidget->getDatabasesBtn(), &QPushButton::clicked, this, &MainWindow::toDatabasesInfo);
    connect(menuWidget->getCompanyBtn(), &QPushButton::clicked, this, &MainWindow::toCompanyWidget);
    connect(menuWidget->getCashBtn(), &QPushButton::clicked, this, &MainWindow::toCashWidget);

    CadreWidget* cadreWidget = new CadreWidget(this);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(menuWidget, 0, Qt::AlignTop);
    layout->addWidget(cadreWidget, 1);

    QWidget* mainWidget = new QWidget;
    mainWidget->setLayout(layout);

    setCentralWidget(mainWidget);
}


MainWindow::~MainWindow()
{
}



//Connecting Slots--------------------------------------------------
void MainWindow::toCompanyWidget(){
    if(DBManager::isConnected()){
        if(AppState::getState() != AppState::State::COMPANY){
            AppState::setState(AppState::State::COMPANY);

            CompanyWidget::loadCompanyData();
            CompanyWidget* companyWidget = new CompanyWidget(this);

            QVBoxLayout* layout = new QVBoxLayout;
            layout->setMargin(0);
            layout->setSpacing(0);
            layout->addWidget(menuWidget, 0, Qt::AlignTop);
            layout->addWidget(companyWidget, 1);

            QWidget* mainWidget = new QWidget;
            mainWidget->setLayout(layout);

            setCentralWidget(mainWidget);
        }
    }
    else{
        QMessageBox::warning(this, "Ups...", "Proszę o otwarcie bazy danych");
    }
}


void MainWindow::toEmployeeWidget(){
    if(AppState::getState() != AppState::State::EMPLOYEES){
        AppState::setState(AppState::State::EMPLOYEES);

        CompanyWidget::loadCompanyData();
        CadreWidget* cadreWidget = new CadreWidget(this);

        QVBoxLayout* layout = new QVBoxLayout;
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(menuWidget, 0, Qt::AlignTop);
        layout->addWidget(cadreWidget, 1);

        QWidget* mainWidget = new QWidget;
        mainWidget->setLayout(layout);

        setCentralWidget(mainWidget);
    }
}


void MainWindow::toCustomersWidget(){
    if(AppState::getState() != AppState::State::CUSTOMERS){
        AppState::setState(AppState::State::CUSTOMERS);

        CompanyWidget::loadCompanyData();
        CustomersWidget* customersWidget = new CustomersWidget(this);

        QVBoxLayout* layout = new QVBoxLayout;
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(menuWidget, 0, Qt::AlignTop);
        layout->addWidget(customersWidget, 1);

        QWidget* mainWidget = new QWidget;
        mainWidget->setLayout(layout);

        setCentralWidget(mainWidget);
    }
}


void MainWindow::toDatabasesInfo(){
    if(AppState::getState() != AppState::State::DATABASES){
        AppState::setState(AppState::State::DATABASES);

        CompanyWidget::loadCompanyData();
        DatabaseWidget* dbWidget = new DatabaseWidget(this);

        QVBoxLayout* layout = new QVBoxLayout;
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(menuWidget, 0, Qt::AlignTop);
        layout->addWidget(dbWidget);

        QWidget* mainWidget = new QWidget;
        mainWidget->setLayout(layout);

        setCentralWidget(mainWidget);
    }
}


void MainWindow::toCashWidget(){
    if(DBManager::isConnected()){
        if(AppState::getState() != AppState::State::CASH){
            AppState::setState(AppState::State::CASH);

            CompanyWidget::loadCompanyData();
            CashWidget* dbWidget = new CashWidget(this);

            QVBoxLayout* layout = new QVBoxLayout;
            layout->setMargin(0);
            layout->setSpacing(0);
            layout->addWidget(menuWidget, 0, Qt::AlignTop);
            layout->addWidget(dbWidget);

            QWidget* mainWidget = new QWidget;
            mainWidget->setLayout(layout);

            setCentralWidget(mainWidget);
        }
    }
    else{
        QMessageBox::warning(this, "Ups...", "Proszę o otwarcie bazy danych");
    }
}

