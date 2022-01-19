#include "printdocuments.h"

PrintDocuments::PrintDocuments() : progressValue{0}
{
    progress = new QProgressDialog;
    progress->setRange(0, 100);
    progress->setWindowModality(Qt::WindowModality::WindowModal);
    progress->setMinimumDuration(0);
}


void PrintDocuments::setEmployeeID(const QString &id){
    employeeID = id;
}


void PrintDocuments::setDocumentID(const QString &id){
    docId = id;
}


void PrintDocuments::setOrderBillID(const QString &id){
    orderBillId = id;
}


void PrintDocuments::loadDocuments(){
    QSqlQuery query;
    query.exec("SELECT id, name, secondName, surname, birthDate, birthPlace, cityCode, city, address, houseNumber, localNumber, pesel, parentName, parentSurname, evidenceNumber FROM employees WHERE id = " + employeeID);
    if(query.next()){
        for(int i = 0; i < query.record().count(); ++i)
            employeeVariables.push_back(query.value(i).toString());
    }

    query.exec("SELECT jobCategory, jobPlace, reward, jobDimension, hoursCount, jobStartDate, jobEndDate, id, decision FROM jobContracts WHERE docId = " + docId);
    if(query.next()){
        for(int i = 0; i < query.record().count(); ++i)
            jobContractVariables.push_back(query.value(i).toString());

        decision = query.value(8).toBool();
    }

    query.exec("SELECT id, hoursCount, jobStartDate, jobEndDate, payForHour, jobDimension, job FROM orderContracts WHERE docId = " + docId);
    if(query.next()){
        for(int i = 0; i < query.record().count(); ++i)
            orderContractVariables.push_back(query.value(i).toString());
    }

    query.exec("SELECT id, reward, jobs, entrusts, jobStartDate, jobEndDate, jobDimension FROM workContracts WHERE docId = " + docId);
    if(query.next()){
        for(int i = 0; i < query.record().count(); ++i)
            workContractVariables.push_back(query.value(i).toString());
    }

    query.exec("SELECT name, director, nip, city, cityCode, address, regon, office, email FROM mainCompany WHERE id = 1");
    if(query.next()){
        for(int i = 0; i < query.record().count(); ++i)
            companyVariables.push_back(query.value(i).toString());
    }

    query.exec("SELECT docId, type, createDate, changeList FROM annexes WHERE id = " + docId);
    if(query.next()){
        for(int i = 0; i < query.record().count(); ++i)
            annexVariables.push_back(query.value(i).toString());
    }

    query.exec("SELECT docId, rewardBrutto, insuranceOld, insuranceRent, insuranceDisease, zus, insuranceOldP, insuranceRentP, insuranceAccident, zusP, incomeCost, incomeCostAfter, rate, insurance, insuranceAdd, insuranceSub, "
               "jobFund, fgsp, rewardNetto, payType, createDate, type, numberId, basicBrutto, freeBrutto, sickBrutto FROM bills WHERE id = " + orderBillId);
    if(query.next()){
        for(int i = 0; i < query.record().count(); ++i)
            orderBillVariables.push_back(query.value(i).toString());
    }

     query.exec("SELECT docId, createDate, payDate, payType, itemName, unit, count, discount, costNetto, costNettoDiscount, netto, vat, costVat, brutto, status, sumNetto, sumVat, sumBrutto from finance WHERE id = " + docId);
     if(query.next()){
         for(int i = 0; i < query.record().count(); ++i)
             financeVariables.push_back(query.value(i).toString());
     }

     query.exec("SELECT balance, sellDocId, sellDocDate, sellDocBrutto, buyDocId, buyDocDate, buyDocBrutto FROM account WHERE id = " + docId);
     if(query.next()){
         for(int i = 0; i < query.record().count(); ++i)
             compensationVariables.push_back(query.value(i).toString());
     }
}


void PrintDocuments::printJobContract(){
    progress->setLabelText("Tworzenie Umowy o Pracę. Proszę czekać...");

    PageDrawer page;
    page.setSaveFileName("Umowa o pracę - " + employeeVariables.at(1) + "_" + employeeVariables.at(3) + ".pdf");
    page.setStartPoint(0, 100);
    page.setVerseOffset(20);
    page.pageStart();

    page.drawBegin(companyVariables.at(3) + ", " + jobContractVariables.at(5));
    page.addSpace(20);
    page.drawHeader("Umowa o Pracę", PageDrawer::Align::CENTER);
    page.drawVerse(jobContractVariables.at(7), PageDrawer::Align::CENTER);
    page.addSpace(20);
    page.drawDataVerse("Zawarta w dniu: ", jobContractVariables.at(5));
    page.addSpace(20);
    page.drawBoldVerse("Między:");
    page.drawBoldVerse(companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " " + companyVariables.at(5));
    page.drawBoldVerse("Reprezentowanego przez: " + companyVariables.at(1));
    page.addSpace(20);
    page.drawBoldVerse("a:");

    drawEmployeeData(page, employeeVariables);

    page.addSpace(30);
    page.setVerseOffset(40);
    page.drawDataNewVerseRect("Rodzaj wykonywanej pracy: ", jobContractVariables.at(0), 546, true);
    page.drawDataNewVerseRect("Miejsce pracy: ", companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " " + companyVariables.at(5), 546, true);
    page.drawDataNewVerseRect("Wynagrodzenie: ", jobContractVariables.at(2) + "zł brutto", 546, true);

    const int weekHours = jobContractVariables.at(4).toInt() * 5;
    if(decision)
        page.setVerseOffset(14);

    page.drawDataNewVerseRect("Ilość godzin: ", jobContractVariables.at(4) + " godzin dziennie, " + QString::number(weekHours) + " godzin tygodniowo", 546, true);

    if(decision){
        page.setVerseOffset(40);
        page.drawInfo("(zgodnie z Ustawą z dnia 27-08-1997r., o rehabilitacji zawodowej i społecznej oraz zatrudniania osób niepełnosprawnych)", PageDrawer::Align::CENTER, 20);
    }

    page.drawDataNewVerseRect("Termin rozpoczęcia: ", jobContractVariables.at(5), 546, true);
    page.setVerseOffset(20);

    page.drawFooter(companyVariables.at(3) + " " + jobContractVariables.at(5));

    progressValue += 10;
    progress->setValue(progressValue);

    drawBHPDeclarationPage(page, employeeVariables, companyVariables.at(3) + ", " + jobContractVariables.at(5), jobContractVariables.at(5),
                           companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " " + companyVariables.at(5));

    drawStartProvisionsPage(page, companyVariables.at(3) + ", " + jobContractVariables.at(5));
    drawEndProvisionsPage(page, companyVariables.at(3) + ", " + jobContractVariables.at(5));
    drawBHPPage(page, companyVariables.at(3) + ", " + jobContractVariables.at(5));
    drawHopePage(page, companyVariables.at(3) + ", " + jobContractVariables.at(5));

    drawHopeDeclarationPage(page, employeeVariables, companyVariables.at(3) + ", " + jobContractVariables.at(5),
                            companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " " + companyVariables.at(5));

    page.pageEnd();
    progressValue = 0;
    progress->reset();
}


void PrintDocuments::printOrderContract(){
    progress->setLabelText("Tworzenie Umowy Zlecenie. Proszę czekać...");

    PageDrawer page;
    page.setSaveFileName("Umowa Zlecenie - " + employeeVariables.at(1) + "_" + employeeVariables.at(3) + ".pdf");
    page.setStartPoint(0, 100);
    page.setVerseOffset(20);
    page.pageStart();

    page.drawBegin(companyVariables.at(3) + ", " + orderContractVariables.at(2));

    page.addSpace(20);
    page.drawHeader("Umowa Zlecenie", PageDrawer::Align::CENTER);
    page.drawVerse(orderContractVariables.at(0), PageDrawer::Align::CENTER);

    page.addSpace(20);
    page.drawDataVerse("Zawarta w dniu:", orderContractVariables.at(2));

    page.addSpace(20);
    page.drawBoldVerse("Między:");
    page.drawBoldVerse(companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " " + companyVariables.at(5));
    page.drawBoldVerse("Reprezentowanego przez: " + companyVariables.at(1));

    page.addSpace(20);
    page.drawBoldVerse("a:");

    drawEmployeeData(page, employeeVariables);

    page.addSpace(15);
    page.drawBoldVerse(QString::fromUtf8("\0xA7") + "1.", PageDrawer::Align::CENTER);
    page.drawVerse("\"Zleceniodawca\" zleca, a \"Zleceniobiorca\" przyjmuję usługę do wykonania polegającą na:");
    page.drawDataSameVerseRect(" ", orderContractVariables.at(6), 300);
    page.drawDataNewVerseRect("przez", orderContractVariables.at(1) + " godzin dziennie", 300);
    page.drawVerse("ze stawką godzinową według najnowszych przepisów obowiązujących na terenie kraju.");

    page.addSpace(15);
    page.drawBoldVerse(QString::fromUtf8("\0xA7") + "2.", PageDrawer::Align::CENTER);
    page.drawDataRect("\"Zleceniodawca\" zobowiązuję się wykonać zlecenie najpóźniej do dnia: ", orderContractVariables.at(3), 200);

    page.addSpace(15);
    page.setFontSize(10);
    page.setVerseOffset(12);
    page.drawBoldVerse("3.", PageDrawer::Align::CENTER);
    page.drawVerse("Za prawidłowe wykonanie zlecenia określonego w \"1.\". Zleceniobiorcy przysługuje wynagrodzenie w wysokości wyliczonego");
    page.drawVerse("\"Rachunku do umowy zlecenia\" opracowanego przez Zleceniobiorcę.");

    page.addSpace(15);
    page.drawBoldVerse("4.", PageDrawer::Align::CENTER);
    page.drawVerse("Wynagrodzenie za wykonanie zlecenia wypłacone zostanie na podstawie rachunku wystawionego co miesiąc, w którym umowa");
    page.drawVerse("obowiązuję przez \"Zleceniobiorcę\" i dostarczonego do \"Zleceniodawcy\". Wynagrodzenie zostanie przelane na wskazane");
    page.drawVerse("konto lub płatne w \"kasie\" firmy \"Zleceniobiorcy\" tj. " + companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " Ul." + companyVariables.at(5) + ",");
    page.drawVerse("\"Zleceniobiorcy\" najpóźniej w ciągu 14 dni od otrzymania rachunku przez \"Zleceniodawce\". Czas pracy będzie opracowywany");
    page.drawVerse("tzw.: \"Ewidenca czasu pracy\". Wypłata będzie wyliczana w akordzi na co \"Zleceniobiorca\" i \"Zleceniodawca\" wyraża zgodę");
    page.drawVerse("podpisując umowę.");

    page.addSpace(15);
    page.drawBoldVerse("5.", PageDrawer::Align::CENTER);
    page.drawVerse("W prawach nieuregulowanych stosuje się przepisy Kodeksu cywilnego i innych obowiązaującyh aktów prawnych.");

    page.addSpace(15);
    page.drawBoldVerse("6.", PageDrawer::Align::CENTER);
    page.drawVerse("Zmiany w umowie wymagają formy pisemnej pod rygorem nie ważności obowiązującej umowy.");

    page.addSpace(15);
    page.drawBoldVerse("7.", PageDrawer::Align::CENTER);
    page.drawVerse("Umowa została sporządzona w 2 jednobrzmiących egzemplarzach, po jednym dla każdej ze stron. Dołączone do umowy załączniki");
    page.drawVerse("stanowią integralną część umowy.");

    page.setVerseOffset(20);
    page.setFontSize(12);

    page.drawFooter(companyVariables.at(3) + ", " + orderContractVariables.at(2));

    progressValue += 10;
    progress->setValue(progressValue);

    drawBHPDeclarationPage(page, employeeVariables, companyVariables.at(3) + ", " + orderContractVariables.at(2), orderContractVariables.at(2),
                           companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " " + companyVariables.at(5));

    drawStartProvisionsPage(page, companyVariables.at(3) + ", " + orderContractVariables.at(2));
    drawEndProvisionsPage(page, companyVariables.at(3) + ", " + orderContractVariables.at(2));
    drawBHPPage(page, companyVariables.at(3) + ", " + orderContractVariables.at(2));
    drawHopePage(page, companyVariables.at(3) + ", " + orderContractVariables.at(2));

    drawHopeDeclarationPage(page, employeeVariables, companyVariables.at(3) + ", " + orderContractVariables.at(2),
                            companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " " + companyVariables.at(5));

    page.pageEnd();
    progressValue = 0;
    progress->reset();
}


