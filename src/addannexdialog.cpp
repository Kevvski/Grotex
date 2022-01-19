#include "addannexdialog.h"

AddAnnexDialog::AddAnnexDialog(const AppState::ContractState& state, const QString& docId, const QString& empId, const QString& textId, bool changeData)
{
    this->changeData = changeData;
    this->state = state;
    this->docId = docId;
    employeeId = empId;
    setWindowTitle("KadriX - Stwórz aneks do umowy " + textId);
    setStyleSheet("background-color: white;");
    initializeUI();
    addToLayout();
    connectUI();
}


void AddAnnexDialog::initializeUI(){
    empDataWidget = new EmployeeDataWidget(employeeId);

    changeLabel = new QLabel("Zmiana:");
    changeLabel->setStyleSheet(Data::boldLabel);

    fromLabel = new QLabel("Z:");
    fromLabel->setStyleSheet(Data::boldLabel);

    onLabel = new QLabel("Na:");
    onLabel->setStyleSheet(Data::boldLabel);

    changeBox = new QComboBox;
    changeBox->setFixedWidth(350);
    changeBox->setStyleSheet("QComboBox{ font-size: 15px; border: 2px solid #1c45a6; border-radius: 8px; padding: 5px; }");
    changeBox->addItem("Okres trwania umowy");
    changeBox->addItem("Dane pracownika");
    changeBox->addItem("Wynagrodzenie");
    changeBox->addItem("Stanowisko");

    changeList = new QListWidget;

    fromEdit = new QLineEdit;
    fromEdit->setFixedWidth(250);
    fromEdit->setStyleSheet(Data::defaultLineEdit);

    onEdit = new QLineEdit;
    onEdit->setFixedWidth(250);
    onEdit->setStyleSheet(Data::defaultLineEdit);

    onBtn = new QPushButton("Dodaj");
    onBtn->setFixedSize(Data::defaultButtonSize);
    onBtn->setStyleSheet(Data::defaultButtonSheet);

    addBtn = new QPushButton("Zapisz");
    addBtn->setFixedSize(Data::defaultButtonSize);
    addBtn->setStyleSheet(Data::defaultButtonSheet);

    cancelBtn = new QPushButton("Anuluj");
    cancelBtn->setFixedSize(Data::defaultButtonSize);
    cancelBtn->setStyleSheet(Data::defaultButtonSheet);

    if(changeData){
        QStringList items;
        QSqlQuery query;
        query.exec("SELECT changeList FROM annexes WHERE id = " + docId);
        if(query.next()){
            items = query.value(0).toString().split(",");
        }

        for(int i = 0; i < items.size(); ++i){
            QListWidgetItem* item = new QListWidgetItem;
            item->setText(items.at(i));
            changeList->addItem(item);
        }
    }
}


void AddAnnexDialog::addToLayout(){
    QHBoxLayout* layout = new QHBoxLayout;
    QVBoxLayout* vLayout = new QVBoxLayout;
    QHBoxLayout* firstRow = new QHBoxLayout;
    QHBoxLayout* secondRow = new QHBoxLayout;
    QHBoxLayout* thirdRow = new QHBoxLayout;
    QHBoxLayout* btnLayout = new QHBoxLayout;

    firstRow->addWidget(fromLabel);
    firstRow->addWidget(fromEdit);
    firstRow->addSpacing(Data::defaultButtonSize.width() + 5);

    secondRow->addWidget(onLabel);
    secondRow->addWidget(onEdit);
    secondRow->addWidget(onBtn);

    thirdRow->addWidget(changeLabel);
    thirdRow->addWidget(changeBox);

    btnLayout->setAlignment(Qt::AlignRight);
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(cancelBtn);

    vLayout->addLayout(thirdRow);
    vLayout->addLayout(firstRow);
    vLayout->addLayout(secondRow);
    vLayout->addWidget(changeList);
    vLayout->addLayout(btnLayout);

    layout->addWidget(empDataWidget);
    layout->addLayout(vLayout);

    setLayout(layout);
}


void AddAnnexDialog::connectUI(){
    connect(onBtn, &QPushButton::clicked, this, &AddAnnexDialog::addToList);
    connect(changeList, &QListWidget::doubleClicked, this, [this](){
        delete changeList->takeItem(changeList->currentRow());
    });

    if(!changeData)
        connect(addBtn, &QPushButton::clicked, this, &AddAnnexDialog::addAnnex);
    else
        connect(addBtn, &QPushButton::clicked, this, &AddAnnexDialog::changeAnnex);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::close);
}


const QString AddAnnexDialog::getChangesAsString() const{
    QString result;

    for(int i = 0; i < changeList->count(); ++i){
        if(i != changeList->count() - 1)
            result += changeList->item(i)->text() + ",";
        else
            result += changeList->item(i)->text();
    }

    return result;
}


void AddAnnexDialog::addToList(){
    if(!fromEdit->text().isEmpty() && !onEdit->text().isEmpty()){
        const QString result = changeBox->currentText() + " z " + fromEdit->text() + " na " + onEdit->text();
        QListWidgetItem* item = new QListWidgetItem;
        item->setText(result);

        changeList->addItem(item);
        fromEdit->clear();
        onEdit->clear();
    }
    else{
        QMessageBox::warning(this, "Błąd", "Proszę uzupełnić wszystkie pola.");
    }
}


void AddAnnexDialog::addAnnex(){
    if(changeList->count() != 0){
        int currentID{0};
        QSqlQuery query;

        query.exec("SELECT createDate, numberId FROM annexes");
        if(query.isSelect() && query.last()){
            if(QDate::currentDate().month() == query.value(0).toDate().month() && QDate::currentDate().year() == query.value(0).toDate().year()){
                currentID = query.value(1).toInt() + 1;
            }
        }
        else{
            currentID = 1;
        }

        const QString tableVariables = "(docId, contractId, type, createDate, numberId, changeList)";
        const QString bindVariables = "(:docId, :contractId, :type, :createDate, :numberId, :changeList)";
        query.prepare("INSERT INTO annexes " + tableVariables + " VALUES " + bindVariables);
        query.bindValue(":docId", "A/" + QString::number(currentID) + "/" + QString::number(QDate::currentDate().month()) + "/" + QString::number(QDate::currentDate().year()));
        query.bindValue(":contractId", docId);

        switch(state){
        case AppState::ContractState::JOB_ANNEX: query.bindValue(":type", "Umowa o pracę"); break;
        case AppState::ContractState::ORDER_ANNEX: query.bindValue(":type", "Umowa zlecenie"); break;
        case AppState::ContractState::WORK_ANNEX: query.bindValue(":type", "Umowa o dzieło"); break;
        }

        query.bindValue(":createDate", QDate::currentDate());
        query.bindValue(":numberId", currentID);
        query.bindValue(":changeList", getChangesAsString());
        query.exec();

        close();
        QMessageBox::information(this, "Sukces!", "Pomyślnie dodano nowy aneks.");

    }
    else{
        QMessageBox::warning(this, "Błąd", "Proszę o wprowadzenie zmian na listę.");
    }
}


void AddAnnexDialog::changeAnnex(){
    if(changeList->count() != 0){
        QSqlQuery query;
        query.exec("UPDATE annexes SET changeList = '" + getChangesAsString() + "' WHERE id = " + docId);

        close();
        QMessageBox::information(this, "Sukces!", "Pomyślnie zaktualizowano aneks.");
    }
    else{
        QMessageBox::warning(this, "Błąd", "Proszę o wprowadzenie zmian na listę.");
    }
}
