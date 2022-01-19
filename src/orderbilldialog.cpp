#include "orderbilldialog.h"
#include <cmath>

OrderBillDialog::OrderBillDialog(const QString& docId, const QString& evId, const OrderBillDialog::State& state, bool changeData)
{
    setStyleSheet("background-color: white;");
    this->changeData = changeData;
    this->docId = docId;
    this->state = state;
    evidenceId = evId;
    initializeUI();
    addToLayout();
    connectUI();

    switch(state){
        case OrderBillDialog::State::ORDER_BILL: setWindowTitle("KadriX - Stwórz rachunek do umowy zlecenie"); break;
        case OrderBillDialog::State::WORK_BILL: setWindowTitle("KadriX - Stwórz rachunek do umowy o dzieło"); break;
        case OrderBillDialog::State::JOB_BILL: setWindowTitle("KadriX - Stwórz liste płac"); break;
    }
}


void OrderBillDialog::initializeUI(){
    for(int i = 0; i < 15; ++i)
        labels.push_back(new QLabel);

    for(int i = 0; i < 11; ++i)
        checks.push_back(new QCheckBox);

    for(auto& label : labels)
        label->setStyleSheet(Data::boldLabel);

    labels.at(0)->setText("Sposób płatności:");
    labels.at(1)->setText("Prawa autorskie:");
    labels.at(2)->setText("Zryczałtowany podatek:");
    labels.at(3)->setText("Zwolnienie PIT:");
    labels.at(4)->setText("Próg podatkowy:");
    labels.at(5)->setText("Schemat składek:");
    labels.at(6)->setText("Ubez. emerytalne:");
    labels.at(7)->setText("Ubez. rentowe:");
    labels.at(8)->setText("Ubez. chorobowe:");
    labels.at(9)->setText("Ubez. zdrowotne:");
    labels.at(10)->setText("Ubez. wypadkowe:");
    labels.at(11)->setText("Fundusz pracy:");
    labels.at(12)->setText("FGŚP:");
    labels.at(13)->setText("Stawka wypadkowego(%):");
    labels.at(14)->setText("Praca poza miejscem zam.:");

    payBox = new QComboBox;
    payBox->setFixedWidth(200);
    payBox->setStyleSheet("QComboBox{ font-size: 15px; border: 2px solid #1c45a6; border-radius: 8px; padding: 5px; }");
    payBox->addItem("Gotówka");
    payBox->addItem("Przelew");

    rateBox = new QComboBox;
    rateBox->setFixedWidth(200);
    rateBox->setStyleSheet("QComboBox{ font-size: 15px; border: 2px solid #1c45a6; border-radius: 8px; padding: 5px; }");
    rateBox->addItem("17%");
    rateBox->addItem("32%");

    schemaBox = new QComboBox;
    schemaBox->setStyleSheet("QComboBox{ font-size: 15px; border: 2px solid #1c45a6; border-radius: 8px; padding: 5px; }");
    schemaBox->setFixedWidth(350);
    schemaBox->addItem("Brak");
    schemaBox->addItem("Własny pracownik");

    if(state == OrderBillDialog::State::ORDER_BILL){
        schemaBox->addItem("Student lub uczeń do 26 roku życia");
        schemaBox->addItem("Pracownik innej firmy, z wynagrodzeniem większym lub równym minimalnemu");
        schemaBox->addItem("Pracownik innej firmy, z wynagrodzeniem mniejszym od minimalnego");
        schemaBox->addItem("Osoba bez zatrudnienia");
        schemaBox->addItem("Emeryt lub rencista");
        schemaBox->addItem("Kolejna umowa zlecenie, przychód z poprzedniej umowy wynosi conajmniej minimalne wynagrodzenie");
        schemaBox->addItem("Kolejna umowa zlecenie, przychód z poprzedniej nie przekracza minimalnego wynagrodzenia");
    }

    if(state == OrderBillDialog::State::JOB_BILL){
        for(int i = 3; i < 10; ++i)
            checks.at(i)->setChecked(true);
    }

    accidentEdit = new QLineEdit;
    accidentEdit->setStyleSheet(Data::defaultLineEdit);
    accidentEdit->setFixedWidth(200);
    accidentEdit->setText("1.67");

    addBtn = new QPushButton("Zapisz");
    addBtn->setFixedSize(Data::defaultButtonSize);
    addBtn->setStyleSheet(Data::defaultButtonSheet);

    cancelBtn = new QPushButton("Anuluj");
    cancelBtn->setFixedSize(Data::defaultButtonSize);
    cancelBtn->setStyleSheet(Data::defaultButtonSheet);

    if(changeData){
        QSqlQuery query;
        query.exec("SELECT payType FROM bills WHERE id = " + docId);
        if(query.next()){
            payBox->setCurrentText(query.value(0).toString());
        }
    }
}