void PrintDocuments::printWorkContract(){
    progress->setLabelText("Tworzenie Umowy o Dzieło. Proszę czekać...");

    PageDrawer page;
    page.setSaveFileName("Umowa o Dzieło - " + employeeVariables.at(1) + "_" + employeeVariables.at(3) + ".pdf");
    page.setStartPoint(0, 100);
    page.setVerseOffset(20);
    page.pageStart();

    page.drawBegin(companyVariables.at(3) + ", " + workContractVariables.at(4));

    page.addSpace(20);
    page.drawHeader("Umowa o Dzieło", PageDrawer::Align::CENTER);
    page.drawVerse(workContractVariables.at(0), PageDrawer::Align::CENTER);

    page.addSpace(20);
    page.drawDataVerse("Zawarta w dniu:", workContractVariables.at(4));

    page.addSpace(20);
    page.drawBoldVerse("Między:");
    page.drawBoldVerse(companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " " + companyVariables.at(5));
    page.drawBoldVerse("Reprezentowanego przez: " + companyVariables.at(1));

    page.addSpace(20);
    page.drawBoldVerse("a:");

    drawEmployeeData(page, employeeVariables);

    page.addSpace(20);
    page.drawVerse("\"Zamawiający\" zamawia, a \"Wykonawca\" wykonuję prace polegające na:");

    QStringList jobs = workContractVariables.at(2).split(",");
    for(int i = 0; i < jobs.size(); ++i){
        page.drawDataNewVerseRect(QString::number(i), jobs.at(i), 600);
    }
    page.drawDataRect("Strony ustalają termin realizacji dzieła na dzień: ", workContractVariables.at(4), 200);

    page.addSpace(20);
    page.drawVerse("Do wykonania dzieła \"Zamawiający\" powierza:");
    QStringList trusts = workContractVariables.at(3).split(",");
    for(int i = 0; i < trusts.size(); ++i){
        page.drawDataNewVerseRect(QString::number(i), trusts.at(i), 600);
    }
    page.drawDataRect("Za wykonanie \"Zamawiający\" zapłaci \"Wykonującemu\" kwotę:", workContractVariables.at(1), 200);
    page.drawVerse("w terminie 7 dni od otrzymania \"Rachunku do umowy\" od \"Wykonującego\"");

    page.drawFooter(companyVariables.at(3) + ", " + workContractVariables.at(4));

    progressValue += 55;
    progress->setValue(progressValue);

    drawEndProvisionsPage(page, companyVariables.at(3) + ", " + workContractVariables.at(4));
    drawHopePage(page, companyVariables.at(3) + ", " + workContractVariables.at(4));

    drawHopeDeclarationPage(page, employeeVariables, companyVariables.at(3) + ", " + workContractVariables.at(4),
                            companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " " + companyVariables.at(5));

    page.pageEnd();
    progressValue = 0;
    progress->reset();
}


void PrintDocuments::printSickLeaveEvidence(const QString& date, QVector<Data::EvidenceRecord>& records){
    progress->setLabelText("Tworzenie Ewidencji zwolnień chorobowych.");

    PageDrawer page;
    page.setSaveFileName("Ewidencja zwolnień chorobowych - " + date + ".pdf");
    page.setStartPoint(0, 100);
    page.setVerseOffset(20);
    page.setColumnsCount(7);
    page.setTableOffset(50);
    page.pageStart();

    page.drawBegin(companyVariables.at(3) + ", " + QDate::currentDate().toString("dd.MM.yyyy"));

    progress->setValue(25);

    page.addSpace(20);
    page.drawHeader("Ewidencja zwolnień chorobowych", PageDrawer::Align::CENTER);
    page.drawVerse(date, PageDrawer::Align::CENTER);

    progress->setValue(50);

    page.addSpace(20);
    page.drawCell("Lp.", 60);
    page.drawCell("Imię", 80);
    page.drawCell("Nazwisko", 80);
    page.drawCell("Pesel", 100);
    page.drawCell("Data rozpoczęcia", 120);
    page.drawCell("Data zakończenia", 120);
    page.drawCell("Ilość dni", 100);

    progress->setValue(75);

    for(int i = 0; i < records.size(); ++i){
        page.drawCell(QString::number(i), 60);
        page.drawCell(records.at(i).name, 80);
        page.drawCell(records.at(i).surname, 80);
        page.drawCell(records.at(i).pesel, 100);
        page.drawCell(records.at(i).startDate.toString("dd.MM.yyyy"), 120);
        page.drawCell(records.at(i).endDate.toString("dd.MM.yyyy"), 120);
        page.drawCell(QString::number(records.at(i).daysCount), 100);
    }

    progress->setValue(100);

    page.pageEnd();
    progress->reset();
}


void PrintDocuments::printFreeLeaveEvidence(const QString& date, QVector<Data::EvidenceRecord>& records){
    progress->setLabelText("Tworzenie Ewidencji urlopów.");

    PageDrawer page;
    page.setSaveFileName("Ewidencja urlopów - " + date + ".pdf");
    page.setStartPoint(0, 100);
    page.setVerseOffset(20);
    page.setColumnsCount(8);
    page.setTableOffset(-5);
    page.pageStart();

    page.drawBegin(companyVariables.at(3) + ", " + QDate::currentDate().toString("dd.MM.yyyy"));

    progress->setValue(25);

    page.addSpace(20);
    page.drawHeader("Ewidencja urlopów", PageDrawer::Align::CENTER);
    page.drawVerse(date, PageDrawer::Align::CENTER);

    progress->setValue(50);

    page.addSpace(20);
    page.drawCell("Lp.", 60);
    page.drawCell("Imię", 80);
    page.drawCell("Nazwisko", 80);
    page.drawCell("Pesel", 100);
    page.drawCell("Data rozpoczęcia", 120);
    page.drawCell("Data zakończenia", 120);
    page.drawCell("Ilość dni", 100);
    page.drawCell("Rodzaj urlopu", 120);

    progress->setValue(75);

    for(int i = 0; i < records.size(); ++i){
        page.drawCell(QString::number(i), 60);
        page.drawCell(records.at(i).name, 80);
        page.drawCell(records.at(i).surname, 80);
        page.drawCell(records.at(i).pesel, 100);
        page.drawCell(records.at(i).startDate.toString("dd.MM.yyyy"), 120);
        page.drawCell(records.at(i).endDate.toString("dd.MM.yyyy"), 120);
        page.drawCell(QString::number(records.at(i).daysCount), 100);
        page.drawCell(records.at(i).type, 120);
    }

    progress->setValue(100);

    page.pageEnd();
    progress->reset();
}


void PrintDocuments::printJobTimeEvidence(const QString& date, const Data::JobEvidenceRecord& record){
    PageDrawer page;
    page.setStartPoint(0, 100);
    page.setSaveFileName("ECP - " + record.type + " " + record.name + "_" + record.surname + " - " + record.month + ", " + record.year + ".pdf");
    page.setVerseOffset(20);
    page.setColumnsCount(2);
    page.setTableOffset(280);
    page.pageStart();

    page.drawBegin(companyVariables.at(3) + ", " + QDate::currentDate().toString("dd.MM.yyyy"));

    page.addSpace(20);
    page.drawHeader("Ewidencja czasu pracy", PageDrawer::Align::CENTER);
    page.drawVerse(date, PageDrawer::Align::CENTER);

    page.addSpace(20);
    page.drawDataNewVerseRect("Numer Pracownika:", QString::number(record.employeeId), 100, true);
    page.drawDataNewVerseRect("Imię i nazwisko:", record.name + " " + record.surname, 150, true);
    page.drawDataNewVerseRect("Rodzaj umowy:", record.type, 150, true);

    page.addSpace(20);
    page.drawCell("Lp.", 100);
    page.drawCell("Ilość godzin", 100);

    QStringList days = record.days.split(",");
    QStringList hours = record.hours.split(",");
    QStringList showHours {"2", "4", "6", "7", "8", "9", "10", "Urlop wyp.", "Urlop rodz.", "Urlop bezpł.", "Chorobowe"};

    for(int i = 0; i < days.size(); ++i){
        page.drawCell(days.at(i), 100);
        page.drawCell(showHours.at(hours.at(i).toInt()), 100);
    }

    page.addSpace(20);
    page.drawDataNewVerseRect("Razem godzin:", QString::number(record.sum), 100);
    page.drawFooter(companyVariables.at(3) + ", " + QDate::currentDate().toString("dd.MM.yyyy"));

    page.pageEnd();
}


void PrintDocuments::printAnnex(){
    PageDrawer page;
    page.setSaveFileName("Aneks do " + annexVariables.at(1) + " - " + employeeVariables.at(1) + " " + employeeVariables.at(3) + ".pdf");
    page.setStartPoint(0, 100);
    page.setVerseOffset(20);
    page.setColumnsCount(2);
    page.setTableOffset(90);
    page.pageStart();
    page.drawBegin(companyVariables.at(3) + ", " + annexVariables.at(2));

    page.addSpace(20);
    page.drawHeader("Aneks do " + annexVariables.at(1), PageDrawer::Align::CENTER);
    page.drawVerse(annexVariables.at(0), PageDrawer::Align::CENTER);

    page.addSpace(20);
    page.drawBoldVerse("Między:");
    page.drawVerse(companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " " + companyVariables.at(5));
    page.drawVerse("reprezentowanego przez: Piotr Strauss - specjalistę ds. administracyjnych i kadr.");

    progress->setValue(25);

    page.addSpace(20);
    page.drawBoldVerse("a:");
    drawEmployeeData(page, employeeVariables);

    page.addSpace(20);
    page.drawBoldVerse("Strony wprowadzają następujące zmiany:");
    page.drawDataNewVerseRect("Na mocy tego aneksu do " + annexVariables.at(1) + " z dnia:", annexVariables.at(2), 120);

    progress->setValue(50);

    page.addSpace(20);
    page.drawCell("Lp.", 100);
    page.drawCell("Zmiany", 500);

    QStringList changes = annexVariables.at(3).split(",");
    for(int i = 0; i < changes.size(); ++i){
        page.drawCell(QString::number(i + 1), 100);
        page.drawCell(changes.at(i), 500);
    }

    progress->setValue(75);

    page.addSpace(20);
    page.drawDataNewVerseRect("Aneks wchodzi w życie z dniem:", annexVariables.at(2), 120);
    page.drawFooter(companyVariables.at(3) + ", " + annexVariables.at(2));

    progress->setValue(100);

    page.pageEnd();
    progress->reset();
}


