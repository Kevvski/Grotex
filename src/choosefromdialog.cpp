#include "choosefromdialog.h"

ChooseFromDialog::ChooseFromDialog()
{
    setWindowTitle("KadriX - Wybierz typ umowy");
    setStyleSheet("background-color: white;");
    initializeUI();
    addToLayout();
    connectUI();
}


void ChooseFromDialog::initializeUI(){
    chooseBox = new QComboBox;
    chooseBox->setFixedWidth(350);
    chooseBox->setStyleSheet("QComboBox{ font-size: 15px; border: 2px solid #1c45a6; border-radius: 8px; padding: 5px; }");
    chooseBox->addItem("Umowa o pracę");
    chooseBox->addItem("Umowa zlecenie");
    chooseBox->addItem("Umowa o dzieło");

    chooseBtn = new QPushButton("Wybierz");
    chooseBtn->setFixedSize(Data::defaultButtonSize);
    chooseBtn->setStyleSheet(Data::defaultButtonSheet);

    cancelBtn = new QPushButton("Wróć");
    cancelBtn->setFixedSize(Data::defaultButtonSize);
    cancelBtn->setStyleSheet(Data::defaultButtonSheet);
}


void ChooseFromDialog::addToLayout(){
    QVBoxLayout* layout = new QVBoxLayout;
    QHBoxLayout* btnLayout = new QHBoxLayout;

    btnLayout->setAlignment(Qt::AlignRight);
    btnLayout->addWidget(chooseBtn);
    btnLayout->addWidget(cancelBtn);

    layout->addWidget(chooseBox);
    layout->addLayout(btnLayout);

    setLayout(layout);
}


void ChooseFromDialog::connectUI(){
    connect(chooseBtn, &QPushButton::clicked, this, &ChooseFromDialog::chooseContract);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::close);
}


int ChooseFromDialog::chooseContract(){
    if(chooseBox->currentIndex() == 0){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::CHOOSE_TO_ANNEX, AppState::ContractState::JOB_ANNEX);
        close();
        return dialog->exec();
    }
    else if(chooseBox->currentIndex() == 1){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::CHOOSE_TO_ANNEX, AppState::ContractState::ORDER_ANNEX);
        close();
        return dialog->exec();
    }
    else if(chooseBox->currentIndex() == 2){
        DBViewDialog* dialog = new DBViewDialog(DBViewDialog::State::CHOOSE_TO_ANNEX, AppState::ContractState::WORK_ANNEX);
        close();
        return dialog->exec();
    }

    return 0;
}