void OrderBillDialog::addToLayout(){
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QVector<QHBoxLayout*> hLayouts;
    QHBoxLayout* btnLayout = new QHBoxLayout;

    for(int i = 0; i < labels.size(); ++i)
        hLayouts.push_back(new QHBoxLayout);

    hLayouts.at(0)->addWidget(labels.at(0));
    hLayouts.at(0)->addWidget(payBox);

    if(state != OrderBillDialog::State::JOB_BILL){
        hLayouts.at(1)->addWidget(labels.at(1));
        hLayouts.at(1)->addWidget(checks.at(0));

        hLayouts.at(2)->addWidget(labels.at(2));
        hLayouts.at(2)->addWidget(checks.at(1));
    }

    if(state != OrderBillDialog::State::WORK_BILL){
        hLayouts.at(3)->addWidget(labels.at(3));
        hLayouts.at(3)->addWidget(checks.at(2));
    }

    hLayouts.at(4)->addWidget(labels.at(4));
    hLayouts.at(4)->addWidget(rateBox);

    if(state != OrderBillDialog::State::JOB_BILL){
        hLayouts.at(5)->addWidget(labels.at(5));
        hLayouts.at(5)->addWidget(schemaBox);
    }

    hLayouts.at(6)->addWidget(labels.at(6));
    hLayouts.at(6)->addWidget(checks.at(3));

    hLayouts.at(7)->addWidget(labels.at(7));
    hLayouts.at(7)->addWidget(checks.at(4));

    hLayouts.at(8)->addWidget(labels.at(8));
    hLayouts.at(8)->addWidget(checks.at(5));

    hLayouts.at(9)->addWidget(labels.at(9));
    hLayouts.at(9)->addWidget(checks.at(6));

    hLayouts.at(10)->addWidget(labels.at(10));
    hLayouts.at(10)->addWidget(checks.at(7));

    hLayouts.at(11)->addWidget(labels.at(11));
    hLayouts.at(11)->addWidget(checks.at(8));

    hLayouts.at(12)->addWidget(labels.at(12));
    hLayouts.at(12)->addWidget(checks.at(9));

    hLayouts.at(13)->addWidget(labels.at(13));
    hLayouts.at(13)->addWidget(accidentEdit);

    if(state == OrderBillDialog::State::JOB_BILL){
        hLayouts.at(14)->addWidget(labels.at(14));
        hLayouts.at(14)->addWidget(checks.at(10));
    }

    btnLayout->setAlignment(Qt::AlignRight);
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(cancelBtn);

    mainLayout->setMargin(20);
    mainLayout->setSpacing(10);

    for(auto& layout : hLayouts)
        mainLayout->addLayout(layout);

    mainLayout->addLayout(btnLayout);

    setLayout(mainLayout);
}


void OrderBillDialog::connectUI(){
    if(!changeData)
        connect(addBtn, &QPushButton::clicked, this, &OrderBillDialog::saveOrderBill);
    else
        connect(addBtn, &QPushButton::clicked, this, &OrderBillDialog::updateOrderBill);

    connect(checks.at(1), &QCheckBox::clicked, this, &OrderBillDialog::changeDealRate);
    connect(checks.at(2), &QCheckBox::clicked, this, &OrderBillDialog::changeZeroPit);
    connect(schemaBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &OrderBillDialog::setSchemat);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::close);
}