void PrintDocuments::printOrderBill(int hoursSum, int daysSum, int freeDaysSum, int sickDaysSum){
    QString fileName{""};
    PageDrawer page;
    if(orderBillVariables.at(21) == "uz") //Typ rachunku
        fileName = "Rachunek do umowy zlecenie nr." + orderBillVariables.at(22) + " - ";
    else if(orderBillVariables.at(21) == "ud")
        fileName = "Rachunek do umowy o dzieło nr." + orderBillVariables.at(22) + " - ";
    else if(orderBillVariables.at(21) == "uop")
        fileName = "Lista płac nr." + orderBillVariables.at(22) + " - ";

    page.setSaveFileName(fileName + employeeVariables.at(1) + " " + employeeVariables.at(3) + ".pdf");
    page.setStartPoint(0, 100);
    page.setVerseOffset(20);
    page.pageStart();
    page.drawBegin(companyVariables.at(3) + ", " + orderBillVariables.at(20));

    page.addSpace(20);
    if(orderBillVariables.at(21) == "uz"){
        page.drawHeader("Rachunek do umowy zelecenia", PageDrawer::Align::CENTER);
        page.drawVerse("Nr. " + orderBillVariables.at(0) + " do " + orderContractVariables.at(0), PageDrawer::Align::CENTER);
    }
    else if(orderBillVariables.at(21) == "ud"){
        page.drawHeader("Rachunek do umowy o dzieło", PageDrawer::Align::CENTER);
        page.drawVerse("Nr. " + orderBillVariables.at(0) + " do " + workContractVariables.at(0), PageDrawer::Align::CENTER);
    }
    else if(orderBillVariables.at(21) == "ud"){
        page.drawHeader("Lista płac", PageDrawer::Align::CENTER);
        page.drawVerse("Nr. " + orderBillVariables.at(0) + " do " + jobContractVariables.at(0), PageDrawer::Align::CENTER);
    }

    page.addSpace(20);
    page.drawBoldVerse("Wystawił:");
    drawEmployeeData(page, employeeVariables);

    page.addSpace(20);
    page.drawDataNewVerseRect("Urząd Skarbowy:", companyVariables.at(7), 400);

    page.addSpace(20);
    if(orderBillVariables.at(21) != "uop"){
        page.drawDataNewVerseRect("Rachunek wystawiono dla:",
                                companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " " + companyVariables.at(5) + " NIP: " + companyVariables.at(2) + " REGON:" + companyVariables.at(6),
                                550);
    }
    else{
        page.drawDataNewVerseRect("Dane pracodawcy:",
                                  companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " " + companyVariables.at(5) + " NIP: " + companyVariables.at(2) + " REGON:" + companyVariables.at(6),
                                  550);
    }

    progress->setValue(25);

    page.addSpace(20);
    page.drawDataNewVerseRect("Wynagrodzenie brutto:", QString::number(orderBillVariables.at(1).toFloat()) + " zł", 150, true);

    if(orderBillVariables.at(21) == "uop"){
        page.drawDataNewVerseRect("Wyna. zasadnicze brutto:", QString::number(orderBillVariables.at(23).toFloat()) + " zł", 150, true);
        page.drawDataNewVerseRect("Wyna. urlopowe:", QString::number(orderBillVariables.at(24).toFloat()) + " zł", 150, true);
        page.drawDataNewVerseRect("Zasiłek chorobowy:", QString::number(orderBillVariables.at(25).toFloat()) + " zł", 150, true);
    }

    page.drawDataSameVerseRect("Ub. emerytalne(pracownik):", QString::number(orderBillVariables.at(2).toFloat()) + " zł", 150, true);
    page.drawDataNewVerseRect("Ub. emerytalne(pracodawca):", QString::number(orderBillVariables.at(6).toFloat()) + " zł", 150, true);
    page.drawDataSameVerseRect("Ub. rentowe(pracownik):", QString::number(orderBillVariables.at(3).toFloat()) + " zł", 150, true);
    page.drawDataNewVerseRect("Ub. rentowe(pracodawca):", QString::number(orderBillVariables.at(7).toFloat()) + " zł", 150, true);
    page.drawDataSameVerseRect("Ub. chorobowe(pracownik):", QString::number(orderBillVariables.at(4).toFloat()) + " zł", 150, true);
    page.drawDataNewVerseRect("Ub. wypadkowe(pracodawca):", QString::number(orderBillVariables.at(8).toFloat()) + " zł", 150, true);
    page.drawDataSameVerseRect("Suma składek(pracownik):", QString::number(orderBillVariables.at(5).toFloat()) + " zł", 150, true);
    page.drawDataNewVerseRect("Suma składek(pracodawca):", QString::number(orderBillVariables.at(9).toFloat()) + " zł", 150, true);
    page.drawDataSameVerseRect("Koszt uzyskania przychodu:", QString::number(orderBillVariables.at(10).toFloat()) + " zł", 150, true);
    page.drawDataNewVerseRect("Ub. zdrowotne(9%):", QString::number(orderBillVariables.at(13).toFloat()) + " zł", 150, true);
    page.drawDataSameVerseRect("Dochód do opodatkowania:", QString::number(orderBillVariables.at(11).toFloat()) + " zł", 150, true);
    page.drawDataNewVerseRect("Ub. zdrowotne(7,75%):", QString::number(orderBillVariables.at(14).toFloat()) + " zł", 150, true);
    page.drawDataSameVerseRect("Podatek:", QString::number(orderBillVariables.at(12).toFloat()) + " zł", 150, true);
    page.drawDataNewVerseRect("Ub. zdrowotne(1,25%):", QString::number(orderBillVariables.at(15).toFloat()) + " zł", 150, true);
    page.drawDataSameVerseRect("Fundusz Pracy:", QString::number(orderBillVariables.at(16).toFloat()) + " zł", 150, true);
    page.drawDataNewVerseRect("Netto do wypłaty:", QString::number(orderBillVariables.at(18).toFloat()) + " zł", 150, true);
    page.drawDataNewVerseRect("FGŚP:", QString::number(orderBillVariables.at(17).toFloat()) + " zł", 150, true);

    progress->setValue(50);

    if(orderBillVariables.at(21) == "uz"){
        page.addSpace(15);
        page.setVerseOffset(12);
        page.drawBoldVerse("OŚWIADCZENIA I PODPIS ZLECENIOBIORCY(zaznaczyć właściwe):");
        page.drawInfo("1.Oświadczam, iż nie jestem zarejestrowany jako osoba bezrobotna w Urzędzie Pracy.");
        page.drawInfo("2.Oświadczam, iż w zakresie przedmiotu umowy nie prowadzę działalności gospodarczej w rozumieniu ustawy o podatku dochodowym od osób fizycznych.");
        page.drawInfo("3.Załączam do rachunku wniosek o objęcie dobrowolnym ubezpieczeniem chorobowym.");
        page.drawInfo("4.Oświadczam, iż łączna kwota moich przychodów, stanowiących podstawę do naliczenia składek na ubezpieczenia społeczne, jest w przeliczeniu na");
        page.drawInfo("pełen miesiąc niższa od minimalnego wynagrodzenia za pracę.");
        page.drawInfo("5.Oświadczam, iż jestem uczniem/studentem i nie ukończyłem 26 roku życia.");
        page.drawInfo("6.Oświadczam, iż posiadam inny tytuł ubezpieczenia, a kwota brutto wynagrodzenia z tego tytyłu jest wyższa od minimalnego wynagrodzenia za pracę.");

        page.addSpace(15);
        page.drawBoldVerse("OŚWIADCZENIA I PODPIS ZLECENIODAWCY");
        page.drawInfo("1.Stwierdzam wykonanie zlecenia.");
        page.drawInfo("2.Zatwierdzam do wypłaty.");
    }

    progress->setValue(75);

    page.addSpace(15);
    page.setVerseOffset(20);
    page.drawBoldVerse("Rozliczenie:");
    if(orderBillVariables.at(21) == "uop"){
        page.drawDataRect("Liczba dni(Urlop):", QString::number(freeDaysSum), 100, 200);
        page.drawDataRect("Liczba dni(Chorobowe):", QString::number(sickDaysSum), 100, 200);
    }
    if(orderBillVariables.at(21) == "uz"){
        page.drawDataRect("Liczba przepracowanych dni:", QString::number(daysSum), 100, 200);
        page.drawDataRect("Liczba godzin:", QString::number(hoursSum), 100, 200);
        page.drawDataRect("Stawka na godzinę:", QString::number(orderContractVariables.at(4).toFloat()) + " zł", 100, 200);
    }
    page.drawDataRect("Sposób płatności:", orderBillVariables.at(19), 100, 200);

    if(orderBillVariables.at(21) == "uz")
        page.addSpace(40);
    else
        page.addSpace(100);
    page.drawOneLineVerses("Potwierdzam odbiór kwoty " + QString::number(orderBillVariables.at(18).toFloat()) + " zł", "Podpis zleceniodawcy");

    progress->setValue(100);

    page.pageEnd();
    progress->reset();
}


void PrintDocuments::printJobListBill(QVector<Data::BillRecord>& records){
    PageDrawer page;
    page.setSaveFileName("Zbiorcza Lista Płac - " + records.at(0).date.toString("dd.MM.yyyy") + ".pdf");
    page.setStartPoint(30, 100);
    page.setVerseOffset(20);
    page.setOrientation(QPageLayout::Orientation::Landscape);
    page.setColumnsCount(17);
    page.setTableOffset(25);
    page.pageStart();

    QDate date = records.at(0).date;
    page.drawHeader("Lista płac nr. " + QString::number(date.month()) + "/" + QString::number(date.year()), PageDrawer::Align::CENTER);
    page.drawVerse("za miesiąc " + date.toString("MMMM, yyyy"), PageDrawer::Align::CENTER);

    page.addSpace(20);
    page.drawDataNewVerseRect("Firma: ", companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " " + companyVariables.at(5), 600);
    page.setStartPoint(30, page.getPenPosition().y());
    page.drawDataNewVerseRect("NIP:  ", companyVariables.at(2), 200);
    page.setStartPoint(30, page.getPenPosition().y());
    page.drawDataNewVerseRect("REGON: ", companyVariables.at(6), 200);

    progress->setValue(10);

    page.setFontSize(6);
    page.setVerseOffset(41);
    page.drawCell("Lp.", 30, 30, true);
    page.drawCell("Pracownik", 60, 30, true);
    page.drawCell("Wyn.\nZasad.", 60, 30, true);
    page.drawCell("Wyn.\nChorob.", 60, 30, true);
    page.drawCell("Wyn.\nUrlop.", 60, 30, true);
    page.drawCell("Razem", 60, 30, true);
    page.drawCell("Ubez.\nEmer.", 60, 30, true);
    page.drawCell("Ubez.\nRent.", 60, 30, true);
    page.drawCell("Ubez.\nChorob.", 60, 30, true);
    page.drawCell("Razem", 60, 30, true);
    page.drawCell("KUP", 60, 30, true);
    page.drawCell("Zrow.\nOdliczane", 60, 30, true);
    page.drawCell("Ubez.\nNie odliczane", 60, 30, true);
    page.drawCell("Razem", 60, 30, true);
    page.drawCell("Zaliczka\nPIT", 60, 30, true);
    page.drawCell("Do\nWypłaty", 60, 30, true);
    page.drawCell("Podpis", 100, 30, true);

    int currentPage{0};
    int recordsOnPage{0};
    for(int i = 0 ; i < records.size(); ++i){
        page.drawCell(QString::number(i + 1), 30, 30);
        page.drawCell(records.at(i).employeeName, 60, 30);
        page.drawCell(QString::number(records.at(i).basicBrutto), 60, 30);
        page.drawCell(QString::number(records.at(i).sickBrutto), 60, 30);
        page.drawCell(QString::number(records.at(i).freeBrutto), 60, 30);
        page.drawCell(QString::number(records.at(i).rewardBrutto), 60, 30);
        page.drawCell(QString::number(records.at(i).iOld), 60, 30);
        page.drawCell(QString::number(records.at(i).iRent), 60, 30);
        page.drawCell(QString::number(records.at(i).iDisease), 60, 30);
        page.drawCell(QString::number(records.at(i).iSum), 60, 30);
        page.drawCell(QString::number(records.at(i).incomeCost), 60, 30);
        page.drawCell(QString::number(records.at(i).insuranceAdd), 60, 30);
        page.drawCell(QString::number(records.at(i).insuranceSub), 60, 30);
        page.drawCell(QString::number(records.at(i).insurance), 60, 30);
        page.drawCell(QString::number(records.at(i).rate), 60, 30);
        page.drawCell(QString::number(records.at(i).rewardNetto), 60, 30);
        page.drawCell(records.at(i).payType + " DN " + QDate::currentDate().toString("dd.MM.yyyy"), 100, 30);

        if(currentPage == 0){
            if(recordsOnPage == 10){
                page.toNextPage();
                page.setStartPoint(0, 20);
                currentPage++;
                recordsOnPage = 0;
            }
        }
        else{
            if(recordsOnPage == 18 && i != records.size() - 1){
                page.toNextPage();
                page.setStartPoint(0, 20);
                currentPage++;
                recordsOnPage = 0;
            }
        }

        recordsOnPage++;
    }

    progress->setValue(40);

    page.toNextPage();
    page.resetCells();
    page.setStartPoint(0, 20);
    page.setColumnsCount(9);

    page.drawCell("Dane narastająco z poprzednich stron", 150, 30, true);
    page.drawCell("Składki\nEmer.", 60, 30, true);
    page.drawCell("Składki\nRent.", 60, 30, true);
    page.drawCell("Składki\nChorob.", 60, 30, true);
    page.drawCell("Składki\nWyp.", 60, 30, true);
    page.drawCell("Składki\nZdrow.", 60, 30, true);
    page.drawCell("Fundusz\nPracy", 60, 30, true);
    page.drawCell("FGŚP", 60, 30, true);
    page.drawCell("Zaliczka\nPIT", 60, 30, true);

    const auto getSum = [&](int index){
        float sum{0.0f};
        for(auto& record : records){
            switch(index){
            case 0: sum += record.iOld; break;
            case 1: sum += record.iRent; break;
            case 2: sum += record.iDisease; break;
            case 3: sum += record.iOldP; break;
            case 4: sum += record.iRentP; break;
            case 5: sum += record.iAccident; break;
            case 6: sum += record.insurance; break;
            case 7: sum += record.jobFund; break;
            case 8: sum += record.fgsp; break;
            case 9: sum += record.rate; break;
            case 10: sum += record.rewardNetto; break;
            }
        }

        return QString::number(sum);
    };

    page.setVerseOffset(21);
    page.drawCell("Potrącenia należne one pracowników", 150, 10);
    page.drawCell(getSum(0), 60, 10);
    page.drawCell(getSum(1), 60, 10);
    page.drawCell(getSum(2), 60, 10);
    page.drawCell("0", 60, 10);
    page.drawCell(getSum(6), 60, 10);
    page.drawCell("0", 60, 10);
    page.drawCell("0", 60, 10);
    page.drawCell(getSum(9), 60, 10);

    page.resetCells();
    page.setColumnsCount(8);

    progress->setValue(50);

    page.drawCell("Składki należne od pracowników", 150, 10);
    page.drawCell(getSum(3), 60, 10);
    page.drawCell(getSum(4), 60, 10);
    page.drawCell("0", 60, 10);
    page.drawCell(getSum(5), 60, 10);
    page.drawCell("0", 60, 10);
    page.drawCell(getSum(7), 60, 10);
    page.drawCell(getSum(8), 60, 10);

    page.resetCells();
    page.setColumnsCount(2);

    progress->setValue(60);

    page.drawCell("Stawka ubezpieczenia wyp. [%]", 150, 10);
    page.drawCell("0", 60, 10);

    page.drawCell("Wynagrodzenie netto", 150, 10);
    page.drawCell(getSum(10), 60, 10);

    progress->setValue(80);

    page.setStartPoint(30, page.getPenPosition().y());

    page.addSpace(40);
    page.drawBoldVerse("Sporządził(data i podpis):");

    page.addSpace(20);
    page.drawBoldVerse("Zatwierdził(data i podpis):");

    page.addSpace(20);
    page.drawBoldVerse("Wypłacił(data i podpis):");

    page.pageEnd();
    progress->setValue(100);
}


