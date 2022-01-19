#include "financedialog.h"

FinanceDialog::FinanceDialog(const FinanceDialog::State& state, const QString& id, bool change, bool base)
{
    this->state = state;
    companyId = id;
    changeData = change;
    isBaseDoc = base;
    setStyleSheet("background-color: white;");
    initializeUI();
    addToLayout();
    connectUI();

    switch(state){
        case FinanceDialog::State::WZ: setWindowTitle("Dodaj nowe wydanie zewnętrzne"); break;
        case FinanceDialog::State::PZ: setWindowTitle("Dodaj nowe przyjęcie zewnętrzne"); break;
        case FinanceDialog::State::FS: setWindowTitle("Dodaj nową fakture sprzedaży"); break;
        case FinanceDialog::State::FZ: setWindowTitle("Dodaj nową fakture zakupu"); break;
    }
}


void FinanceDialog::initializeUI(){
    for(int i = 0; i < 11; ++i){
        labels.push_back(new QLabel);
        if(i > 2 && i < 10) lineEdits.push_back(new QLineEdit);
    }

    for(auto& label : labels)
        label->setStyleSheet(Data::boldLabel);

    for(int i = 0; i < lineEdits.size(); ++i){
        if(i > 0 && i < lineEdits.size())
            lineEdits.at(i)->setStyleSheet("QLineEdit{ font-size: 15px; border: 2px solid #1c45a6; border-radius: 8px; padding: 5px; background-color: #69ff98; }");
        else
            lineEdits.at(i)->setStyleSheet(Data::defaultLineEdit);
        lineEdits.at(i)->setFixedWidth(350);
    }

    labels.at(0)->setText("Nr. dokumentu:");
    labels.at(1)->setText("Data wydania:");
    labels.at(2)->setText("Termin płatności:");
    labels.at(3)->setText("Sposób zapłaty:");
    labels.at(4)->setText("Nazwa towaru/usługi:");
    labels.at(5)->setText("Jednostka:");
    labels.at(6)->setText("Ilość:");
    labels.at(7)->setText("Rabat(%):");
    labels.at(8)->setText("Cenna netto(szt.):");
    labels.at(9)->setText("Vat(%):");
    labels.at(10)->setText("Status(Tak/Nie):");

    dateEdit = new QDateEdit;
    dateEdit->setStyleSheet(Data::defaultDateEdit);
    dateEdit->setFixedWidth(350);

    payEdit = new QDateEdit;
    payEdit->setStyleSheet(Data::defaultDateEdit);
    payEdit->setFixedWidth(350);

    payTypeBox = new QComboBox;
    payTypeBox->setFixedWidth(350);
    payTypeBox->setStyleSheet("QComboBox{ font-size: 15px; border: 2px solid #1c45a6; border-radius: 8px; padding: 5px; }");
    payTypeBox->addItem("Gotówka");
    payTypeBox->addItem("Przelew");
    payTypeBox->addItem("Kompensata");

    statusCheck = new QCheckBox;

    addRecordBtn = new QPushButton("Dodaj");
    addRecordBtn->setStyleSheet(Data::defaultButtonSheet);
    addRecordBtn->setFixedSize(Data::defaultButtonSize);

    saveBtn = new QPushButton("Zapisz");
    saveBtn->setStyleSheet(Data::defaultButtonSheet);
    saveBtn->setFixedSize(Data::defaultButtonSize);

    cancelBtn = new QPushButton("Anuluj");
    cancelBtn->setStyleSheet(Data::defaultButtonSheet);
    cancelBtn->setFixedSize(Data::defaultButtonSize);

    QStringList headers{"Nazwa Towaru/Usługi", "J.M.", "Ilość", "Rabat(%)", "Cena Netto", "Cena Netto(Po rabacie)", "Wartość Netto", "VAT(%)", "Wartość VAT", "Wartość Brutto"};
    table = new QTableWidget;
    table->setColumnCount(headers.size());
    table->setHorizontalHeaderLabels(headers);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setColumnWidth(0, 130);
    table->setColumnWidth(5, 130);

    lineEdits.at(0)->setEnabled(false);

    if(isBaseDoc || changeData){
        lineEdits.at(0)->setEnabled(false);
        QVector<QStringList> stringLists;
        QSqlQuery query;

        query.exec("SELECT docId, createDate, payDate, payType, itemName, unit, count, discount, costNetto, costNettoDiscount, netto, vat, costVat, brutto, status from finance WHERE id = " + companyId);
        if(query.next()){

            if(changeData){
                lineEdits.at(0)->setText(query.value(0).toString());
                dateEdit->setDate(query.value(1).toDate());
                payEdit->setDate(query.value(2).toDate());
                payTypeBox->setCurrentText(query.value(3).toString());

                if(query.value(14).toString() == "Tak")
                    statusCheck->setChecked(true);
                else
                    statusCheck->setChecked(false);
            }

            for(int i = 4; i < 14; ++i){
                stringLists.push_back(query.value(i).toString().split(","));
            }

            table->setRowCount(stringLists.at(0).size());

            for(int i = 0; i < stringLists.size(); ++i){
                for(int j = 0; j < stringLists.at(i).size(); ++j){
                    table->setItem(j, i, new QTableWidgetItem(stringLists.at(i).at(j)));
                }
            }
        }
    }
}