void OrderBillDialog::calculateBill(float brutto){
    auto rtd = [](float value){ float var = (int)(value * 100 + 0.5f); return (float)var / 100; };

    float incomeCost{0.0f};
    float incomeCostP{0.0f};
    float insuranceAll{0.0f};
    float insuranceAdd{0.0f};
    prices.push_back(checks.at(3)->isChecked() ? rtd(brutto * 0.0976f) : 0.0f); //Ub. emerytalne(pracownik)
    prices.push_back(checks.at(4)->isChecked() ? rtd(brutto * 0.015f) : 0.0f); //Ub. rentowne(pracownik)
    prices.push_back(checks.at(5)->isChecked() ? rtd(brutto * 0.0245f) : 0.0f);  //Ub. chorobowe(pracownik)
    incomeCost = rtd(prices.at(0) + prices.at(1) + prices.at(2));
    prices.push_back(incomeCost);  //Suma składek(pracownik)

    prices.push_back(checks.at(3)->isChecked() ? rtd(brutto * 0.0976f) : 0.0f);  //Ub. emerytalne(pracodawca)
    prices.push_back(checks.at(4)->isChecked() ? rtd(brutto * 0.065f) : 0.0f);  //Ub. rentowne(pracodawca)
    prices.push_back(checks.at(7)->isChecked() ? rtd(brutto * accidentEdit->text().toFloat() / 100) : 0.0f);  //Ub. wypadkowe(pracodawca)
    incomeCostP = rtd(prices.at(4) + prices.at(5) + prices.at(6));
    prices.push_back(incomeCostP);  //Suma składek(pracodawca)

    if(checks.at(1)->isEnabled() && checks.at(1)->isChecked()){ //Zryczałtowany podatek
        prices.push_back(0.0f); //Koszt przychodu
        prices.push_back(brutto); //Dochód do opodatkowania
    }
    else{ //Normalny koszt przychodu
        if(checks.at(2)->isEnabled() && checks.at(2)->isChecked()){
            prices.push_back(0.0f); //Koszt przychodu
            prices.push_back(0.0f); //Dochód do opodatkowania
        }
        else{
            if(state != OrderBillDialog::State::JOB_BILL){
                prices.push_back(rtd((brutto - incomeCost) * (checks.at(0)->isChecked() ? 0.5f : 0.2f))); //Koszt przychodu
                prices.push_back(std::roundf(brutto - incomeCost - prices.at(8))); //Dochód do opodatkowania
            }
            else{
                prices.push_back(checks.at(10)->isChecked() ? 300.0f : 250.0f); //Koszt przychodu
                prices.push_back(std::roundf(brutto - incomeCost - prices.at(8))); //Dochód do opodatkowania
            }
        }
    }

    if(checks.at(6)->isChecked()){
        insuranceAll = rtd((brutto - incomeCost) * 0.09f);
        insuranceAdd = rtd((brutto - incomeCost) * 0.0775f);
    }

    float rate{0.0f};
    if(rateBox->isEnabled() && rateBox->currentIndex() == 0){
        prices.push_back(std::roundf((prices.at(9) * 0.17f) - insuranceAdd)); //Podatek
        rate = rtd(prices.at(9) * 0.17f);
    }
    else if(rateBox->isEnabled() && rateBox->currentIndex() == 1){
        prices.push_back(std::roundf((prices.at(9) * 0.32f) - insuranceAdd)); //Podatek
        rate = rtd(prices.at(9) * 0.32f);
    }
    else if(checks.at(1)->isEnabled() && checks.at(1)->isChecked())
        prices.push_back(std::roundf(brutto * 0.17f)); //Podatek

    if(insuranceAll > rate)
        insuranceAll = rate;

    prices.push_back(insuranceAll); //Ubez. zdrowotne
    prices.push_back(insuranceAdd); //Ubez. zdrowotne odliczane
    prices.push_back(insuranceAll - insuranceAdd); //Ubez. zdrowotne nieodliczane
    prices.push_back(checks.at(8)->isChecked() ? rtd(brutto * 0.0245f) : 0.0f); //Fundusz pracy
    prices.push_back(checks.at(9)->isChecked() ? rtd(brutto * 0.001f) : 0.0f); //FGŚP
    prices.push_back(rtd(brutto - incomeCost - insuranceAll - prices.at(10))); //Wypłata netto
}