void PrintDocuments::printJobBill(QVector<Data::BillRecord> &records){
    QString empId{""};
    QString contractId{""};
    QString eviId{""};

    QString employeeName{""}; //employees
    QString pesel{""}; //employees
    QString jobTime{""}; //jobContracts
    QString jobReward{""}; //jobContracts
    QString daysCount{""}; //jobEvidence
    QString freeDaysCount{""}; //jobEvidence
    QString sickDaysCount{""}; //jobEvidence
    QString hoursSum{""}; //jobEvidence;

    PageDrawer page;
    page.setStartPoint(0, 100);
    page.setVerseOffset(20);
    page.setSaveFileName("Paski wynagrodzeń - " + records.at(0).date.toString("MMMM.yyyy") + ".pdf");
    page.pageStart();
    page.setFontSize(8);

    int currentBill{1};
    int posY{page.getPenPosition().y()};
    QSqlQuery query;
    for(int i = 0; i < records.size(); ++i){
        query.exec("SELECT contractId, jobEvidenceId FROM bills WHERE id = " + QString::number(records.at(i).id));
        if(query.next()){
            contractId = query.value(0).toString();
            eviId = query.value(1).toString();
        }

        query.exec("SELECT employeeId, reward, jobDimension FROM jobContracts WHERE docId = " + contractId);
        if(query.next()){
            empId = query.value(0).toString();
            jobReward = query.value(1).toString();
            jobTime = query.value(2).toString();
        }

        query.exec("SELECT name, surname, pesel FROM employees WHERE id = " + empId);
        if(query.next()){
            employeeName = query.value(0).toString() + " " + query.value(1).toString();
            pesel = query.value(2).toString();
        }

        query.exec("SELECT daysSum, freeDaysSum, sickDaysSum, sum FROM jobTimeEvidence WHERE id = " + eviId);
        if(query.next()){
            daysCount = query.value(0).toString();
            freeDaysCount = query.value(1).toString();
            sickDaysCount = query.value(2).toString();
            hoursSum = query.value(3).toString();
        }

        if(currentBill == 5){
            page.toNextPage();
            posY = 20;
        }
        page.setStartPoint(0, posY);

        page.setColumnsCount(4);
        page.drawCell("Pracownik: " + employeeName + "\n" +
                      "PESEL: " + pesel + "\n" +
                      "Wymiar etatu: " + jobTime + "\n" +
                      "Stawka: " + jobReward + " PLN", 200, 70);

        page.drawCell("Wynag. zasadnicze: " + QString::number(records.at(i).basicBrutto) + "\n" +
                      "Wynag. urlopowe: " + QString::number(records.at(i).freeBrutto) + "\n" +
                      "Wynag. chorobowe: " + QString::number(records.at(i).sickBrutto) + "\n" +
                      "Razem: " + QString::number(records.at(i).rewardBrutto), 200, 70);

        page.drawCell("Dni pracy: " + daysCount + "\n" +
                      "Dni urlopu: " + freeDaysCount + "\n" +
                      "Dni chorobowe: " + sickDaysCount + "\n" +
                      "Czas pracy: " + hoursSum, 200, 70);

        page.drawCell("Podpis: ........................\n"
                      "Data wyst.: " + QDate::currentDate().toString("dd.MM.yyyy"), 150, 70);

        posY += 85;

        page.resetCells();
        page.setColumnsCount(2);
        page.setStartPoint(0, posY);
        page.drawCell("", 100, 10);
        page.drawCell("Składki", 650, 10);

        posY += 25;

        page.resetCells();
        page.setColumnsCount(10);
        page.setStartPoint(0, posY);
        page.drawCell("", 100, 10);
        page.drawCell("Emerty.", 72, 10);
        page.drawCell("Rent.", 72, 10);
        page.drawCell("Chor.", 72, 10);
        page.drawCell("Wyp.", 72, 10);
        page.drawCell("FP.", 72, 10);
        page.drawCell("FGŚP.", 72, 10);
        page.drawCell("Zdr. (Odl.)", 72, 10);
        page.drawCell("Zdr. (Nie.)", 74, 10);
        page.drawCell("Razem", 72, 10);

        posY += 25;

        page.resetCells();
        page.setStartPoint(0, posY);
        page.drawCell("Pracownik", 100, 10);
        page.drawCell(QString::number(records.at(i).iOld), 72, 10);
        page.drawCell(QString::number(records.at(i).iRent), 72, 10);
        page.drawCell(QString::number(records.at(i).iDisease), 72, 10);
        page.drawCell("", 72, 10);
        page.drawCell("", 72, 10);
        page.drawCell("", 72, 10);
        page.drawCell(QString::number(records.at(i).insuranceAdd), 72, 10);
        page.drawCell(QString::number(records.at(i).insuranceSub), 74, 10);
        page.drawCell(QString::number(records.at(i).insurance), 72, 10);

        posY += 25;

        page.resetCells();
        page.setStartPoint(0, posY);
        page.drawCell("Pracodawca", 100, 10);
        page.drawCell(QString::number(records.at(i).iOldP), 72, 10);
        page.drawCell(QString::number(records.at(i).iRentP), 72, 10);
        page.drawCell("", 72, 10);
        page.drawCell(QString::number(records.at(i).iAccident), 72, 10);
        page.drawCell(QString::number(records.at(i).jobFund), 72, 10);
        page.drawCell(QString::number(records.at(i).fgsp), 72, 10);
        page.drawCell("", 72, 10);
        page.drawCell("", 74, 10);
        page.drawCell(QString::number(records.at(i).iOldP + records.at(i).iRentP + records.at(i).iAccident + records.at(i).jobFund + records.at(i).fgsp), 72, 10);

        posY += 25;

        page.resetCells();
        page.setStartPoint(0, posY);
        page.setColumnsCount(4);
        page.drawCell("Zaliczka PIT: " + QString::number(records.at(i).rate), 187, 10);
        page.drawCell("KUP: " + QString::number(records.at(i).incomeCost), 187, 10);
        page.drawCell("Podst. Zdrow.: " + QString::number(records.at(i).incomeCostAfter), 187, 10);
        page.drawCell("Wynag. Netto: " + QString::number(records.at(i).rewardNetto), 189, 10);

        posY += 25;
        page.setStartPoint(0, posY);
        page.drawVerse("Osoba wystawiająca: Piotr Strauss");

        posY += 30;
        currentBill++;
    }

    page.pageEnd();
}


void PrintDocuments::printPayCertificate(QVector<Data::BillRecord> &records){
    PageDrawer page;
    page.setStartPoint(0, 100);
    page.setVerseOffset(20);
    page.setSaveFileName("Zaświadczenie o zatrudnieniu - " + employeeVariables.at(1) + " " + employeeVariables.at(3) + ".pdf");
    page.pageStart();
    page.drawBegin(companyVariables.at(3) + ", " + QDate::currentDate().toString("dd.MM.yyyy"));

    page.addSpace(40);
    page.drawHeader("Zaświadczenie o zatrudnieniu", PageDrawer::Align::CENTER);

    page.addSpace(20);
    page.drawBoldVerse("Zaświadcza się że:");

    page.addSpace(20);
    drawEmployeeData(page, employeeVariables);

    page.addSpace(20);
    page.drawDataNewVerseRect("Zatrudniony/a: ", companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " " + companyVariables.at(5), 500);

    if(records.at(0).payType == "uop"){
        page.drawDataSameVerseRect("Od dnia: ", jobContractVariables.at(5), 200);
        page.drawDataNewVerseRect("do dnia: ", jobContractVariables.at(6), 200);
    }
    else if(records.at(0).payType == "uz"){
        page.drawDataSameVerseRect("Od dnia: ", orderContractVariables.at(2), 200);
        page.drawDataNewVerseRect("do dnia: ", orderContractVariables.at(3), 200);
    }
    else if(records.at(0).payType == "ud"){
        page.drawDataSameVerseRect("Od dnia: ", workContractVariables.at(4), 200);
        page.drawDataNewVerseRect("do dnia: ", workContractVariables.at(5), 200);
    }

    progress->setValue(25);

    page.addSpace(20);
    page.drawBoldVerse("W następujących miesiącach otrzymał/a wynagrodzenie:");

    page.addSpace(20);
    page.setColumnsCount(5);
    page.setVerseOffset(30);
    page.drawCell("Miesiąc, Rok", 120, 10, true);
    page.drawCell("Kwota Brutto", 120, 10, true);
    page.drawCell("Składki ZUS", 120, 10, true);
    page.drawCell("Zaliczka PIT", 120, 10, true);
    page.drawCell("Kwota Netto", 120, 10, true);

    for(int i = 0 ; i < records.size(); ++i){
        page.drawCell(records.at(i).docId, 120, 10);
        page.drawCell(QString::number(records.at(i).rewardBrutto), 120, 10);
        page.drawCell(QString::number(records.at(i).iSum), 120, 10);
        page.drawCell(QString::number(records.at(i).rate), 120, 10);
        page.drawCell(QString::number(records.at(i).rewardNetto), 120, 10);
    }

    progress->setValue(50);

    const auto getSum = [&](int index){
        float sum{0.0f};
        for(auto& record : records){
            switch(index){
            case 0: sum += record.rewardBrutto; break;
            case 1: sum += record.iSum; break;
            case 2: sum += record.rate; break;
            case 3: sum += record.rewardNetto; break;
            }
        }

        return QString::number(sum);
    };

    page.drawCell("Razem", 120, 10);
    page.drawCell(getSum(0), 120, 10);
    page.drawCell(getSum(1), 120, 10);
    page.drawCell(getSum(2), 120, 10);
    page.drawCell(getSum(3), 120, 10);

    progress->setValue(75);

    page.addSpace(10);
    page.drawVerse("Niniejsze zaświadczenie wydaję się na prośbę osoby zainteresowanej.");

    page.addSpace(100);
    page.drawOneLineVerses("(data i podpis  osoby upoważnionej)", "");

    progress->setValue(100);
    page.pageEnd();
}