void FinanceDialog::addToLayout(){
    QVBoxLayout* layout = new QVBoxLayout;
    QGridLayout* grid = new QGridLayout;
    QHBoxLayout* addRecordLayout = new QHBoxLayout;
    QHBoxLayout* btnLayout = new QHBoxLayout;

    for(int i = 0; i < labels.size(); ++i)
        grid->addWidget(labels.at(i), i, 0);

    for(int i = 0; i < lineEdits.size(); ++i){
        if(i == 0)
            grid->addWidget(lineEdits.at(i), i, 1);
        else
            grid->addWidget(lineEdits.at(i), i + 3, 1);
    }

    grid->addWidget(dateEdit, 1, 1);
    grid->addWidget(payEdit, 2, 1);
    grid->addWidget(payTypeBox, 3, 1);
    grid->addWidget(statusCheck, lineEdits.size() + 3, 1);

    layout->addLayout(grid);

    addRecordLayout->setAlignment(Qt::AlignRight);
    addRecordLayout->addWidget(addRecordBtn);

    layout->addLayout(addRecordLayout);

    layout->addSpacing(30);
    layout->addWidget(table);

    btnLayout->setAlignment(Qt::AlignRight);
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(cancelBtn);

    layout->addLayout(btnLayout);
    setLayout(layout);
}


void FinanceDialog::connectUI(){
    if(!changeData)
        connect(saveBtn, &QPushButton::clicked, this, &FinanceDialog::addDoc);
    else
        connect(saveBtn, &QPushButton::clicked, this, &FinanceDialog::changeDoc);

    connect(addRecordBtn, &QPushButton::clicked, this, &FinanceDialog::addRecord);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::close);
    connect(table, &QTableWidget::itemDoubleClicked, this, [this](){
        for(int i = 0; i < table->columnCount(); ++i){
            delete table->takeItem(table->currentRow(), i);
        }
        table->removeRow(table->currentRow());
    });
}


QString FinanceDialog::getColumnAsString(int index){
    QString result{""};

    if(index >= 0){
        for(int i = 0; i < table->rowCount(); ++i){
            if(i != table->rowCount() - 1)
                result += table->item(i, index)->text() + ",";
            else
                result += table->item(i, index)->text();
        }
    }

    return result;
}


float FinanceDialog::getColumnSum(int index){
    float result{0.0f};

    if(index >= 0){
        for(int i = 0; i < table->rowCount(); ++i){
            result += table->item(i, index)->text().toFloat();
        }
    }

    return result;
}


