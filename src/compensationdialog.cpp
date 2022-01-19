#include "compensationdialog.h"

CompensationDialog::CompensationDialog()
{
    setStyleSheet("background-color: white");
    setWindowTitle("KadriX - Dodaj Kompensatę");
    setFixedWidth(350);
    initializeUI();
    addToLayout();
    connectUI();
}


void CompensationDialog::initializeUI(){
    label = new QLabel("Rozlicz:");
    label->setStyleSheet(Data::boldLabel);

    chargeLabel = new QLabel("Należność");
    chargeLabel->setStyleSheet(Data::boldLabel);

    compensationLabel = new QLabel("Zobowiązanie");
    compensationLabel->setStyleSheet(Data::boldLabel);

    chargeCheckBox = new QCheckBox;

    commitmentCheckBox = new QCheckBox;

    chooseBtn = new QPushButton("Rozlicz");
    chooseBtn->setStyleSheet(Data::defaultButtonSheet);
    chooseBtn->setFixedSize(Data::defaultButtonSize);

    cancelBtn = new QPushButton("Wróć");
    cancelBtn->setStyleSheet(Data::defaultButtonSheet);
    cancelBtn->setFixedSize(Data::defaultButtonSize);
}


void CompensationDialog::addToLayout(){
    QVBoxLayout* layout = new QVBoxLayout;
    QHBoxLayout* firstRow = new QHBoxLayout;
    QHBoxLayout* secondRow = new QHBoxLayout;
    QHBoxLayout* btnLayout = new QHBoxLayout;

    layout->addWidget(label);

    firstRow->addWidget(chargeLabel);
    firstRow->addWidget(chargeCheckBox);

    layout->addLayout(firstRow);

    secondRow->addWidget(compensationLabel);
    secondRow->addWidget(commitmentCheckBox);

    layout->addLayout(secondRow);

    btnLayout->setAlignment(Qt::AlignRight);
    btnLayout->addWidget(chooseBtn);
    btnLayout->addWidget(cancelBtn);

    layout->addLayout(btnLayout);

    setLayout(layout);
}


void CompensationDialog::connectUI(){
    connect(chargeCheckBox, &QCheckBox::clicked, this, [&](){
        if(chargeCheckBox->isChecked())
            commitmentCheckBox->setEnabled(false);
        else
            commitmentCheckBox->setEnabled(true);
    });

    connect(commitmentCheckBox, &QCheckBox::clicked, this, [&](){
        if(commitmentCheckBox->isChecked())
            chargeCheckBox->setEnabled(false);
        else
            chargeCheckBox->setEnabled(true);
    });

    connect(chooseBtn, &QPushButton::clicked, this, &CompensationDialog::chooseDoc);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::close);
}


int CompensationDialog::chooseDoc(){
    if(chargeCheckBox->isChecked() || commitmentCheckBox->isChecked()){
        if(chargeCheckBox->isChecked()){
            DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::RECEIVABLES, FinanceDialog::State::FS);
            close();
            return dialog->exec();
        }
        else if(commitmentCheckBox->isChecked()){
            DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::COMMITMENT, FinanceDialog::State::FZ);
            close();
            return dialog->exec();
        }
    }
    else{
        QMessageBox::warning(this, "Błąd", "Proszę o wybranie określonego rozliczenia");
        return 0;
    }

    return 0;
}