void PrintDocuments::printHouseCertificate(QVector<Data::BillRecord> &records){
    PageDrawer page;
    page.setStartPoint(0, 100);
    page.setVerseOffset(20);
    page.setSaveFileName("Zaświadczenie o dodatku mieszkaniowym - " + employeeVariables.at(1) + " " + employeeVariables.at(3) + ".pdf");
    page.pageStart();
    page.drawBegin(companyVariables.at(3) + ", " + QDate::currentDate().toString("dd.MM.yyyy"));

    page.addSpace(40);
    page.drawHeader("Zaświadczenie", PageDrawer::Align::CENTER);

    page.addSpace(20);
    page.drawDataNewVerseRect("Zaświadcza się że Pan/i:", employeeVariables.at(1) + " " + employeeVariables.at(3), 200);
    page.drawDataNewVerseRect("Zamieszkały/a:", employeeVariables.at(7) + " " + employeeVariables.at(6) + " " + employeeVariables.at(8) + " " + employeeVariables.at(9) + "/" + employeeVariables.at(10), 400);

    QDate startDate;
    QDate endDate;
    if(records.at(0).payType == "uop"){
        startDate = QDate::fromString(jobContractVariables.at(5), "yyyy-MM-dd");
        endDate = QDate::fromString(jobContractVariables.at(6), "yyyy-MM-dd");

        page.drawDataNewVerseRect("Jest zatrudniony/a w wymiarze czasu pracy:", jobContractVariables.at(3), 200);
        page.drawDataSameVerseRect("Stosunek pracy zawarto dnia:", jobContractVariables.at(5), 200);
        page.drawDataNewVerseRect("na czas:", QString::number(endDate.month() - startDate.month()) + " miesięcy", 200);
    }
    else if(records.at(0).payType == "uz"){
        startDate = QDate::fromString(orderContractVariables.at(2), "yyyy-MM-dd");
        endDate = QDate::fromString(orderContractVariables.at(3), "yyyy-MM-dd");

        page.drawDataNewVerseRect("Jest zatrudniony/a w wymiarze czasu pracy:", orderContractVariables.at(5), 200);
        page.drawDataSameVerseRect("Stosunek pracy zawarto dnia:", orderContractVariables.at(2), 200);
        page.drawDataNewVerseRect("na czas:", QString::number(endDate.month() - startDate.month()) + " miesięcy", 200);
    }
    else if(records.at(0).payType == "ud"){
        startDate = QDate::fromString(workContractVariables.at(4), "yyyy-MM-dd");
        endDate = QDate::fromString(workContractVariables.at(5), "yyyy-MM-dd");

        page.drawDataNewVerseRect("Jest zatrudniony/a w wymiarze czasu pracy:", workContractVariables.at(6), 200);
        page.drawDataSameVerseRect("Stosunek pracy zawarto dnia:", workContractVariables.at(4), 200);
        page.drawDataNewVerseRect("na czas: ", QString::number(endDate.month() - startDate.month()) + " miesięcy", 200);
    }
    page.addSpace(20);
    page.drawBoldVerse("Dochód wypłącony w ostatnich miesiącach wynosi:");

    page.setColumnsCount(5);
    page.setVerseOffset(35);
    page.setFontSize(8);
    page.drawCell("Miesiąc/Rok", 120, 20, true);
    page.drawCell("Kwota Brutto", 120, 20, true);
    page.drawCell("Składki na ubez.\nspołeczne", 120, 20, true);
    page.drawCell("Koszt uzyskania\nprzychodu", 120, 20, true);
    page.drawCell("Dochód do opodatkowania", 130, 20, true);

    page.setVerseOffset(25);
    for(int i = 0; i < records.size(); ++i){
        page.drawCell(records.at(i).docId, 120, 10);
        page.drawCell(QString::number(records.at(i).rewardBrutto), 120, 10);
        page.drawCell(QString::number(records.at(i).iSum), 120, 10);
        page.drawCell(QString::number(records.at(i).rate), 120, 10);
        page.drawCell(QString::number(records.at(i).rewardNetto), 130, 10);
    }

    const auto getSum = [&](int index){
        float sum{0.0f};
        for(auto& record : records){
            switch(index){
            case 0: sum += record.rewardBrutto; break;
            case 1: sum += record.iSum; break;
            case 2: sum += record.rate; break;
            case 3: sum += record.rewardNetto; break;
            }
        }

        return QString::number(sum);
    };

    page.drawCell("Razem", 120, 10);
    page.drawCell(getSum(0), 120, 10);
    page.drawCell(getSum(1), 120, 10);
    page.drawCell(getSum(2), 120, 10);
    page.drawCell(getSum(3), 130, 10);

    page.setVerseOffset(20);
    page.setFontSize(12);
    page.addSpace(10);
    page.drawBoldVerse("Zaświadczenie wydaje się w celu: Naliczenia dodatku mieszkaniowego");

    page.addSpace(40);
    page.drawOneLineVerses("", "podpis i pieczęć osoby upoważnionej");

    page.addSpace(20);
    page.setFontSize(8);
    page.drawVerse("Pouczenie:");
    page.drawVerse("Za dochód zgodnie z art. 3 ust. 3 o dodatkach mieszkaniowych z dnia 21 czerwca 2001r..(Dz. U. Nr. 71 z 2001r. poz. 734) uważa się wszelkie");
    page.drawVerse("przychody po odliczeniu kosztów ich uzyskania oraz po odliczeniu składek na ubezpieczenie emerytalne i rentowne oraz na ubezpieczenie");
    page.drawVerse("chorobowe, określonych w przepisach o systemie ubezpieczeń społecznych, chyba że zostały już zaliczone do kosztów uzyskania przychodu.");

    page.pageEnd();
    progress->setValue(100);
}


void PrintDocuments::printFamilyOrMaintanceCertificate(const Data::BillRecord &record, int index){
    PageDrawer page;
    page.setStartPoint(0, 100);
    page.setVerseOffset(20);
    if(index == 0)
        page.setSaveFileName("Zaświadczenie o zarobkach(Rodzinne) - " + employeeVariables.at(1) + " " + employeeVariables.at(3) + ".pdf");
    else if(index == 1)
        page.setSaveFileName("Zaświadczenie o zarobkach(Alimentacyjne) - " + employeeVariables.at(1) + " " + employeeVariables.at(3) + ".pdf");

    page.pageStart();
    page.drawBegin(companyVariables.at(3) + ", " + QDate::currentDate().toString("dd.MM.yyyy"));

    page.addSpace(40);
    page.drawHeader("Zaświadczenie o zarobkach", PageDrawer::Align::CENTER);

    page.setFontSize(8);
    if(index == 0)
        page.drawVerse("wydane dla potrzeb świdczeń rodzinnych i świadczenia wychowawczego(500+) dla osób zgłaszających dochód uzsykany", PageDrawer::Align::CENTER);
    else if(index == 1)
        page.drawVerse("wydane dla potrzeb świdczeń z funduszu alimentacyjnego dla osób zgłaszających dochód uzyskany", PageDrawer::Align::CENTER);

    page.setFontSize(12);
    page.addSpace(30);
    page.drawDataNewVerseRect("Zaświadcza się:", employeeVariables.at(1) + " " + employeeVariables.at(3), 200);
    page.drawDataNewVerseRect("Nr. PESEL:", employeeVariables.at(11), 200);
    page.drawDataNewVerseRect("Zamieszkały/a:", employeeVariables.at(7) + " " + employeeVariables.at(6) + " " + employeeVariables.at(8) + " " + employeeVariables.at(9) + "/" + employeeVariables.at(10), 400);
    page.drawDataNewVerseRect("Jest zatrudniony/a:", companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " " + companyVariables.at(5), 400);

    page.addSpace(20);
    QString text = "  Na podstawie ";
    if(record.payType == "uop"){
        text += "umowy o pracę w okresie od " + jobContractVariables.at(5) + " do " + jobContractVariables.at(6) + " w wymiarze " + jobContractVariables.at(3) + ".";
    }
    else if(record.payType == "uz"){
        text += "umowy zlecenie w okresie od " + orderContractVariables.at(2) + " do " + orderContractVariables.at(3) + " w wymiarze " + orderContractVariables.at(5) + ".";
    }
    else if(record.payType == "ud"){
        text += "umowy o dzieło w okresie od " + workContractVariables.at(4) + " do " + workContractVariables.at(5) + " w wymiarze " + workContractVariables.at(6) + ".";
    }

    page.drawVerse(text);

    page.addSpace(20);
    page.drawDataNewVerseRect("Wynagrodzenie za:", record.docId, 200);
    page.drawDataNewVerseRect("Data wypłaty wynagrodzenia:", record.date.toString("dd.MM.yyyy"), 200);

    page.addSpace(20);
    page.drawDataNewVerseRect("Przychód:", QString::number(record.rewardBrutto), 200);
    page.drawDataNewVerseRect("Koszt uzyskania przychodu:", QString::number(record.incomeCost), 200);
    page.drawDataNewVerseRect("Należny podatek dochodowy od osób fizycznych:", QString::number(record.rate), 200);
    page.drawDataNewVerseRect("Składki na ubezpieczenie społeczne:", QString::number(record.iSum), 200);
    page.drawDataNewVerseRect("Składka na ubezpieczenie zdrowotne(9%):", QString::number(record.insurance), 200);
    page.drawDataNewVerseRect("Dochód uzyskany:", QString::number(record.rewardBrutto - record.incomeCost - record.rate - record.iSum - record.insurance), 200);

    page.addSpace(130);
    page.drawOneLineVerses("", "(pieczęć zakładu pracy)");

    page.pageEnd();
}


void PrintDocuments::printEmployeeInfo(QVector<Data::EmployeeInfoRecord> &jobRecords, QVector<Data::EmployeeInfoRecord> &freeRecords, QVector<Data::EmployeeInfoRecord> &sickRecords, QString& freeDays){
    PageDrawer page;
    page.setStartPoint(0, 100);
    page.setVerseOffset(20);
    page.setSaveFileName("Historia pracownika - " + employeeVariables.at(1) + " " + employeeVariables.at(3) + ".pdf");
    page.pageStart();

    page.drawHeader("Historia Pracownika", PageDrawer::Align::CENTER);
    page.drawVerse(employeeVariables.at(1) + " " + employeeVariables.at(3), PageDrawer::Align::CENTER);

    page.addSpace(20);
    page.drawBoldVerse("Informajce osobiste:");
    drawEmployeeData(page, employeeVariables);
    page.drawDataNewVerseRect("Dni urlopowe(Pozostało): ", freeDays, 100, true);

    page.addSpace(20);
    page.setVerseOffset(25);
    page.setColumnsCount(4);
    page.setFontSize(8);
    page.drawBoldVerse("Przebieg zatrudnienia:");
    page.drawCell("Rodzaj umowy", 100, 10, true);
    page.drawCell("Data zatrudnienia", 100, 10, true);
    page.drawCell("Data zwolnienia", 100, 10, true);
    page.drawCell("Stanowisko", 100, 10, true);

    if(!jobRecords.empty()){
        for(int i = 0; i < jobRecords.size(); ++i){
            page.drawCell(jobRecords.at(i).jobType, 100, 10);
            page.drawCell(jobRecords.at(i).startDate, 100, 10);
            page.drawCell(jobRecords.at(i).endDate, 100, 10);
            page.drawCell(jobRecords.at(i).type, 100, 10);
        }
    }

    page.addSpace(20);
    page.setVerseOffset(20);
    page.setFontSize(12);
    page.drawBoldVerse("Ewidencja urlopowa:");

    page.setVerseOffset(21);
    page.setFontSize(8);
    page.resetCells();
    page.setColumnsCount(4);
    page.drawCell("Data wzięcia", 100, 10, true);
    page.drawCell("Data zakończenia", 100, 10, true);
    page.drawCell("Rodzaj urlopu", 100, 10, true);
    page.drawCell("Ilość dni", 100, 10, true);

    int sum{0};
    if(!freeRecords.empty()){
        for(int i = 0; i < freeRecords.size(); ++i){
            page.drawCell(freeRecords.at(i).startDate, 100, 10);
            page.drawCell(freeRecords.at(i).endDate, 100, 10);
            page.drawCell(freeRecords.at(i).type, 100, 10);
            page.drawCell(freeRecords.at(i).daysCount, 100, 10);
            sum += freeRecords.at(i).daysCount.toInt();
        }
    }

    page.setFontSize(12);
    page.setVerseOffset(20);
    page.addSpace(10);
    page.drawVerse("Razem: " + QString::number(sum));
    sum = 0;

    page.addSpace(20);
    page.drawBoldVerse("Ewidencja niezdolności do pracy:");

    page.setVerseOffset(21);
    page.setFontSize(8);
    page.resetCells();
    page.setColumnsCount(3);
    page.drawCell("Data wzięcia", 100, 10, true);
    page.drawCell("Data zakończenia", 100, 10, true);
    page.drawCell("Ilość dni", 100, 10, true);

    if(!sickRecords.empty()){
        for(int i = 0; i < sickRecords.size(); ++i){
            page.drawCell(sickRecords.at(i).startDate, 100, 10);
            page.drawCell(sickRecords.at(i).endDate, 100, 10);
            page.drawCell(sickRecords.at(i).daysCount, 100, 10);
            sum += sickRecords.at(i).daysCount.toInt();
        }
    }

    page.setFontSize(12);
    page.setVerseOffset(20);
    page.addSpace(10);
    page.drawVerse("Razem: " + QString::number(sum));

    page.pageEnd();
    progress->setValue(100);
}