void FinanceDialog::addRecord(){
    const int empty = std::count_if(lineEdits.begin() + 2, lineEdits.end() - 1, [](QLineEdit* lineEdit){
        return lineEdit->text().isEmpty();
    });

    if(empty == 0){
        table->setRowCount(table->rowCount() + 1);
        table->setItem(table->rowCount() - 1, 0, new QTableWidgetItem(lineEdits.at(1)->text()));
        table->setItem(table->rowCount() - 1, 1, new QTableWidgetItem(lineEdits.at(2)->text()));
        table->setItem(table->rowCount() - 1, 2, new QTableWidgetItem(lineEdits.at(3)->text()));
        table->setItem(table->rowCount() - 1, 3, new QTableWidgetItem(lineEdits.at(4)->text()));
        table->setItem(table->rowCount() - 1, 4, new QTableWidgetItem(lineEdits.at(5)->text()));

        float discount = lineEdits.at(5)->text().toFloat() * (lineEdits.at(4)->text().toFloat() / 100);
        float discountResult = lineEdits.at(5)->text().toFloat() - discount;
        float nettoResult = discountResult * lineEdits.at(3)->text().toFloat();
        float vat = nettoResult * (lineEdits.at(6)->text().toFloat() / 100);
        float brutto = nettoResult + vat;

        table->setItem(table->rowCount() - 1, 5, new QTableWidgetItem(QString::number(discountResult)));
        table->setItem(table->rowCount() - 1, 6, new QTableWidgetItem(QString::number(nettoResult)));
        table->setItem(table->rowCount() - 1, 7, new QTableWidgetItem(lineEdits.at(6)->text()));
        table->setItem(table->rowCount() - 1, 8, new QTableWidgetItem(QString::number(vat)));
        table->setItem(table->rowCount() - 1, 9, new QTableWidgetItem(QString::number(brutto)));

        for(int i = 1; i < 7; ++i)
            lineEdits.at(i)->clear();
    }
    else{
        QMessageBox::information(this, "Ups!", "Proszę o wypełnienie zielonych pól.");
    }
}


void FinanceDialog::addDoc(){
    if(table->rowCount() != 0){
        QSqlQuery query;
        int docId{0};
        QString type{""};

        if(state == FinanceDialog::State::FZ)
            type = "FZ";
        else if(state == FinanceDialog::State::WZ)
            type = "WZ";
        else if(state == FinanceDialog::State::FS)
            type = "FS";
        else if(state == FinanceDialog::State::PZ)
            type = "PZ";

        query.exec("SELECT createDate, numberId FROM finance WHERE type = '" + type + "'");
        if(query.isSelect() && query.last()){
            if(query.value(0).toDate().month() == QDate::currentDate().month())
                docId = query.value(1).toInt() + 1;
            else
                docId = 1;
        }
        else
            docId = 1;

        const QString tableVariables = "(docId, createDate, payDate, payType, itemName, unit, count, discount, costNetto, costNettoDiscount, netto, vat, costVat, brutto, sumNetto, sumVat, sumBrutto, companyId, type, numberId, status)";
        const QString bindVariables = "(:docId, :createDate, :payDate, :payType, :itemName, :unit, :count, :discount, :costNetto, :costNettoDiscount, :netto, :vat, :costVat, :brutto, :sumNetto, :sumVat, :sumBrutto, :companyId, :type, :numberId, :status)";
        query.prepare("INSERT INTO finance " + tableVariables + " VALUES " + bindVariables);
        query.bindValue(":docId", type + "/" + QString::number(docId) + "/" + QString::number(QDate::currentDate().month()) + "/" + QString::number(QDate::currentDate().year()));
        query.bindValue(":createDate", dateEdit->date());
        query.bindValue(":payDate", payEdit->date());
        query.bindValue(":payType", payTypeBox->currentText());
        query.bindValue(":itemName", getColumnAsString(0));
        query.bindValue(":unit", getColumnAsString(1));
        query.bindValue(":count", getColumnAsString(2));
        query.bindValue(":discount", getColumnAsString(3));
        query.bindValue(":costNetto", getColumnAsString(4));
        query.bindValue(":costNettoDiscount", getColumnAsString(5));
        query.bindValue(":netto", getColumnAsString(6));
        query.bindValue(":vat", getColumnAsString(7));
        query.bindValue(":costVat", getColumnAsString(8));
        query.bindValue(":brutto", getColumnAsString(9));
        query.bindValue(":sumNetto", getColumnSum(6));
        query.bindValue(":sumVat", getColumnSum(8));
        query.bindValue(":sumBrutto", getColumnSum(9));
        query.bindValue(":companyId", companyId);
        query.bindValue(":type", type);
        query.bindValue(":numberId", docId);
        query.bindValue(":status", statusCheck->isChecked() ? "Tak" : "Nie");
        query.exec();

        if(state == FinanceDialog::State::FZ){
            if(payTypeBox->currentIndex() == 2){
                float balance{};
                QString buyDocId, buyDocDate, buyDocBrutto;
                QSqlQuery query;

                query.exec("SELECT balance, buyDocId, buyDocDate, buyDocBrutto FROM account WHERE id = " + companyId);
                if(query.next()){
                    balance = query.value(0).toFloat();
                    buyDocId = query.value(1).toString();
                    buyDocDate = query.value(2).toString();
                    buyDocBrutto = query.value(3).toString();
                }

                balance -= getColumnSum(9);
                buyDocId += type + "/" + QString::number(docId) + "/" + QString::number(QDate::currentDate().month()) + "/" + QString::number(QDate::currentDate().year()) + ",";
                buyDocDate += dateEdit->date().toString("dd.MM.yyyy") + ",";
                buyDocBrutto += QString::number(getColumnSum(9)) + ",";

                query.prepare("UPDATE account SET balance = :balance, buyDocId = :buyDocId, buyDocDate = :buyDocDate, buyDocBrutto = :buyDocBrutto WHERE id = " + companyId);
                query.bindValue(":balance", balance);
                query.bindValue(":buyDocId", buyDocId);
                query.bindValue(":buyDocDate", buyDocDate);
                query.bindValue(":buyDocBrutto", buyDocBrutto);
                query.exec();
            }

            QMessageBox::information(this, "Sukces!", "Pomyślnie dodano fakturę zakupu.");
        }
        else if(state == FinanceDialog::State::WZ)
            QMessageBox::information(this, "Sukces!", "Pomyślnie dodano wydanie zewnętrzne.");
        else if(state == FinanceDialog::State::FS){
            if(payTypeBox->currentIndex() == 2){
                float balance{};
                QString sellDocId, sellDocDate, sellDocBrutto;
                QSqlQuery query;

                query.exec("SELECT balance, sellDocId, sellDocDate, sellDocBrutto FROM account WHERE id = " + companyId);
                if(query.next()){
                    balance = query.value(0).toFloat();
                    sellDocId = query.value(1).toString();
                    sellDocDate = query.value(2).toString();
                    sellDocBrutto = query.value(3).toString();
                }

                balance += getColumnSum(9);

                sellDocId += type + "/" + QString::number(docId) + "/" + QString::number(QDate::currentDate().month()) + "/" + QString::number(QDate::currentDate().year()) + ",";
                sellDocDate += dateEdit->date().toString("dd.MM.yyyy") + ",";
                sellDocBrutto += QString::number(getColumnSum(9)) + ",";

                query.prepare("UPDATE account SET balance = :balance, sellDocId = :sellDocId, sellDocDate = :sellDocDate, sellDocBrutto = :sellDocBrutto WHERE id = " + companyId);
                query.bindValue(":balance", balance);
                query.bindValue(":sellDocId", sellDocId);
                query.bindValue(":sellDocDate", sellDocDate);
                query.bindValue(":sellDocBrutto", sellDocBrutto);
                query.exec();
            }

            QMessageBox::information(this, "Sukces!", "Pomyślnie dodano fakturę sprzedaży.");
        }
        else if(state == FinanceDialog::State::PZ)
            QMessageBox::information(this, "Sukces!", "Pomyślnie dodano przyjęcie zewnętrzne.");

        close();
    }
    else{
        QMessageBox::information(this, "Ups!", "Brak rekordów do dodania.");
    }
}