void OrderBillDialog::changeDealRate(bool value){
    if(state == OrderBillDialog::State::ORDER_BILL){
        if(value){
            checks.at(2)->setEnabled(false);
            rateBox->setEnabled(false);
        }
        else{
            checks.at(2)->setEnabled(true);
            rateBox->setEnabled(true);
        }
    }
    else if(state == OrderBillDialog::State::WORK_BILL){
        if(value){
            rateBox->setEnabled(false);
        }
        else{
            rateBox->setEnabled(true);
        }
    }
}


void OrderBillDialog::changeZeroPit(bool value){
    if(value){
        checks.at(1)->setEnabled(false);
    }
    else{
        checks.at(1)->setEnabled(true);
    }
}


void OrderBillDialog::setSchemat(int index){
    if(index == 0){
        for(int i = 3; i < checks.size(); ++ i)
            checks.at(i)->setChecked(false);
    }
    else if(index == 1){
        for(int i = 3; i < checks.size(); ++ i)
            checks.at(i)->setChecked(true);
    }
    else if(index == 2){
        for(int i = 3; i < checks.size(); ++ i)
            checks.at(i)->setChecked(false);
    }
    else if(index == 3){
        for(int i = 3; i < checks.size(); ++i){
            if(i == 6)
                checks.at(i)->setChecked(true);
            else
                checks.at(i)->setChecked(false);
        }
    }
    else if(index == 4){
        for(int i = 3; i < checks.size(); ++i){
            if(i == 5)
                checks.at(i)->setChecked(false);
            else
                checks.at(i)->setChecked(true);
        }
    }
    else if(index == 5){
        for(int i = 3; i < checks.size(); ++i){
            if(i == 5)
                checks.at(i)->setChecked(false);
            else
                checks.at(i)->setChecked(true);
        }
    }
    else if(index == 6){
        checks.at(3)->setChecked(true);
        checks.at(4)->setChecked(true);
        checks.at(6)->setChecked(true);
        checks.at(7)->setChecked(true);
        checks.at(5)->setChecked(false);
        checks.at(8)->setChecked(false);
        checks.at(9)->setChecked(false);
    }
    else if(index == 7){
        for(int i = 0; i < checks.size(); ++i){
            if(i == 6)
                checks.at(i)->setChecked(true);
            else
                checks.at(i)->setChecked(false);
        }
    }
    else if(index == 8){
        for(int i = 0; i < checks.size(); ++i){
            if(i == 5)
                checks.at(i)->setChecked(false);
            else
                checks.at(i)->setChecked(true);
        }
    }
}