void PrintDocuments::printSellDoc(const FinanceDialog::State& state){
    QVector<QString> customer;
    QVector<QStringList> items;
    QSqlQuery query;
    query.exec("SELECT name, director, nip, regon, city, cityCode, address, email, phone, account FROM companies WHERE id = " + employeeID);
    if(query.next()){
        for(int i = 0; i < query.record().count(); ++i)
            customer.push_back(query.value(i).toString());
    }

    for(int i = 4; i < 14; ++i)
        items.push_back(financeVariables.at(i).split(","));

    PageDrawer page;
    QString changeNumber {financeVariables.at(0)};
    QString fileName {changeNumber.replace("/", "_")};
    QString fullName{""};
    QString header{""};

    if(state == FinanceDialog::State::FS){
        fullName = "Faktura sprzedaży - " + fileName + ".pdf";
        header = "Faktura VAT nr. " + financeVariables.at(0);
    }
    else if(state == FinanceDialog::State::WZ){
        fullName = "Wydanie Zewnętrzne - " + fileName + ".pdf";
        header = "Wydanie Zewnętrzne nr. " + financeVariables.at(0);
    }
    else if(state == FinanceDialog::State::PZ){
        fullName = "Przyjęcie Zewnętrzne - " + fileName + ".pdf";
        header = "Przyjęcie Zewnętrzne nr. " + financeVariables.at(0);
    }

    progress->setValue(25);

    page.setSaveFileName(fullName);
    page.setStartPoint(0, 50);
    page.setVerseOffset(20);
    page.pageStart();

    page.drawVerse("Wystawiono dnia:  " + financeVariables.at(1) + ", " + companyVariables.at(3));

    page.addSpace(60);
    page.drawHeader(header, PageDrawer::Align::CENTER);

    page.addSpace(20);
    page.drawVerse("Data sprzedaży:  " + financeVariables.at(1));
    page.drawVerse("Sposób zapłaty:  " + financeVariables.at(3));
    page.drawVerse("Termin płatności:  " + financeVariables.at(2));

    page.addSpace(20);
    page.drawBoldVerse("Sprzedawca:                                                                    Nabywca:");
    if(state != FinanceDialog::State::PZ){
        page.drawDataSameVerseRect("Firma:", companyVariables.at(0), 300);
        page.drawDataNewVerseRect("Firma:", customer.at(0), 300);
        page.drawDataSameVerseRect("Ulica: ", companyVariables.at(5), 300);
        page.drawDataNewVerseRect("Ulica: ", customer.at(6), 300);
        page.drawDataSameVerseRect("Adres:", companyVariables.at(4) + " " + companyVariables.at(3), 300);
        page.drawDataNewVerseRect("Adres:", customer.at(5) + " " + customer.at(4), 300);
        page.drawDataSameVerseRect("NIP:   ", companyVariables.at(2), 300);
        page.drawDataNewVerseRect("NIP:   ", customer.at(2), 300);
    }
    else{
        page.drawDataSameVerseRect("Firma:", customer.at(0), 300);
        page.drawDataNewVerseRect("Firma:", companyVariables.at(0), 300);
        page.drawDataSameVerseRect("Ulica: ", customer.at(6), 300);
        page.drawDataNewVerseRect("Ulica: ", companyVariables.at(5), 300);
        page.drawDataSameVerseRect("Adres:", customer.at(5) + " " + customer.at(4), 300);
        page.drawDataNewVerseRect("Adres:", companyVariables.at(4) + " " + companyVariables.at(3), 300);
        page.drawDataSameVerseRect("NIP:   ", customer.at(2), 300);
        page.drawDataNewVerseRect("NIP:   ", companyVariables.at(2), 300);
    }

    progress->setValue(50);

    const int cellWidth {165};
    const int cellHeight {20};
    page.addSpace(40);
    page.drawBoldVerse("Pozycje:");
    page.setColumnsCount(10);
    page.setFontSize(8);
    page.setVerseOffset(35);
    page.drawCell("Nazwa towaru/usługi", cellWidth, cellHeight, true);
    page.drawCell("J.M", 40, cellHeight, true);
    page.drawCell("Ilość", 50, cellHeight, true);
    page.drawCell("Rabat(%)", 60, cellHeight, true);
    page.drawCell("Cena netto(zł)", 80, cellHeight, true);
    page.drawCell("Cena netto\npo rabacie(zł)", 80, cellHeight, true);
    page.drawCell("Wartość netto\n(zł)", 80, cellHeight, true);
    page.drawCell("VAT(%)", 50, cellHeight, true);
    page.drawCell("Wartość VAT\n(zł)", 80, cellHeight, true);
    page.drawCell("Wartość brutto\n(zł)", 80, cellHeight, true);

    for(int i = 0; i < items.at(0).size(); ++i){
        if(i != 0 && i % 12 == 0){
            page.toNextPage();
            page.setStartPoint(0, 100);
            page.drawCell("Nazwa towaru/usługi", cellWidth, cellHeight, true);
            page.drawCell("J.M", 40, cellHeight, true);
            page.drawCell("Ilość", 50, cellHeight, true);
            page.drawCell("Rabat(%)", 60, cellHeight, true);
            page.drawCell("Cena netto(zł)", 80, cellHeight, true);
            page.drawCell("Cena netto\npo rabacie(zł)", 80, cellHeight, true);
            page.drawCell("Wartość netto\n(zł)", 80, cellHeight, true);
            page.drawCell("VAT(%)", 50, cellHeight, true);
            page.drawCell("Wartość VAT\n(zł)", 80, cellHeight, true);
            page.drawCell("Wartość brutto\n(zł)", 80, cellHeight, true);
        }

        page.drawCell(items.at(0).at(i), 165, cellHeight);
        page.drawCell(items.at(1).at(i), 40, cellHeight);
        page.drawCell(items.at(2).at(i), 50, cellHeight);
        page.drawCell(items.at(3).at(i), 60, cellHeight);
        page.drawCell(items.at(4).at(i), 80, cellHeight);
        page.drawCell(items.at(5).at(i), 80, cellHeight);
        page.drawCell(items.at(6).at(i), 80, cellHeight);
        page.drawCell(items.at(7).at(i), 50, cellHeight);
        page.drawCell(items.at(8).at(i), 80, cellHeight);
        page.drawCell(items.at(9).at(i), 80, cellHeight);
    }

    progress->setValue(75);

    auto rtd = [](float value){ float var = (int)(value * 100 + 0.5f); return (float)var / 100; };

    page.setVerseOffset(20);
    page.setFontSize(12);
    page.addSpace(40);
    page.drawBoldVerse("Podsumowanie:");
    page.drawDataNewVerseRect("Razem netto(zł):", QString::number(rtd(financeVariables.at(15).toFloat())), 200);
    page.drawDataNewVerseRect("Razem VAT(zł):", QString::number(rtd(financeVariables.at(16).toFloat())), 200);
    page.drawDataNewVerseRect("Razem brutto(zł):", QString::number(rtd(financeVariables.at(17).toFloat())), 200);

    page.addSpace(60);
    page.setFontSize(8);

    if(state == FinanceDialog::State::FS)
        page.drawOneLineVerses("Osoba upoważniona do otrzymania faktury VAT", "Osoba upoważniona do wystawienia faktury VAT");
    else
        page.drawOneLineVerses("          Odebrał          ", "          Wystawił          ");

    page.pageEnd();
    progress->setValue(100);
}


void PrintDocuments::printCompensationDoc(){
    QString name, address, city;
    QSqlQuery query;
    query.exec("SELECT name, city, cityCode, address FROM companies WHERE id = " + docId);
    if(query.next()){
        name = query.value(0).toString();
        address = query.value(3).toString();
        city = query.value(1).toString() + " " + query.value(2).toString();;
    }

    PageDrawer page;
    page.setSaveFileName("Oświadczenie Kompensaty - " + name + ".pdf");
    page.setStartPoint(0, 100);
    page.setVerseOffset(20);
    page.pageStart();

    page.drawBegin(companyVariables.at(3) + "," + QDate::currentDate().toString("dd.MM.yyyy"));

    page.addSpace(20);
    page.drawBoldVerse("Dane kontrahenta:");
    page.drawVerse(name);
    page.drawVerse(address);
    page.drawVerse(city);

    page.addSpace(40);
    page.drawHeader("OŚWIADCZENIE O KOMPENSACIE", PageDrawer::Align::CENTER);

    page.addSpace(20);
    page.drawVerse("Uprzejmie informujemy,że na podstawie art. 498 Kodeksu Cywilnego dokonaliśmy kompensaty:");

    page.setColumnsCount(2);
    page.drawCell("naszych zobowiązań, wynikających z:", 380);
    page.drawCell("z naszymi należnościami, wynikających z:", 380);
    page.resetCells();

    page.setVerseOffset(49);
    page.setColumnsCount(6);
    page.drawCell("Dokument\n(nazwa i numer)", 180, 30);
    page.drawCell("Data\nDokumentu", 100, 30);
    page.drawCell("Kwota(zł)", 100, 30);
    page.drawCell("Dokument\n(nazwa i numer)", 180, 30);
    page.drawCell("Data\nDokumentu", 100, 30);
    page.drawCell("Kwota(zł)", 100, 30);

    progress->setValue(25);

    QStringList sellId { compensationVariables.at(1).split(",") };
    QStringList sellDate { compensationVariables.at(2).split(",") };
    QStringList sellBrutto { compensationVariables.at(3).split(",") };
    QStringList buyId { compensationVariables.at(4).split(",") };
    QStringList buyDate { compensationVariables.at(5).split(",") };
    QStringList buyBrutto { compensationVariables.at(6).split(",") };
    const int size = sellId.size() >= buyId.size() ? sellId.size() : buyId.size();
    float sellSum{}, buySum{}, balance{compensationVariables.at(0).toFloat()};
    page.setVerseOffset(31);

    for(int i = 1; i < size; ++i){
        i < buyId.size() + 1 ? page.drawCell(buyId.at(i - 1), 180, 10) : page.drawCell(" ", 180, 10);
        i < buyDate.size() + 1 ? page.drawCell(buyDate.at(i - 1), 100, 10) : page.drawCell(" ", 100, 10);
        if(i < buyBrutto.size() + 1){
            page.drawCell(buyBrutto.at(i - 1), 100, 10);
            buySum += buyBrutto.at(i - 1).toFloat();
        }
        else
            page.drawCell(" ", 100, 10);

        i < sellId.size() + 1 ? page.drawCell(sellId.at(i - 1), 180, 10) : page.drawCell(" ", 180, 10);
        i < sellDate.size() + 1 ? page.drawCell(sellDate.at(i - 1), 100, 10) : page.drawCell(" ", 100, 10);
        if(i < sellBrutto.size() + 1){
            page.drawCell(sellBrutto.at(i - 1), 100, 10);
            sellSum += sellBrutto.at(i - 1).toFloat();
        }
        else
            page.drawCell(" ", 100, 10);
    }

    progress->setValue(75);

    page.resetCells();
    page.setColumnsCount(4);
    page.drawCell("na łączną kwotę", 280, 10);
    page.drawCell(QString::number(buySum), 100, 10);
    page.drawCell("na łączną kwotę", 280, 10);
    page.drawCell(QString::number(sellSum), 100, 10);
    page.setVerseOffset(20);

    page.addSpace(20);
    page.drawVerse("Po dokonaniu kompensaty do zapłaty na nasze/Państwa dobro pozostaje kwota: " + QString::number(std::abs(balance)) + " zł");
    page.drawVerse("(słownie:                                                                                                                       )");

    page.addSpace(40);
    page.drawVerse("Nierozliczoną w drodze kompensaty należność prosimy przelać na konto bankowe o numerze");
    page.drawVerse("                                                                                           do dnia                                 r.");

    page.addSpace(40);
    page.drawVerse("Nierozliczone w drodze kompensaty zobowiązanie zostało/zostanie uregulowane w/do dniu/dnia                          r.");

    page.addSpace(40);
    page.drawVerse("Prosimy o zgodne z nami księgowanie.");

    page.addSpace(60);
    page.setFontSize(8);
    page.drawOneLineVerses("", "podpis osoby upoważnionej do składania oświadczeń");

    page.pageEnd();
    progress->setValue(100);
}



//Draw Pages Functions--------------------------------------------------
void PrintDocuments::drawEmployeeData(PageDrawer& page, QVector<QString>& employee, bool currentData){
    //if(!currentData) page.drawDataNewVerseRect(DBManager::getEmployeeVariable(0), employee.at(0), 80, true);      //ID
    page.drawDataSameVerseRect(DBManager::getEmployeeVariable(1), employee.at(1), 200, true);    //Name
    page.drawDataNewVerseRect(DBManager::getEmployeeVariable(2), employee.at(2), 249);           //Second Name
    page.drawDataNewVerseRect(DBManager::getEmployeeVariable(3), employee.at(3), 546, true);     //Surname
    if(!currentData) page.drawDataSameVerseRect(DBManager::getEmployeeVariable(4), employee.at(4), 200, true);    //Birth Date
    if(!currentData) page.drawDataNewVerseRect(DBManager::getEmployeeVariable(5), employee.at(5), 204);           //Birth Place
    page.drawDataSameVerseRect(DBManager::getEmployeeVariable(6), employee.at(6), 80, true);     //City Code
    page.drawDataNewVerseRect(DBManager::getEmployeeVariable(7), employee.at(7), 300);           //City
    page.drawDataNewVerseRect(DBManager::getEmployeeVariable(8), employee.at(8), 546, true);     //Address
    page.drawDataSameVerseRect(DBManager::getEmployeeVariable(9), employee.at(9), 80, true);     //House Number
    page.drawDataNewVerseRect(DBManager::getEmployeeVariable(10), employee.at(10), 80);          //Local Number

    if(!currentData) page.drawDataSameVerseRect(DBManager::getEmployeeVariable(11), employee.at(11), 200, true);  //Pesel
    else page.drawDataNewVerseRect(DBManager::getEmployeeVariable(11), employee.at(11), 200, true);

    if(!currentData) page.drawDataNewVerseRect(DBManager::getEmployeeVariable(12), employee.at(12), 200);         //Parents Names
    if(!currentData) page.drawDataNewVerseRect(DBManager::getEmployeeVariable(13), employee.at(13), 200, true);   //Parent Surname
    if(!currentData) page.drawDataNewVerseRect(DBManager::getEmployeeVariable(14), employee.at(14), 200, true);   //Evidence Number
}