void FinanceDialog::changeDoc(){
    if(table->rowCount() != 0){
        QSqlQuery query;
        QString type{""};

        if(state == FinanceDialog::State::FZ)
            type = "FZ";
        else if(state == FinanceDialog::State::WZ)
            type = "WZ";
        else if(state == FinanceDialog::State::FS)
            type = "FS";
        else if(state == FinanceDialog::State::PZ)
            type = "PZ";

        query.prepare("UPDATE finance SET createDate = :createDate, payDate = :payDate, payType = :payType, itemName = :itemName, unit = :unit, count = :count, discount = :discount, costNetto = :costNetto, costNettoDiscount = :costNettoDiscount,"
                      "netto = :netto, vat = :vat, costVat = :costVat, brutto = :brutto, sumNetto = :sumNetto, sumVat = :sumVat, sumBrutto = :sumBrutto, status = :status WHERE id = " + companyId);
        query.bindValue(":createDate", dateEdit->date());
        query.bindValue(":payDate", payEdit->date());
        query.bindValue(":payType", payTypeBox->currentText());
        query.bindValue(":itemName", getColumnAsString(0));
        query.bindValue(":unit", getColumnAsString(1));
        query.bindValue(":count", getColumnAsString(2));
        query.bindValue(":discount", getColumnAsString(3));
        query.bindValue(":costNetto", getColumnAsString(4));
        query.bindValue(":costNettoDiscount", getColumnAsString(5));
        query.bindValue(":netto", getColumnAsString(6));
        query.bindValue(":vat", getColumnAsString(7));
        query.bindValue(":costVat", getColumnAsString(8));
        query.bindValue(":brutto", getColumnAsString(9));
        query.bindValue(":sumNetto", getColumnSum(6));
        query.bindValue(":sumVat", getColumnSum(8));
        query.bindValue(":sumBrutto", getColumnSum(9));
        query.bindValue(":status", statusCheck->isChecked() ? "Tak" : "Nie");
        query.exec();

        if(state == FinanceDialog::State::FZ){
            if(payTypeBox->currentIndex() == 2){
                float balance{};
                QString docId, cId, buyDocId, buyDocDate, buyDocBrutto;
                QSqlQuery query;

                query.exec("SELECT docId, companyId FROM finance WHERE id = " + companyId);
                if(query.next()){
                    docId = query.value(0).toString();
                    cId = query.value(1).toString();
                }

                query.exec("SELECT balance, buyDocId, buyDocDate, buyDocBrutto FROM account WHERE id = " + cId);
                if(query.next()){
                    balance = query.value(0).toFloat();
                    buyDocId = query.value(1).toString();
                    buyDocDate = query.value(2).toString();
                    buyDocBrutto = query.value(3).toString();
                }

                balance -= getColumnSum(9);
                buyDocId += docId + ",";
                buyDocDate += dateEdit->date().toString("dd.MM.yyyy") + ",";
                buyDocBrutto += QString::number(getColumnSum(9)) + ",";

                query.prepare("UPDATE account SET balance = :balance, buyDocId = :buyDocId, buyDocDate = :buyDocDate, buyDocBrutto = :buyDocBrutto WHERE id = " + cId);
                query.bindValue(":balance", balance);
                query.bindValue(":buyDocId", buyDocId);
                query.bindValue(":buyDocDate", buyDocDate);
                query.bindValue(":buyDocBrutto", buyDocBrutto);
                query.exec();
            }

            QMessageBox::information(this, "Sukces!", "Pomyślnie zaktualizowano fakturę zakupu.");
        }
        else if(state == FinanceDialog::State::WZ)
            QMessageBox::information(this, "Sukces!", "Pomyślnie zaktualizowano wydanie zewnętrzne.");
        else if(state == FinanceDialog::State::FS){
            if(payTypeBox->currentIndex() == 2){
                float balance{};
                QString docId, cId, sellDocId, sellDocDate, sellDocBrutto;
                QSqlQuery query;

                query.exec("SELECT docId, companyId FROM finance WHERE id = " + companyId);
                if(query.next()){
                    docId = query.value(0).toString();
                    cId = query.value(1).toString();
                }

                query.exec("SELECT balance, sellDocId, sellDocDate, sellDocBrutto FROM account WHERE id = " + cId);
                if(query.next()){
                    balance = query.value(0).toFloat();
                    sellDocId = query.value(1).toString();
                    sellDocDate = query.value(2).toString();
                    sellDocBrutto = query.value(3).toString();
                }

                balance += getColumnSum(9);
                sellDocId += docId + ",";
                sellDocDate += dateEdit->date().toString("dd.MM.yyyy") + ",";
                sellDocBrutto += QString::number(getColumnSum(9)) + ",";

                query.prepare("UPDATE account SET balance = :balance, sellDocId = :sellDocId, sellDocDate = :sellDocDate, sellDocBrutto = :sellDocBrutto WHERE id = " + cId);
                query.bindValue(":balance", balance);
                query.bindValue(":sellDocId", sellDocId);
                query.bindValue(":sellDocDate", sellDocDate);
                query.bindValue(":sellDocBrutto", sellDocBrutto);
                query.exec();
            }

            QMessageBox::information(this, "Sukces!", "Pomyślnie zaktualizowano fakturę sprzedaży.");
        }
        else if(state == FinanceDialog::State::PZ)
            QMessageBox::information(this, "Sukces!", "Pomyślnie zaktualizowano przyjęcie zewnętrzne.");

        close();
    }
    else{
        QMessageBox::information(this, "Ups!", "Brak rekordów do dodania.");
    }
}