void OrderBillDialog::saveOrderBill(){
    auto rtd = [](float value){ float var = (int)(value * 100 + 0.5f); return (float)var / 100; };

    QString employeeId{""};
    float reward{0.0f};
    float payForHour{0.0f};
    int hoursSum{0};
    int freeHoursSum{0};
    int sickDaysSum{0};
    float brutto{0.0f};
    float freeBrutto{0.0f};
    float sickBrutto{0.0f};
    float basicBrutto{0.0f};

    QSqlQuery query;
    if(state == OrderBillDialog::State::ORDER_BILL){
        query.exec("SELECT payForHour, employeeId FROM orderContracts WHERE docId = " + docId);
        if(query.next()){
            payForHour = query.value(0).toFloat();
            employeeId = query.value(1).toString();
        }

        query.exec("SELECT sum FROM jobTimeEvidence WHERE id = " + evidenceId);
        if(query.next()){
            hoursSum = query.value(0).toInt();
        }

        brutto = payForHour * hoursSum;
    }
    else if(state == OrderBillDialog::State::WORK_BILL){
        query.exec("SELECT reward, employeeId FROM workContracts WHERE docId = " + docId);
        if(query.next()){
            brutto = query.value(0).toFloat();
            employeeId = query.value(1).toString();
        }
    }
    else if(state == OrderBillDialog::State::JOB_BILL){
        query.exec("SELECT payForHour, reward, employeeId FROM jobContracts WHERE docId = " + docId);
        if(query.next()){
            payForHour = query.value(0).toFloat();
            reward = query.value(1).toFloat();
            employeeId = query.value(2).toString();
        }

        query.exec("SELECT sum, freeDaysSum, sickDaysSum FROM jobTimeEvidence WHERE id = " + evidenceId);
        if(query.next()){
            hoursSum = query.value(0).toInt();
            freeHoursSum = query.value(1).toInt();
            sickDaysSum = query.value(2).toInt();
        }

        basicBrutto = payForHour * hoursSum;
        freeBrutto = payForHour * freeHoursSum;
        sickBrutto = reward - (reward * 0.1371f);
        sickBrutto /= 30.0f;
        sickBrutto *= 0.8f;
        sickBrutto *= sickDaysSum;

        brutto = basicBrutto + freeBrutto + sickBrutto;
    }

    calculateBill(brutto);

    int currentId{0};
    if(state == OrderBillDialog::State::ORDER_BILL)
        query.exec("SELECT numberId, createDate FROM bills WHERE type = 'uz'");
    else if(state == OrderBillDialog::State::WORK_BILL)
        query.exec("SELECT numberId, createDate FROM bills WHERE type = 'ud'");
    else if(state == OrderBillDialog::State::JOB_BILL)
        query.exec("SELECT numberId, createDate FROM bills WHERE type = 'uop'");

    if(query.isSelect() && query.last()){
        if(query.value(1).toDate().month() == QDate::currentDate().month())
            currentId = query.value(0).toInt() + 1;
        else
            currentId = 1;
    }
    else{
        currentId = 1;
    }

    const QString tableVariables = "(docId, rewardBrutto, insuranceOld, insuranceRent, insuranceDisease, zus, insuranceOldP, insuranceRentP, insuranceAccident, zusP, incomeCost, incomeCostAfter, rate, insurance, insuranceAdd, insuranceSub, "
                                   "jobFund, fgsp, rewardNetto, payType, createDate, contractId, jobEvidenceId, numberId, type, basicBrutto, freeBrutto, sickBrutto, employeeId)";
    const QString bindVariables = "(:docId, :rewardBrutto, :insuranceOld, :insuranceRent, :insuranceDisease, :zus, :insuranceOldP, :insuranceRentP, :insuranceAccident, :zusP, :incomeCost, :incomeCostAfter, :rate, :insurance, :insuranceAdd, :insuranceSub, "
                                  ":jobFund, :fgsp, :rewardNetto, :payType, :createDate, :contractId, :jobEvidenceId, :numberId, :type, :basicBrutto, :freeBrutto, :sickBrutto, :employeeId)";
    QString docNumber;
    if(state == OrderBillDialog::State::ORDER_BILL){
        docNumber = "RUZ/";
        query.exec("UPDATE jobTimeEvidence SET hasBill = true WHERE id = " + evidenceId);
    }
    else if(state == OrderBillDialog::State::WORK_BILL){
        docNumber = "RUD/";
        query.exec("UPDATE workContracts SET hasBill = true WHERE docId = " + docId);
    }
    else if(state == OrderBillDialog::State::JOB_BILL){
        docNumber = "LP/";
        query.exec("UPDATE jobTimeEvidence SET hasBill = true WHERE id = " + evidenceId);
    }

    query.prepare("INSERT INTO bills " + tableVariables + " VALUES " + bindVariables);
    query.bindValue(":docId", docNumber + QString::number(currentId) + "/" + QString::number(QDate::currentDate().month()) + "/" + QString::number(QDate::currentDate().year()));
    query.bindValue(":rewardBrutto", brutto);
    query.bindValue(":insuranceOld", prices.at(0));
    query.bindValue(":insuranceRent", prices.at(1));
    query.bindValue(":insuranceDisease", prices.at(2));
    query.bindValue(":zus", prices.at(3));
    query.bindValue(":insuranceOldP", prices.at(4));
    query.bindValue(":insuranceRentP", prices.at(5));
    query.bindValue(":insuranceAccident", prices.at(6));
    query.bindValue(":zusP", prices.at(7));
    query.bindValue(":incomeCost", prices.at(8));
    query.bindValue(":incomeCostAfter", prices.at(9));
    query.bindValue(":rate", prices.at(10));
    query.bindValue(":insurance", prices.at(11));
    query.bindValue(":insuranceAdd", prices.at(12));
    query.bindValue(":insuranceSub", prices.at(13));
    query.bindValue(":jobFund", prices.at(14));
    query.bindValue(":fgsp", prices.at(15));
    query.bindValue(":rewardNetto", prices.at(16));
    query.bindValue(":payType", payBox->currentText());
    query.bindValue(":createDate", QDate::currentDate());
    query.bindValue(":contractId", docId);
    query.bindValue(":jobEvidenceId", evidenceId);
    query.bindValue(":numberId", currentId);
    query.bindValue(":employeeId", employeeId);

    if(state == OrderBillDialog::State::ORDER_BILL)
        query.bindValue(":type", "uz");
    else if(state == OrderBillDialog::State::WORK_BILL)
        query.bindValue(":type", "ud");
    else if(state == OrderBillDialog::State::JOB_BILL)
        query.bindValue(":type", "uop");

    query.bindValue(":basicBrutto", rtd(basicBrutto));
    query.bindValue(":freeBrutto", rtd(freeBrutto));
    query.bindValue(":sickBrutto", rtd(sickBrutto));

    query.exec();

    prices.clear();
    close();
    QMessageBox::information(this, "Sukces!", "Pomyślnie dodano nowy rachunek.");
}


