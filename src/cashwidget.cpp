#include "cashwidget.h"

CashWidget::CashWidget(QWidget *parent) : QWidget(parent), change{true}, recordSum{0.0f}
{
    setStyleSheet("background-color: white;");
    initializeUI();
    addToLayout();
    connectUI();
}


void CashWidget::initializeUI(){
    balanceHeaderLabel = new QLabel("Saldo:");
    balanceHeaderLabel->setAlignment(Qt::AlignCenter);
    balanceHeaderLabel->setStyleSheet("QLabel{ color: #1c45a6; font-size: 25px; font-weight: bold; }");

    balanceLabel = new QLabel("0.00 zł");
    balanceLabel->setAlignment(Qt::AlignCenter);
    balanceLabel->setStyleSheet("QLabel{ color: #1c45a6; font-size: 65px; font-weight: bold; }");

    dateLabel = new QLabel("Data:");
    dateLabel->setStyleSheet(Data::boldLabel);

    aboutLabel = new QLabel("Opis:");
    aboutLabel->setStyleSheet(Data::boldLabel);

    costLabel = new QLabel("Kwota:");
    costLabel->setStyleSheet(Data::boldLabel);

    compensationLabel = new QLabel("Spłata kompensaty(Tak/Nie):");
    compensationLabel->setStyleSheet(Data::boldLabel);

    listLabel = new QLabel("Spisy:");
    //listLabel->setAlignment(Qt::AlignCenter);
    listLabel->setStyleSheet(Data::boldLabel);

    lastAddLabel = new QLabel("Spis:");
    lastAddLabel->setStyleSheet(Data::boldLabel);

    incomeLabel = new QLabel("Przychody razem:");
    incomeLabel->setStyleSheet(Data::boldLabel);

    incomeValueLabel = new QLabel("0.00 zł");
    incomeValueLabel->setStyleSheet(Data::boldLabel);

    leaveLabel = new QLabel("Rozchody razem:");
    leaveLabel->setStyleSheet(Data::boldLabel);

    leaveValueLabel = new QLabel("0.00 zł");
    leaveValueLabel->setStyleSheet(Data::boldLabel);

    dateEdit = new QDateEdit();
    dateEdit->setStyleSheet(Data::defaultDateEdit);
    dateEdit->setFixedWidth(300);

    aboutEdit = new QLineEdit();
    aboutEdit->setStyleSheet(Data::defaultLineEdit);
    aboutEdit->setFixedWidth(300);

    costEdit = new QLineEdit();
    costEdit->setStyleSheet(Data::defaultLineEdit);
    costEdit->setFixedWidth(300);

    compensationCheckBox = new QCheckBox();

    table = new QTableWidget;
    table->setFixedWidth(520);
    table->setFixedHeight(280);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels(QStringList{ "Data dodania", "Opis", "Kwota(zł)" });
    table->setColumnWidth(0, 150);
    table->setColumnWidth(1, 185);
    table->setColumnWidth(2, 182);

    addRecordBtn = new QPushButton("Dodaj");
    addRecordBtn->setStyleSheet(Data::defaultButtonSheet);
    addRecordBtn->setFixedSize(Data::defaultButtonSize);

    incomeBtn = new QPushButton("Przychód");
    incomeBtn->setStyleSheet(Data::defaultButtonSheet);
    incomeBtn->setFixedSize(Data::defaultButtonSize);

    leaveBtn = new QPushButton("Rozchód");
    leaveBtn->setStyleSheet(Data::defaultButtonSheet);
    leaveBtn->setFixedSize(Data::defaultButtonSize);

    incomeListBtn = new QPushButton("Przychodów");
    incomeListBtn->setStyleSheet(Data::defaultButtonSheet);
    incomeListBtn->setFixedSize(Data::defaultButtonSize);

    leaveListBtn = new QPushButton("Rozchodów");
    leaveListBtn->setStyleSheet(Data::defaultButtonSheet);
    leaveListBtn->setFixedSize(Data::defaultButtonSize);
}