void PrintDocuments::drawStartProvisionsPage(PageDrawer &page, const QString &currentDate){
    page.toNextPage();
    page.setStartPoint(0, 100);
    page.drawBegin(currentDate);

    page.addSpace(40);
    page.drawHeader("Postanowienia Ogólne", PageDrawer::Align::CENTER);

    page.addSpace(20);
    page.drawVerse("1. W przypadku nienależytego wykonania pracy, pracodawca ma prawo odmowy wypłaty całości lub części");
    page.drawVerse("    wynagrodzenia. Jeśli pracownik wypowie umowę i nie zawiadomi o tym fakcie pracodawcy na piśmie");
    page.drawVerse("    najpóźnirj w następnym dniu roboczym, od daty zaprzestania wykonywania pracy to pracodawca jest");
    page.drawVerse("    uprawniony do zarządzania od pracownika zapłaty kary umownej wysokości stanowiącej 20% wynagrodzenia");
    page.drawVerse("    zasadniczego należnego za miesiąc, w którym zaprzestał on wykonywania swych czynności, postanowienie");
    page.drawVerse("    to nie wyłącza prawa pracodawcy do dochodzenia dalej idących roszczeń odszkodowawczych z tego tytułu.");

    page.addSpace(10);
    page.drawVerse("2. Pracownik jest zobowiązany do stosowania się przyjętych zasad i norm oraz procedur obowiązujących");
    page.drawVerse("    w firmie, dotyczących wykonywania pracy według potrzeb pracodawcy, naruszenie powyższych obowiązków");
    page.drawVerse("    przez pracownika, będzie skutkowało nałożenia kary umownej wysokości wskazanej według pracodawcy,");
    page.drawVerse("    nie większej niż 100zł za każdy przypadek.");

    page.addSpace(10);
    page.drawVerse("3. Pracownik odpowiada w pełnej wysokości na zasadach określonych przepisami kodeksu cywilnego za wszelkie");
    page.drawVerse("    szkody wyrządzone przez niego na rzecz pracodawcy lub jego klientom, kontrahentom związku z wykonywaniem");
    page.drawVerse("    albo nienależytym wykonaniem swych zadań.");

    page.addSpace(10);
    page.drawVerse("4. Pracownik może powierzyć wykonanie zleconych mu czynności innej osobie jedynie za uprzednią pisemną zgodą");
    page.drawVerse("    Pracodawcy, Pracownik ten powinien wykonywać powierzone mu czynności zachowanej należytej staranności");
    page.drawVerse("    wymaganej uwagi na szczególny charakter. Pracownik ten zobowiązuję się przy tym do takiego angażowania");
    page.drawVerse("    wykonania danej pracy jakie będzie wymagane z uwagi na stopień trudności wykonywanego zadania.");

    page.addSpace(10);
    page.drawVerse("5. Pracodawca nie odpowiada za szkody wyrządzone przez pracownika lub przez osoby trzecie wykonujące pracę");
    page.drawVerse("    w zastępstwie na rzecz pracodawcy.");

    page.addSpace(10);
    page.drawVerse("6. Pracownik oświadcza, że zna przepisy oraz zasady bezpieczeństwa i higieny pracy obowiązujących przy");
    page.drawVerse("    wykonywanych czynnościach wynikających z umowy i oświadcza, że będzie ich przestrzegał, a także oświadcza,");
    page.drawVerse("    że jego stan zdrowia pozwala na wykonywanie danej pracy, czym potwierdza te treści swym podpisem.");

    page.addSpace(10);
    page.drawVerse("7. W sprawach nieuregulowanych w tej umowie zastosowanie mają przepisy Kodeksu Cywilnego oraz przepisy prawa");
    page.drawVerse("    Ubezpieczeń Społecznych i Zdrowotnych.");

    page.addSpace(10);
    page.drawVerse("8. Strony zgodnie ustalają, że w przypadku powstania sporu, którego podstawą roszczenia wynikające postanowień");
    page.drawVerse("    umowy z sądem właściwym do rozpoznania sprawy będzie sąd miejscowy właściwy dla zakładu pracy.");

    page.addSpace(10);
    page.drawVerse("9. Umowę sporządzono w dwóch jednobrzmiących egzemplarzach po jednym dla każdej ze stron.");

    page.drawFooter(currentDate);

    progressValue += 15;
    progress->setValue(progressValue);
}


void PrintDocuments::drawEndProvisionsPage(PageDrawer &page, const QString &currentDate){
    page.toNextPage();
    page.setStartPoint(0, 100);
    page.drawBegin(currentDate);

    page.addSpace(40);
    page.drawHeader("Postanowienia szczegółowe", PageDrawer::Align::CENTER);

    page.addSpace(20);
    page.drawVerse("1. Pracownik jest zobowiązany do zachowania tajemnicy odnośnie wszystkich dokumentów przedsiębiorstwa oraz");
    page.drawVerse("    innych informacji pozyskiwanych w związku z wykonywaniem na podstawie umowy jak również zachowanie");
    page.drawVerse("    najwyższej staranności w celu przed ujawnieniem ich osobom Nieupoważnionym obowiązek powyższe obejmuje");
    page.drawVerse("    w szczególności poufne informacje dotyczące klientów sposobów negocjacji prowadzenia rozmów z klientami");
    page.drawVerse("    warunku i treści zawieranych w nim umów poufne informacje odnośnie struktury organizacyjnej i zasady");
    page.drawVerse("    działania przedsiębiorstwa zleceniodawcy oraz inne informacje, w których ujawnienie mogłoby zagrozić");
    page.drawVerse("    lub naruszać interes lub narazić pracodawcę na szkodę, naruszenie powyższych obowiązków w szczególności");
    page.drawVerse("    poprzez przekazywanie lub ujawnienie informacji osobom Nieupoważnionym, jak i również innym nieuprawnione");
    page.drawVerse("    ich wykorzystywanie nakładana jest kara umowna wysokości 100zł za każdy przypadek narusza co nie wyłącza");
    page.drawVerse("    możliwości dochodzenia odszkodowania na zasadach ogólnych. Obowiązki powyższą bo mają okres trwania umowy");
    page.drawVerse("    przez 3 lata od jej ustalenia.");

    page.addSpace(10);
    page.drawVerse("2. Pracownik przyjmuję do wiadomości, że wykonane zadania będą monitorowane przez pracodawće oraz, że wszelkie");
    page.drawVerse("    ich kopiowanie rozpowszechnianie poza struktury firmy bez zgody zarządu jest zabronione. Ponadto pracownik");
    page.drawVerse("    zobowiązuję się zachować tajemnicę zawodową, która zostanie opisana w osobnym dokumencie stanowiącym");
    page.drawVerse("    integralną część powyższej umowy.");

    page.addSpace(10);
    page.drawVerse("3. Pracownik wyraża zgodę na pobieranie oraz przetwarzanie danych osobowych przez zleceniodawcę w zakresie");
    page.drawVerse("    obejmującym: imienia, nazwiska innych danych w celu sporządzenia umowy oraz stworzenie kartoteki osobowej");
    page.drawVerse("    i archiwizację tych danych na rzecz w/w pracodawcy.");

    page.addSpace(100);
    page.drawOneLineVerses("podpis pracodawcy", "podpis pracownika");

    page.addSpace(50);
    page.drawHeader("Postanowienia końcowe", PageDrawer::Align::CENTER);

    page.addSpace(40);
    page.drawVerse("1. Umowa ulega rozwiązaniu w przypadku zaprzestania stosunku pracy.");
    page.addSpace(10);
    page.drawVerse("2. Wypłata ostatniego wynagrodzenia po zakończeniu umowy, wskutek jej wygaśnięcia, następuję bezgotówkowo");
    page.drawVerse("    przelewem na wskazane przez zleceniobiorce konto bankowe lub przekazem pocztowym na adres pracownika,");
    page.drawVerse("    w terminie do 10 dni od sporządzenia \"Paska wynagrodzenia osobowego\". Wypłata też może nastąpić w");
    page.drawVerse("    \"Kasie\" przedsiębiorstwa na co pracownik udziela zgodę podpisując daną umowę. Pokwitowaniem takiego");
    page.drawVerse("    rodzaju wypłacenia wynagrodzenia będzie druk \"KW\" dla każdej ze stron.");

    page.addSpace(100);
    page.drawOneLineVerses("podpis pracodawcy", "podpis pracownika");

    progressValue += 15;
    progress->setValue(progressValue);
}


void PrintDocuments::drawBHPPage(PageDrawer& page, const QString& currentDate){
    page.toNextPage();
    page.setStartPoint(0, 100);
    page.drawBegin(currentDate);

    page.addSpace(40);
    page.drawHeader("Instruktaż ogólny - cel i program", PageDrawer::Align::CENTER);
    page.drawVerse("Szkolenie z Bezpieczeństwa i Higieny Pracy", PageDrawer::Align::CENTER);
    page.addSpace(20);
    page.drawVerse("Celem szkolenia jest zapoznanie pracownika w szczególności z:");
    page.addSpace(10);
    page.drawVerse("1. Podstawowymi przepisami bezpieczeństwa i higieny pracy zawartymi w Kodeksie pracy,");
    page.drawVerse(" w układach zbiorowych Pracy lub w regulaminach pracy, przepisami oraz z zasadami");
    page.drawVerse("bezpieczeństwa i higieny pracy obowiązującymi w zakładzie pracy, ");
    page.drawVerse("zasadami udzielania pierwszej pomocy w razie wypadku.");

    page.setColumnsCount(3);
    page.setFontSize(8);
    page.setVerseOffset(25);
    page.drawCell("Lp.", 100, 10, true);
    page.drawCell("Temat Szkolenia", 500, 10, true);
    page.drawCell("Liczba godzin", 100, 10, true);

    page.drawCell("1.", 100, 10);
    page.drawCell("2", 500, 10);
    page.drawCell("3", 100, 10);

    page.drawCell("2.", 100, 10);
    page.drawCell("Istota bezpieczeństwa i higieny pracy.", 500, 10);
    page.drawCell("0,6", 100, 65);

    page.resetCells();
    page.setColumnsCount(2);
    page.setVerseOffset(55);
    page.drawCell("3.", 100, 40);
    page.drawCell("Zakres obowiązków i uprawnienia pracodawcy, pracowników oraz poszczególnych komórek\n"
                  "organizacyjnych zakładu. Odpowiedzialność za naruszenie przepisów lub zasad bezpieczeństwa i\n"
                  "higieny pracy.", 500, 40);

    page.resetCells();
    page.setColumnsCount(3);
    page.setVerseOffset(25);
    page.drawCell("4.", 100, 10);
    page.drawCell("Zasady poruszania się na terenie zakładu pracy." , 500, 10);
    page.drawCell("0,5", 100, 55);

    page.resetCells();
    page.setColumnsCount(2);
    page.setVerseOffset(45);
    page.drawCell("5.", 100, 30);
    page.drawCell("Zagrożenia wypadkowe i zagrożenia dla zdrowia występujące w zakładzie i podstawowe środki\n"
                  "zapobiegawcze.", 500, 30);

    page.resetCells();
    page.setColumnsCount(3);
    page.setVerseOffset(45);
    page.drawCell("6.", 100, 30);
    page.drawCell("Podstawowe zasady bezpieczeństwa i higieny pracy związane z obsługą urządzeń technicznych\n"
                  "oraz transportem wewnątrz zakładowym." , 500, 30);
    page.drawCell("0,4", 100, 30);

    page.drawCell("7.", 100, 30);
    page.drawCell("Zasady przydziału odzieży roboczej i obuwia roboczego (jeżeli są one wymagane na\n"
                  "stanowisku pracy), oraz środki ochrony indywidualnej w tym w odniesieniu do stanowiska pracy." , 500, 30);
    page.drawCell("0,5", 100, 80);

    page.resetCells();
    page.setColumnsCount(2);
    page.setVerseOffset(25);
    page.drawCell("8.", 100, 10);
    page.drawCell("Porządek i czystość w miejscu pracy - ich wpływ na zdrowie i bezpieczeństwo pracownika.", 500, 10);

    page.drawCell("9.", 100, 10);
    page.drawCell("Porządek i czystość w miejscu pracy - ich wpływ na zdrowie i bezpieczeństwo pracownika.", 500, 10);

    page.resetCells();
    page.setColumnsCount(3);
    page.setVerseOffset(25);
    page.drawCell("10.", 100, 10);
    page.drawCell("Podstawowe zasady ochrony przeciwpożarowej oraz postępowanie w razie pożaru.", 500, 10);
    page.drawCell("1", 100, 35);

    page.resetCells();
    page.setColumnsCount(2);
    page.setVerseOffset(25);
    page.drawCell("11.", 100, 10);
    page.drawCell("Postępowanie w razie wypadku, w tym organizacja i zasay udzielania pierwszej pomocy.", 500, 10);

    page.resetCells();
    page.setColumnsCount(3);
    page.setVerseOffset(45);
    page.drawCell(" ", 100, 10);
    page.drawCell("Razem:", 500, 10);
    page.drawCell("6", 100, 10);

    page.setFontSize(12);
    page.drawFooter(currentDate);

    progressValue += 15;
    progress->setValue(progressValue);
}


