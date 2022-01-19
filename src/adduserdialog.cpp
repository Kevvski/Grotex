#include "adduserdialog.h"

AddUserDialog::AddUserDialog()
{
    state = AddUserDialog::State::NEW_EMPLOYEE;
    setStyleSheet("background-color: white;");
    setWindowTitle("KadriX - Nowy Pracownik");
    initializeUI();
    addToLayout();
    connectUI();
}


AddUserDialog::AddUserDialog(QString id)
{
    state = AddUserDialog::State::UPDATE_EMPLOYEE;
    setStyleSheet("background-color: white;");
    setWindowTitle("KadriX - Aktualizacja Pracownika");
    initializeUI();
    addToLayout();
    connectUI();

    addBtn->setText("Zapisz");
    labels.at(1)->setText(id);

    QSqlQuery query;
    query.exec("SELECT name, secondName, surname, birthDate, birthPlace, cityCode, city, address, houseNumber, localNumber, pesel, parentName, parentSurname, evidenceNumber FROM employees WHERE id = " + id);

    if(query.next()){
        for(int i = 0; i < lineEdits.size(); ++i){
            lineEdits.at(i)->setText(query.value(i).toString());
        }
    }
}


void AddUserDialog::initializeUI(){
    for(int i = 0; i < 16; ++i){
        labels.push_back(new QLabel);
    }

    for(int i = 0; i < 14; ++i){
        lineEdits.push_back(new QLineEdit);
    }

    for(auto& label : labels){
        label->setFixedWidth(200);
        label->setStyleSheet(Data::boldLabel);
    }

    for(auto& lineEdit : lineEdits){
        lineEdit->setStyleSheet(Data::defaultLineEdit);
    }

    labels.at(0)->setText(DBManager::getEmployeeVariable(0));

    QSqlQuery query;
    query.exec("SELECT id FROM employees");
    if(query.isSelect() && query.last()){
        const int futureEmployeeId = query.value(0).toInt() + 1;
        labels.at(1)->setText(QString::number(futureEmployeeId));
    }
    else
        labels.at(1)->setText("1");

    for(int i = 0; i < 14; ++i)
        labels.at(i + 2)->setText(DBManager::getEmployeeVariable(i + 1));

    addBtn = new QPushButton("Dodaj");
    addBtn->setFixedSize(Data::defaultButtonSize);
    addBtn->setStyleSheet(Data::defaultButtonSheet);

    cancelBtn = new QPushButton("Anuluj");
    cancelBtn->setFixedSize(Data::defaultButtonSize);
    cancelBtn->setStyleSheet(Data::defaultButtonSheet);
}


void AddUserDialog::addToLayout(){
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(labels.at(0), 0, 0);
    layout->addWidget(labels.at(1), 0, 1);

    for(int i = 2; i < labels.size(); ++i){
        layout->addWidget(labels.at(i), i, 0);
    }

    for(int i = 1; i < lineEdits.size() + 1; ++i){
        layout->addWidget(lineEdits.at(i - 1), i + 1, 1);
    }

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->setAlignment(Qt::AlignRight);
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(cancelBtn);

    layout->addLayout(btnLayout, lineEdits.size() + 2, 1);
    setLayout(layout);
}


void AddUserDialog::connectUI(){
    switch(state){
    case AddUserDialog::State::NEW_EMPLOYEE: connect(addBtn, &QPushButton::clicked, this, &AddUserDialog::addEmployee); break;
    case AddUserDialog::State::UPDATE_EMPLOYEE: connect(addBtn, &QPushButton::clicked, this, &AddUserDialog::updateEmployee); break;
    }

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::close);
}