void CashWidget::addToLayout(){
    QVBoxLayout* layout = new QVBoxLayout;
    QHBoxLayout* incomeLayout = new QHBoxLayout;
    QHBoxLayout* leaveLayout = new QHBoxLayout;
    QVBoxLayout* balanceLayout = new QVBoxLayout;
    QVBoxLayout* listLayout = new QVBoxLayout;
    QVBoxLayout* tableLayout = new QVBoxLayout;
    QHBoxLayout* firstRow = new QHBoxLayout;
    QHBoxLayout* secondRow = new QHBoxLayout;

    QVBoxLayout* addMainLayout = new QVBoxLayout;
    QGridLayout* addLayout = new QGridLayout;
    QHBoxLayout* addBtnLayout = new QHBoxLayout;

    incomeLayout->addWidget(incomeLabel);
    incomeLayout->addWidget(incomeValueLabel);

    leaveLayout->addWidget(leaveLabel);
    leaveLayout->addWidget(leaveValueLabel);

    balanceLayout->addWidget(balanceHeaderLabel);
    balanceLayout->addWidget(balanceLabel);
    balanceLayout->setMargin(90);
    firstRow->addLayout(balanceLayout);
    firstRow->addSpacing(50);

    addLayout->addWidget(dateLabel, 0, 0);
    addLayout->addWidget(dateEdit, 0, 1);
    addLayout->addWidget(aboutLabel, 1, 0);
    addLayout->addWidget(aboutEdit, 1, 1);
    addLayout->addWidget(costLabel, 2, 0);
    addLayout->addWidget(costEdit, 2, 1);
    addLayout->addWidget(compensationLabel, 3, 0);
    addLayout->addWidget(compensationCheckBox, 3, 1);

    addBtnLayout->setAlignment(Qt::AlignRight);
    addBtnLayout->addWidget(addRecordBtn);
    addBtnLayout->addWidget(incomeBtn);
    addBtnLayout->addWidget(leaveBtn);

    addMainLayout->addLayout(addLayout);
    addMainLayout->addLayout(addBtnLayout);
    firstRow->addLayout(addMainLayout);

    listLayout->addLayout(incomeLayout);
    listLayout->addLayout(leaveLayout);
    listLayout->addStretch(10);
    listLayout->addWidget(listLabel);
    listLayout->addWidget(incomeListBtn);
    listLayout->addWidget(leaveListBtn);

    tableLayout->addWidget(lastAddLabel);
    tableLayout->addWidget(table);

    secondRow->addLayout(listLayout);
    secondRow->addStretch(10);
    secondRow->addLayout(tableLayout);

    layout->addLayout(firstRow);
    layout->addStretch(15);
    layout->addLayout(secondRow);
    layout->addStretch(20);
    setLayout(layout);
}


void CashWidget::connectUI(){
    connect(addRecordBtn, &QPushButton::clicked, this, &CashWidget::addRecord);
    connect(incomeBtn, &QPushButton::clicked, this, [&](){ change = true; addCash(); });
    connect(leaveBtn, &QPushButton::clicked, this, [&](){ change = false; addCash(); });
    connect(table, &QTableWidget::itemClicked, table, &QTableWidget::editItem);
    connect(table, &QTableWidget::itemDoubleClicked, this, [&](){
        for(int i = 0; i < table->columnCount(); ++i){
            delete table->takeItem(table->currentRow(), i);
        }
        table->removeRow(table->currentRow());
    });
}


void CashWidget::addRecord(){
    const int row = table->rowCount();
    table->setRowCount(row + 1);
    table->setItem(row, 0, new QTableWidgetItem(dateEdit->date().toString("dd.MM.yyyy")));
    table->setItem(row, 1, new QTableWidgetItem(aboutEdit->text()));
    table->setItem(row, 2, new QTableWidgetItem(costEdit->text()));

    recordSum += costEdit->text().toFloat();

    aboutEdit->clear();
    costEdit->clear();
}


void CashWidget::addIncome(){
    recordSum = 0.0f;
}


void CashWidget::addLeave(){
    recordSum = 0.0f;
}


void CashWidget::addCash(){
    if(compensationCheckBox->isChecked()){
        if(change){
            addIncome();
        }
        else{
            addLeave();
        }
    }
    else{
        if(change){
            addIncome();
        }
        else{
            addLeave();
        }
    }
}