void PrintDocuments::drawBHPDeclarationPage(PageDrawer &page, QVector<QString> &employee, const QString &currentDate, const QString& date, const QString &company){
    page.toNextPage();
    page.setStartPoint(0 ,100);
    page.setVerseOffset(20);
    page.drawBegin(currentDate);
    page.addSpace(40);
    page.drawHeader("Oświadczenie pracownika", PageDrawer::Align::CENTER);
    page.drawVerse("o zapoznaniu się z dokonaną oceny ryzyka zawodowego", PageDrawer::Align::CENTER);
    page.addSpace(10);
    page.drawVerse("Ja niżej podpisany:");
    page.addSpace(10);

    drawEmployeeData(page, employee);

    page.addSpace(10);
    page.drawDataVerse("Zatrudniony/a w: ", company);
    page.addSpace(10);
    page.drawVerse("Potwierdzam, że zostałem/am w dniu " + date + " poinformowany/a przez pracodawcę o ryzyku");
    page.drawVerse("zawodowym, które wiąże się z wykonywaną prze ze mnie pracą na stanowisku: " + employee.at(15));
    page.addSpace(10);
    page.drawVerse("Ponadto pracodawca poinformował mnie o zasadach ochrony przeciwpożarowej");
    page.drawVerse("obowiązujących w zakładzie pracy.");
    page.addSpace(10);
    page.drawVerse("W szczególności:");
    page.drawVerse("1. Postanowienia BHP, zadania oraz procedury w zakresie ochrony");
    page.drawVerse("przeciwpożarowej odnoszące się do mojego stanowiska pracy.");
    page.drawVerse("2. Przyczyny zapobiegania powstaniu i rozprzestrzenianiu się pożarów.");
    page.drawVerse("3. Zasady zapobiegania się z podręcznym sprzętem gaśniczym, gaśnicami");
    page.drawVerse(" oraz innymi sprzętami gaśniczymi.");
    page.drawVerse("4. Zasady postępowania na wypadek pożaru.");
    page.addSpace(10);
    page.drawVerse("Zaświadczam, że w dniu " + date + " zostałem/am zapoznany/a z wymogami");
    page.drawVerse("Bezpieczeństwa i Higieny Pracy obowiązujących w zakładzie.");
    page.addSpace(10);
    page.drawVerse("Instruktarzu udzielił mi:");
    page.addSpace(20);
    page.drawInfo("(podpis osoby udzielającej instruktora)", PageDrawer::Align::CENTER);
    page.drawFooter(currentDate);

    progressValue += 15;
    progress->setValue(progressValue);
}


void PrintDocuments::drawHopePage(PageDrawer &page, const QString &currentDate){
    page.toNextPage();
    page.setStartPoint(0, 100);
    page.drawBegin(currentDate);
    page.addSpace(40);
    page.drawHeader("Klauzula poufności informacji", PageDrawer::Align::CENTER);

    page.addSpace(20);
    page.drawVerse("1. Strony zobowiązują się:");
    page.setFontSize(10);
    page.setVerseOffset(12);
    page.drawVerse("    1.1 Do zachowania w ścisłej tajemnicy oraz do nie przekazywania, nie ujawniania");
    page.drawVerse("        i nie wykorzystywania informacji stanowiązych tajemnicę przedsiębiorstwa drugiej");
    page.drawVerse("        Stronie, a także wszelkich poufnych informacji i faktów, o których dowiem się");
    page.drawVerse("        w trakci wzajemnej wpsółpracy lub przy okazji współpracy w związku z przygotowaniem");
    page.drawVerse("        oferty, niezależnie od formy przekazania/pozyskania tych informacji i ich źródła.");
    page.drawVerse("        Przepis ten dotyczy w szczególności nie przekazywania, nie ujawniania i niewykorzystywania");
    page.drawVerse("        infromacji oraz dokumentacji określonej w załączonej \"DEKLARACJI w zakresie poufności informacji\",");
    page.drawVerse("        stanowiącyh tajemnic przedsiębiorstwa w myśl art. 11 ust. 4 Ustawy o Zwalczaniu Nieuczciwej");
    page.drawVerse("        Konkurencji z dnia 16 kwietnia 1993r.");
    page.addSpace(10);
    page.drawVerse("    1.2 Wykorzystywania informacji, o których mowa w ust. 1.1 jedynie w celach określonych");
    page.drawVerse("        ustaleniami przez Strony w związki z przygotowaniem oferty.");
    page.addSpace(10);
    page.drawVerse("    1.3 Podjąć wszelkie niezbędne kroki dla zapewnienia, że żadna z osób otrzymujących informacje");
    page.drawVerse("        nie ujawni tych informacji, ani ich źródła zarówno w całości jak i części, stronom trzecim");
    page.drawVerse("        bez uzsykania przedniego wybranego upoważnienia na piśmie od Strony, której informacja lub");
    page.drawVerse("        źródło dotyczy. Strona, która przekazuje informacje drugiej Stronie, odpowiada za osoby, ");
    page.drawVerse("        który, te informacje zostają udostępnione/przekazane jak za własne działanie lub zaniechanie, ");
    page.drawVerse("        w szczególności ponosi odpowiedzialność za zachowanie przestrzegania postanowienia ust. 1.1 i 1.2");
    page.drawVerse("        Ujawnia informacje, o których mowa w ust 1.1 jedynie tym pracownikom, współpracownikom i ");
    page.drawVerse("        doradcom Stron, którym będą one niezbędne do wykonania powierzonych im czynnośći i tylko w ");
    page.drawVerse("        zakresie w jakim odbiorca informacji musi mieć dostęp dla celów określonych w ust. 1.2.");

    page.setFontSize(12);
    page.setVerseOffset(20);
    page.addSpace(15);
    page.drawVerse("2. Postanowienia ust. 1.1 - 1.4 nie będą miały zastosowania do tych informacji uzyskanych od drugiej Strony,");
    page.drawVerse("które:");
    page.setFontSize(10);
    page.setVerseOffset(12);
    page.drawVerse("    2.1 Są opublikowane, powszechnie znane lub urzędowo podane do publicznej wiadomości;");
    page.addSpace(10);
    page.drawVerse("    2.2 Są znane danej Stronie przed przystąpieniem do czynności związanych z przygotowaniem oferty lub");
    page.drawVerse("        zostały uzyskane od osoby trzeciej zgodnie z prawem, bez ograniczeń do ich ujawniania;");
    page.addSpace(10);
    page.drawVerse("    2.3 Zostaną ujawnione przez jedną ze Stron za uprzednim podpisem na to zgody drugiej Stronie;");
    page.addSpace(10);
    page.drawVerse("    2.4 Zostaną ujawnione przez jedną ze Stron ze względu na obowiązujące wymogi prawa lub zgodnie z");
    page.drawVerse("        prawomocnym orzeczeniem sądu lub prawomocną decyzją administracyjną z zastzeżeniem, że podjęte");
    page.drawVerse("        zostały rozsądne i zgodne z prawem kroki zmierzające do zachowania poufności takich informacji;");
    page.addSpace(10);
    page.drawVerse("    2.5 Zostały uzyskane przez daną Stronę niezależnie od czynności związanych z przygotowaniem oferty.");

    page.addSpace(15);
    page.setFontSize(12);
    page.setVerseOffset(20);
    page.drawVerse("    Czas trwania obowiązku zachowania w tajemnicy informacji, o których mowa w ust. 1.1 obowiązuję również");
    page.drawVerse("po wygaśnięciu terminu procedury przetargowej.");

    page.addSpace(15);
    page.drawVerse("4. W przypadku naruszenia obowiązku zachowania w tajemnicy informacji o których mowa w ust. 1.1 przez");
    page.drawVerse("jedną ze Stron, druga Strona będzie uprawniona do dochodzenia odszkodowania w pełnej wysokości i ");
    page.drawVerse("naprawienia szkody zgodnie z obowiązującymi przepisami Kodeksu cywilnego.");

    page.addSpace(15);
    page.drawVerse("5. Informacje, o których mowa w ust. 1 niniejszej Umowy mogą stanowić informacje poufne w rozumieniu");
    page.drawVerse("Ustawy o obrocie instrumentami finansowymi z dnia 9 lipca 2005r. (Dz. U. Nr. 183, poz. 1538). Ujawnienie");
    page.drawVerse("ww. informacji poufnych podlega odpowiedzialności karnej określonej w Ustawie.");

    page.drawFooter(currentDate);

    progressValue += 15;
    progress->setValue(progressValue);
}


void PrintDocuments::drawHopeDeclarationPage(PageDrawer &page, QVector<QString> &employee, const QString& currentDate, const QString& company){
    page.toNextPage();
    page.setStartPoint(0, 100);
    page.drawBegin(currentDate);

    page.addSpace(40);
    page.drawHeader("Deklaracja", PageDrawer::Align::CENTER);
    page.drawVerse("w zakresie poufności informacji stanowiących tajemnicę zakładu", PageDrawer::Align::CENTER);

    page.addSpace(20);
    page.drawBoldVerse("Ja niżej podpisany:");

    page.addSpace(10);
    drawEmployeeData(page, employee, true);

    page.addSpace(10);
    page.drawDataNewVerseRect("Jako pracownik firmy:", company, 546, true);

    page.addSpace(30);
    page.drawBoldVerse("1. Informacje techniczne i technologiczne:");
    page.setFontSize(10);
    page.drawVerse("a) \"Know-How\" związane z procesem produkcji, technologii, środkami produkcji, jakości produktów wytwarzanych w zakładzie pracy.");
    page.drawVerse("b) Wszelkie infromacje techniczne związane z wyrobem, jego konstrukcji i zastosowanymi materiałami.");
    page.drawVerse("c) Szczegółowe informacje techniczne dotyczące stanu posiadania: wyposażenia, narzędzi, środków produkcji i środków kontroli.");
    page.drawVerse("d) Infromacje dotyczące lokalizacji i rodzaju pomieszczeń, zabezpieczeń(w tym zabezpieczeń fizycznych i proceduralnych wejścia");
    page.drawVerse("na teren organizacji oraz dostępu do poszczególnych pomieszczeń i aktywów), jak i również wszelkich aktywów organizacji, które");
    page.drawVerse("mogłyby w jakikolwiek sposób zagrozić przedsiębiorstwu.");

    page.addSpace(30);
    page.drawBoldVerse("2. Informacje handlowe:");
    page.drawVerse("a) Stosowane ceny wyrobów, koszty wytworzenia wyrobów, ceny zakupów materiałów i akcesoriów stosowanych w zakładzie pracy.");
    page.drawVerse("b) Informacje dotyczące bazy dostawców i odbiorców materiałów, usług i innych oraz stosowanych cen.");
    page.drawVerse("c) Wszelkie informacje dotyczące zawartych umów z dostawcami i odbiorcami");
    page.drawVerse("d) Informacje dotyczące rozmów z kontrahentami, w szczególności z partnerami zagranicznymi.");

    page.addSpace(30);
    page.drawBoldVerse("3. Informacje ekonomiczno-finansowe lub inne posiadające wartość gospodarczą:");
    page.drawVerse("a) Informacje dotyczącej kondycji finansowej firmy oraz źródeł finansowania.");
    page.drawVerse("b) Informacje dotyczące organizacji spółki i jego stanu prawnego oraz umów z udziałowcami.");
    page.drawVerse("c) Informacje n/t stosowanych systemu zarządzania.");
    page.drawVerse("d) Informacje dotyczące wysokości wynagrodzeń, sposobów motywacji i wynagrodzenia pracowników.");
    page.drawVerse("e) Dane osobowe zatrudnionego personelu, ich kwalifikacji oraz wykształcenia.");
    page.drawVerse("f) Informacje dotyczące stosowanych aplikacji informatycznych i oprogramowania.");
    page.drawVerse("g) Informacje dotyczące inwestycji i planowanych projektów.");
    page.drawVerse("h) Informacje dotyczące sposobu przesyłania i szyfrowania danych.");

    page.addSpace(100);
    page.drawOneLineVerses("podpis składającego oświadczenie", "");

    page.setFontSize(12);

    progressValue += 15;
    progress->setValue(progressValue);
}
