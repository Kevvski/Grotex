#include "addfreewidget.h"

AddFreeWidget::AddFreeWidget(const AddFreeWidget::State& state, const QString& id, bool changeData)
{
    this->state = state;
    initializeUI(id, changeData);
    addToLayout();
}


void AddFreeWidget::initializeUI(const QString& id, bool changeData){
    startDateLabel = new QLabel("Termin rozpoczęcia:");
    startDateLabel->setStyleSheet(Data::boldLabel);

    endDateLabel = new QLabel("Termin zakończenia:");
    endDateLabel->setStyleSheet(Data::boldLabel);

    freeTypeLabel = new QLabel("Rodzaj urlopu:");
    freeTypeLabel->setStyleSheet(Data::boldLabel);

    QDate now;
    startDateEdit = new QDateEdit(now.currentDate());
    startDateEdit->setStyleSheet(Data::defaultDateEdit);
    startDateEdit->setFixedWidth(350);
    startDateEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);

    endDateEdit = new QDateEdit(now.currentDate());
    endDateEdit->setStyleSheet(Data::defaultDateEdit);
    endDateEdit->setFixedWidth(350);
    endDateEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);

    freeTypeBox = new QComboBox;
    freeTypeBox->setStyleSheet("QComboBox{ font-size: 15px; border: 2px solid #1c45a6; border-radius: 8px; padding: 5px; }");
    freeTypeBox->setFixedWidth(350);
    freeTypeBox->addItem("Urlop bezpłatny");
    freeTypeBox->addItem("Urlop okolicznościowy");
    freeTypeBox->addItem("Urlop na żądanie");
    freeTypeBox->addItem("Opieka nad dzieckiem");
    freeTypeBox->addItem("Urlop ojcowski");
    freeTypeBox->addItem("Urlop macierzyński");
    freeTypeBox->addItem("Urlop rodzicielski");
    freeTypeBox->addItem("Urlop wychowawczy");
    freeTypeBox->addItem("Urlop na poszukiwanie pracy");
    freeTypeBox->addItem("Urlop szkoleniowy");

    if(state == AddFreeWidget::State::SICK_LEAVE && changeData){
        QSqlQuery query;
        query.exec("SELECT leaveStartDate, leaveEndDate FROM sickLeaves WHERE id = " + id);

        if(query.next()){
            startDateEdit->setDate(query.value(0).toDate());
            endDateEdit->setDate(query.value(1).toDate());
        }
    }
    else if(state == AddFreeWidget::State::FREE_LEAVE && changeData){
        QSqlQuery query;
        query.exec("SELECT leaveStartDate, leaveEndDate, freeType FROM freeLeaves WHERE id = " + id);

        if(query.next()){
            startDateEdit->setDate(query.value(0).toDate());
            endDateEdit->setDate(query.value(1).toDate());

            for(int i = 0; i < freeTypeBox->count(); ++i){
                if(freeTypeBox->itemText(i) == query.value(2).toString())
                    freeTypeBox->setCurrentIndex(i);
            }
        }
    }
}


void AddFreeWidget::addToLayout(){
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(startDateLabel, 0, 0);
    layout->addWidget(endDateLabel, 1, 0);
    layout->addWidget(startDateEdit, 0, 1);
    layout->addWidget(endDateEdit, 1, 1);

    if(state == AddFreeWidget::State::FREE_LEAVE){
        layout->addWidget(freeTypeLabel, 2, 0);
        layout->addWidget(freeTypeBox, 2, 1);
    }

    setLayout(layout);
}


const QDateEdit* AddFreeWidget::getStartDate() const{
    return startDateEdit;
}


const QDateEdit* AddFreeWidget::getEndDate() const{
    return endDateEdit;
}


const QComboBox* AddFreeWidget::getFreeComboBox() const{
    return freeTypeBox;
}