//Connecting Slots--------------------------------------------------
void AddUserDialog::addEmployee(){
    const int emptyCount = std::count_if(lineEdits.begin(), lineEdits.end(), [](QLineEdit* lineEdit){
            return lineEdit->text().isEmpty();
    });

    int employeeId{-1};
    QSqlQuery query;
    query.exec("SELECT id FROM employees WHERE pesel = '" + lineEdits.at(10)->text() + "'");
    if(query.next()){
        employeeId = query.value(0).toInt();
    }

    if(emptyCount == 0){
        if(employeeId == -1){
            QString variablesName = "(name, secondName, surname, birthDate, birthPlace, cityCode, city, address, houseNumber,"
                                    "localNumber, pesel, parentName, parentSurname, evidenceNumber, isWorking, activeWork, uop, uz, uod, freeDaysCount)";
            QString variablesIDs = "(:name, :secondName, :surname, :birthDate, :birthPlace, :cityCode, :city, :address, :houseNumber,"
                                   ":localNumber, :pesel, :parentName, :parentSurname, :evidenceNumber, :isWorking, :activeWork, :uop, :uz, :uod, :freeDaysCount)";

            query.prepare("INSERT INTO employees" + variablesName + "values " + variablesIDs);
            query.bindValue(":name", lineEdits.at(0)->text());
            query.bindValue(":secondName", lineEdits.at(1)->text());
            query.bindValue(":surname", lineEdits.at(2)->text());
            query.bindValue(":birthDate", lineEdits.at(3)->text());
            query.bindValue(":birthPlace", lineEdits.at(4)->text());
            query.bindValue(":cityCode", lineEdits.at(5)->text());
            query.bindValue(":city", lineEdits.at(6)->text());
            query.bindValue(":address", lineEdits.at(7)->text());
            query.bindValue(":houseNumber", lineEdits.at(8)->text());
            query.bindValue(":localNumber", lineEdits.at(9)->text());
            query.bindValue(":pesel", lineEdits.at(10)->text());
            query.bindValue(":parentName", lineEdits.at(11)->text());
            query.bindValue(":parentSurname", lineEdits.at(12)->text());
            query.bindValue(":evidenceNumber", lineEdits.at(13)->text());
            query.bindValue(":jobStartDate", "brak");
            query.bindValue(":jobEndDate", "brak");
            query.bindValue(":isWorking", "nie");
            query.bindValue(":activeWork", false);
            query.bindValue(":uop", false);
            query.bindValue(":uz", false);
            query.bindValue(":uod", false);
            query.bindValue(":freeDaysCount", 0.0f);
            query.exec();

            QMessageBox::information(this, "Sukces!", "Pomyślnie dodano nowego pracownika.");

            close();
        }
        else{
            QMessageBox::information(this, "Informacja", "Pracownik o podanym numerze pesel już istnieje.");
        }
    }
    else{
        QMessageBox::warning(this, "Błąd", "Proszę uzupełnić wszystkie pola.");
    }
}


void AddUserDialog::updateEmployee(){
    const int emptyCount = std::count_if(lineEdits.begin(), lineEdits.end(), [](QLineEdit* lineEdit) {
        return lineEdit->text().isEmpty();
    });

    if(emptyCount == 0){
         QString updateString =
         "name = '" + lineEdits.at(0)->text() + "'"
         ",secondName = '" + lineEdits.at(1)->text() + "'"
         ",surname = '" + lineEdits.at(2)->text() + "'"
         ",birthDate = '" + lineEdits.at(3)->text() + "'"
         ",birthPlace = '" + lineEdits.at(4)->text() + "'"
         ",cityCode = '" + lineEdits.at(5)->text() + "'"
         ",city = '" + lineEdits.at(6)->text() + "'"
         ",address = '" + lineEdits.at(7)->text() + "'"
         ",houseNumber = '" + lineEdits.at(8)->text() + "'"
         ",localNumber = '" + lineEdits.at(9)->text() + "'"
         ",pesel = '" + lineEdits.at(10)->text() + "'"
         ",parentName = '" + lineEdits.at(11)->text() + "'"
         ",parentSurname = '" + lineEdits.at(12)->text() + "'"
         ",evidenceNumber = '" + lineEdits.at(13)->text() + "'";

         QSqlQuery query;
         query.exec("UPDATE employees SET " + updateString + "WHERE id = '" + labels.at(1)->text() + "'");

         QMessageBox::information(this, "Sukces!", "Pomyślnie zaktualizowano pracownika.");

         close();
    }
    else{
        QMessageBox::warning(this, "Błąd", "Proszę wypełnić wszystkie pola.");
    }
}