void OrderBillDialog::updateOrderBill(){
    float brutto{0.0f};
    QSqlQuery query;
    query.exec("SELECT rewardBrutto FROM bills WHERE id = " + docId);
    if(query.next()){
        brutto = query.value(0).toFloat();
    }

    calculateBill(brutto);

    query.prepare("UPDATE bills SET rewardBrutto = :rewardBrutto, insuranceOld = :iOld, insuranceRent = :iRent, insuranceDisease = :iDisease, zus = :zus, insuranceOldP = :iOldP"
                  ",insuranceRentP = :iRentP, insuranceAccident = :iAccident, zusP = :zusP, incomeCost = :incomeCost, incomeCostAfter = :incomeCostAfter, rate = :rate, insurance = :insurance"
                  ",insuranceAdd = :insuranceAdd, insuranceSub = :insuranceSub, jobFund = :jobFund, fgsp = :fgsp, rewardNetto = :rewardNetto, payType = :payType WHERE id = " + docId);
    query.bindValue(":rewardBrutto", brutto);
    query.bindValue(":iOld", prices.at(0));
    query.bindValue(":iRent", prices.at(1));
    query.bindValue(":iDisease", prices.at(2));
    query.bindValue(":zus", prices.at(3));
    query.bindValue(":iOldP", prices.at(4));
    query.bindValue(":iRentP", prices.at(5));
    query.bindValue(":iAccident", prices.at(6));
    query.bindValue(":zusP", prices.at(7));
    query.bindValue(":incomeCost", prices.at(8));
    query.bindValue(":incomeCostAfter", prices.at(9));
    query.bindValue(":rate", prices.at(10));
    query.bindValue(":insurance", prices.at(11));
    query.bindValue(":insuranceAdd", prices.at(12));
    query.bindValue(":insuranceSub", prices.at(13));
    query.bindValue(":jobFund", prices.at(14));
    query.bindValue(":fgsp", prices.at(15));
    query.bindValue(":rewardNetto", prices.at(16));
    query.bindValue(":payType", payBox->currentText());
    query.exec();

    prices.clear();
    close();
    QMessageBox::information(this, "Sukces!", "Pomyślnie zaktualizowano rachunek.");
}
