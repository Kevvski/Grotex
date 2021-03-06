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
    progress->setLabelText("Tworzenie Umowy o Prac??. Prosz?? czeka??...");

    PageDrawer page;
    page.setSaveFileName("Umowa o prac?? - " + employeeVariables.at(1) + "_" + employeeVariables.at(3) + ".pdf");
    page.setStartPoint(0, 100);
    page.setVerseOffset(20);
    page.pageStart();

    page.drawBegin(companyVariables.at(3) + ", " + jobContractVariables.at(5));
    page.addSpace(20);
    page.drawHeader("Umowa o Prac??", PageDrawer::Align::CENTER);
    page.drawVerse(jobContractVariables.at(7), PageDrawer::Align::CENTER);
    page.addSpace(20);
    page.drawDataVerse("Zawarta w dniu: ", jobContractVariables.at(5));
    page.addSpace(20);
    page.drawBoldVerse("Mi??dzy:");
    page.drawBoldVerse(companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " " + companyVariables.at(5));
    page.drawBoldVerse("Reprezentowanego przez: " + companyVariables.at(1));
    page.addSpace(20);
    page.drawBoldVerse("a:");

    drawEmployeeData(page, employeeVariables);

    page.addSpace(30);
    page.setVerseOffset(40);
    page.drawDataNewVerseRect("Rodzaj wykonywanej pracy: ", jobContractVariables.at(0), 546, true);
    page.drawDataNewVerseRect("Miejsce pracy: ", companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " " + companyVariables.at(5), 546, true);
    page.drawDataNewVerseRect("Wynagrodzenie: ", jobContractVariables.at(2) + "z?? brutto", 546, true);

    const int weekHours = jobContractVariables.at(4).toInt() * 5;
    if(decision)
        page.setVerseOffset(14);

    page.drawDataNewVerseRect("Ilo???? godzin: ", jobContractVariables.at(4) + " godzin dziennie, " + QString::number(weekHours) + " godzin tygodniowo", 546, true);

    if(decision){
        page.setVerseOffset(40);
        page.drawInfo("(zgodnie z Ustaw?? z dnia 27-08-1997r., o rehabilitacji zawodowej i spo??ecznej oraz zatrudniania os??b niepe??nosprawnych)", PageDrawer::Align::CENTER, 20);
    }

    page.drawDataNewVerseRect("Termin rozpocz??cia: ", jobContractVariables.at(5), 546, true);
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
    progress->setLabelText("Tworzenie Umowy Zlecenie. Prosz?? czeka??...");

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
    page.drawBoldVerse("Mi??dzy:");
    page.drawBoldVerse(companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " " + companyVariables.at(5));
    page.drawBoldVerse("Reprezentowanego przez: " + companyVariables.at(1));

    page.addSpace(20);
    page.drawBoldVerse("a:");

    drawEmployeeData(page, employeeVariables);

    page.addSpace(15);
    page.drawBoldVerse(QString::fromUtf8("\0xA7") + "1.", PageDrawer::Align::CENTER);
    page.drawVerse("\"Zleceniodawca\" zleca, a \"Zleceniobiorca\" przyjmuj?? us??ug?? do wykonania polegaj??c?? na:");
    page.drawDataSameVerseRect(" ", orderContractVariables.at(6), 300);
    page.drawDataNewVerseRect("przez", orderContractVariables.at(1) + " godzin dziennie", 300);
    page.drawVerse("ze stawk?? godzinow?? wed??ug najnowszych przepis??w obowi??zuj??cych na terenie kraju.");

    page.addSpace(15);
    page.drawBoldVerse(QString::fromUtf8("\0xA7") + "2.", PageDrawer::Align::CENTER);
    page.drawDataRect("\"Zleceniodawca\" zobowi??zuj?? si?? wykona?? zlecenie najp????niej do dnia: ", orderContractVariables.at(3), 200);

    page.addSpace(15);
    page.setFontSize(10);
    page.setVerseOffset(12);
    page.drawBoldVerse("3.", PageDrawer::Align::CENTER);
    page.drawVerse("Za prawid??owe wykonanie zlecenia okre??lonego w \"1.\". Zleceniobiorcy przys??uguje wynagrodzenie w wysoko??ci wyliczonego");
    page.drawVerse("\"Rachunku do umowy zlecenia\" opracowanego przez Zleceniobiorc??.");

    page.addSpace(15);
    page.drawBoldVerse("4.", PageDrawer::Align::CENTER);
    page.drawVerse("Wynagrodzenie za wykonanie zlecenia wyp??acone zostanie na podstawie rachunku wystawionego co miesi??c, w kt??rym umowa");
    page.drawVerse("obowi??zuj?? przez \"Zleceniobiorc??\" i dostarczonego do \"Zleceniodawcy\". Wynagrodzenie zostanie przelane na wskazane");
    page.drawVerse("konto lub p??atne w \"kasie\" firmy \"Zleceniobiorcy\" tj. " + companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " Ul." + companyVariables.at(5) + ",");
    page.drawVerse("\"Zleceniobiorcy\" najp????niej w ci??gu 14 dni od otrzymania rachunku przez \"Zleceniodawce\". Czas pracy b??dzie opracowywany");
    page.drawVerse("tzw.: \"Ewidenca czasu pracy\". Wyp??ata b??dzie wyliczana w akordzi na co \"Zleceniobiorca\" i \"Zleceniodawca\" wyra??a zgod??");
    page.drawVerse("podpisuj??c umow??.");

    page.addSpace(15);
    page.drawBoldVerse("5.", PageDrawer::Align::CENTER);
    page.drawVerse("W prawach nieuregulowanych stosuje si?? przepisy Kodeksu cywilnego i innych obowi??zauj??cyh akt??w prawnych.");

    page.addSpace(15);
    page.drawBoldVerse("6.", PageDrawer::Align::CENTER);
    page.drawVerse("Zmiany w umowie wymagaj?? formy pisemnej pod rygorem nie wa??no??ci obowi??zuj??cej umowy.");

    page.addSpace(15);
    page.drawBoldVerse("7.", PageDrawer::Align::CENTER);
    page.drawVerse("Umowa zosta??a sporz??dzona w 2 jednobrzmi??cych egzemplarzach, po jednym dla ka??dej ze stron. Do????czone do umowy za????czniki");
    page.drawVerse("stanowi?? integraln?? cz?????? umowy.");

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
    progress->setLabelText("Tworzenie Umowy o Dzie??o. Prosz?? czeka??...");

    PageDrawer page;
    page.setSaveFileName("Umowa o Dzie??o - " + employeeVariables.at(1) + "_" + employeeVariables.at(3) + ".pdf");
    page.setStartPoint(0, 100);
    page.setVerseOffset(20);
    page.pageStart();

    page.drawBegin(companyVariables.at(3) + ", " + workContractVariables.at(4));

    page.addSpace(20);
    page.drawHeader("Umowa o Dzie??o", PageDrawer::Align::CENTER);
    page.drawVerse(workContractVariables.at(0), PageDrawer::Align::CENTER);

    page.addSpace(20);
    page.drawDataVerse("Zawarta w dniu:", workContractVariables.at(4));

    page.addSpace(20);
    page.drawBoldVerse("Mi??dzy:");
    page.drawBoldVerse(companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " " + companyVariables.at(5));
    page.drawBoldVerse("Reprezentowanego przez: " + companyVariables.at(1));

    page.addSpace(20);
    page.drawBoldVerse("a:");

    drawEmployeeData(page, employeeVariables);

    page.addSpace(20);
    page.drawVerse("\"Zamawiaj??cy\" zamawia, a \"Wykonawca\" wykonuj?? prace polegaj??ce na:");

    QStringList jobs = workContractVariables.at(2).split(",");
    for(int i = 0; i < jobs.size(); ++i){
        page.drawDataNewVerseRect(QString::number(i), jobs.at(i), 600);
    }
    page.drawDataRect("Strony ustalaj?? termin realizacji dzie??a na dzie??: ", workContractVariables.at(4), 200);

    page.addSpace(20);
    page.drawVerse("Do wykonania dzie??a \"Zamawiaj??cy\" powierza:");
    QStringList trusts = workContractVariables.at(3).split(",");
    for(int i = 0; i < trusts.size(); ++i){
        page.drawDataNewVerseRect(QString::number(i), trusts.at(i), 600);
    }
    page.drawDataRect("Za wykonanie \"Zamawiaj??cy\" zap??aci \"Wykonuj??cemu\" kwot??:", workContractVariables.at(1), 200);
    page.drawVerse("w terminie 7 dni od otrzymania \"Rachunku do umowy\" od \"Wykonuj??cego\"");

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
    progress->setLabelText("Tworzenie Ewidencji zwolnie?? chorobowych.");

    PageDrawer page;
    page.setSaveFileName("Ewidencja zwolnie?? chorobowych - " + date + ".pdf");
    page.setStartPoint(0, 100);
    page.setVerseOffset(20);
    page.setColumnsCount(7);
    page.setTableOffset(50);
    page.pageStart();

    page.drawBegin(companyVariables.at(3) + ", " + QDate::currentDate().toString("dd.MM.yyyy"));

    progress->setValue(25);

    page.addSpace(20);
    page.drawHeader("Ewidencja zwolnie?? chorobowych", PageDrawer::Align::CENTER);
    page.drawVerse(date, PageDrawer::Align::CENTER);

    progress->setValue(50);

    page.addSpace(20);
    page.drawCell("Lp.", 60);
    page.drawCell("Imi??", 80);
    page.drawCell("Nazwisko", 80);
    page.drawCell("Pesel", 100);
    page.drawCell("Data rozpocz??cia", 120);
    page.drawCell("Data zako??czenia", 120);
    page.drawCell("Ilo???? dni", 100);

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
    progress->setLabelText("Tworzenie Ewidencji urlop??w.");

    PageDrawer page;
    page.setSaveFileName("Ewidencja urlop??w - " + date + ".pdf");
    page.setStartPoint(0, 100);
    page.setVerseOffset(20);
    page.setColumnsCount(8);
    page.setTableOffset(-5);
    page.pageStart();

    page.drawBegin(companyVariables.at(3) + ", " + QDate::currentDate().toString("dd.MM.yyyy"));

    progress->setValue(25);

    page.addSpace(20);
    page.drawHeader("Ewidencja urlop??w", PageDrawer::Align::CENTER);
    page.drawVerse(date, PageDrawer::Align::CENTER);

    progress->setValue(50);

    page.addSpace(20);
    page.drawCell("Lp.", 60);
    page.drawCell("Imi??", 80);
    page.drawCell("Nazwisko", 80);
    page.drawCell("Pesel", 100);
    page.drawCell("Data rozpocz??cia", 120);
    page.drawCell("Data zako??czenia", 120);
    page.drawCell("Ilo???? dni", 100);
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
    page.drawDataNewVerseRect("Imi?? i nazwisko:", record.name + " " + record.surname, 150, true);
    page.drawDataNewVerseRect("Rodzaj umowy:", record.type, 150, true);

    page.addSpace(20);
    page.drawCell("Lp.", 100);
    page.drawCell("Ilo???? godzin", 100);

    QStringList days = record.days.split(",");
    QStringList hours = record.hours.split(",");
    QStringList showHours {"2", "4", "6", "7", "8", "9", "10", "Urlop wyp.", "Urlop rodz.", "Urlop bezp??.", "Chorobowe"};

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
    page.drawBoldVerse("Mi??dzy:");
    page.drawVerse(companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " " + companyVariables.at(5));
    page.drawVerse("reprezentowanego przez: Piotr Strauss - specjalist?? ds. administracyjnych i kadr.");

    progress->setValue(25);

    page.addSpace(20);
    page.drawBoldVerse("a:");
    drawEmployeeData(page, employeeVariables);

    page.addSpace(20);
    page.drawBoldVerse("Strony wprowadzaj?? nast??puj??ce zmiany:");
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
    page.drawDataNewVerseRect("Aneks wchodzi w ??ycie z dniem:", annexVariables.at(2), 120);
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
        fileName = "Rachunek do umowy o dzie??o nr." + orderBillVariables.at(22) + " - ";
    else if(orderBillVariables.at(21) == "uop")
        fileName = "Lista p??ac nr." + orderBillVariables.at(22) + " - ";

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
        page.drawHeader("Rachunek do umowy o dzie??o", PageDrawer::Align::CENTER);
        page.drawVerse("Nr. " + orderBillVariables.at(0) + " do " + workContractVariables.at(0), PageDrawer::Align::CENTER);
    }
    else if(orderBillVariables.at(21) == "ud"){
        page.drawHeader("Lista p??ac", PageDrawer::Align::CENTER);
        page.drawVerse("Nr. " + orderBillVariables.at(0) + " do " + jobContractVariables.at(0), PageDrawer::Align::CENTER);
    }

    page.addSpace(20);
    page.drawBoldVerse("Wystawi??:");
    drawEmployeeData(page, employeeVariables);

    page.addSpace(20);
    page.drawDataNewVerseRect("Urz??d Skarbowy:", companyVariables.at(7), 400);

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
    page.drawDataNewVerseRect("Wynagrodzenie brutto:", QString::number(orderBillVariables.at(1).toFloat()) + " z??", 150, true);

    if(orderBillVariables.at(21) == "uop"){
        page.drawDataNewVerseRect("Wyna. zasadnicze brutto:", QString::number(orderBillVariables.at(23).toFloat()) + " z??", 150, true);
        page.drawDataNewVerseRect("Wyna. urlopowe:", QString::number(orderBillVariables.at(24).toFloat()) + " z??", 150, true);
        page.drawDataNewVerseRect("Zasi??ek chorobowy:", QString::number(orderBillVariables.at(25).toFloat()) + " z??", 150, true);
    }

    page.drawDataSameVerseRect("Ub. emerytalne(pracownik):", QString::number(orderBillVariables.at(2).toFloat()) + " z??", 150, true);
    page.drawDataNewVerseRect("Ub. emerytalne(pracodawca):", QString::number(orderBillVariables.at(6).toFloat()) + " z??", 150, true);
    page.drawDataSameVerseRect("Ub. rentowe(pracownik):", QString::number(orderBillVariables.at(3).toFloat()) + " z??", 150, true);
    page.drawDataNewVerseRect("Ub. rentowe(pracodawca):", QString::number(orderBillVariables.at(7).toFloat()) + " z??", 150, true);
    page.drawDataSameVerseRect("Ub. chorobowe(pracownik):", QString::number(orderBillVariables.at(4).toFloat()) + " z??", 150, true);
    page.drawDataNewVerseRect("Ub. wypadkowe(pracodawca):", QString::number(orderBillVariables.at(8).toFloat()) + " z??", 150, true);
    page.drawDataSameVerseRect("Suma sk??adek(pracownik):", QString::number(orderBillVariables.at(5).toFloat()) + " z??", 150, true);
    page.drawDataNewVerseRect("Suma sk??adek(pracodawca):", QString::number(orderBillVariables.at(9).toFloat()) + " z??", 150, true);
    page.drawDataSameVerseRect("Koszt uzyskania przychodu:", QString::number(orderBillVariables.at(10).toFloat()) + " z??", 150, true);
    page.drawDataNewVerseRect("Ub. zdrowotne(9%):", QString::number(orderBillVariables.at(13).toFloat()) + " z??", 150, true);
    page.drawDataSameVerseRect("Doch??d do opodatkowania:", QString::number(orderBillVariables.at(11).toFloat()) + " z??", 150, true);
    page.drawDataNewVerseRect("Ub. zdrowotne(7,75%):", QString::number(orderBillVariables.at(14).toFloat()) + " z??", 150, true);
    page.drawDataSameVerseRect("Podatek:", QString::number(orderBillVariables.at(12).toFloat()) + " z??", 150, true);
    page.drawDataNewVerseRect("Ub. zdrowotne(1,25%):", QString::number(orderBillVariables.at(15).toFloat()) + " z??", 150, true);
    page.drawDataSameVerseRect("Fundusz Pracy:", QString::number(orderBillVariables.at(16).toFloat()) + " z??", 150, true);
    page.drawDataNewVerseRect("Netto do wyp??aty:", QString::number(orderBillVariables.at(18).toFloat()) + " z??", 150, true);
    page.drawDataNewVerseRect("FG??P:", QString::number(orderBillVariables.at(17).toFloat()) + " z??", 150, true);

    progress->setValue(50);

    if(orderBillVariables.at(21) == "uz"){
        page.addSpace(15);
        page.setVerseOffset(12);
        page.drawBoldVerse("O??WIADCZENIA I PODPIS ZLECENIOBIORCY(zaznaczy?? w??a??ciwe):");
        page.drawInfo("1.O??wiadczam, i?? nie jestem zarejestrowany jako osoba bezrobotna w Urz??dzie Pracy.");
        page.drawInfo("2.O??wiadczam, i?? w zakresie przedmiotu umowy nie prowadz?? dzia??alno??ci gospodarczej w rozumieniu ustawy o podatku dochodowym od os??b fizycznych.");
        page.drawInfo("3.Za????czam do rachunku wniosek o obj??cie dobrowolnym ubezpieczeniem chorobowym.");
        page.drawInfo("4.O??wiadczam, i?? ????czna kwota moich przychod??w, stanowi??cych podstaw?? do naliczenia sk??adek na ubezpieczenia spo??eczne, jest w przeliczeniu na");
        page.drawInfo("pe??en miesi??c ni??sza od minimalnego wynagrodzenia za prac??.");
        page.drawInfo("5.O??wiadczam, i?? jestem uczniem/studentem i nie uko??czy??em 26 roku ??ycia.");
        page.drawInfo("6.O??wiadczam, i?? posiadam inny tytu?? ubezpieczenia, a kwota brutto wynagrodzenia z tego tyty??u jest wy??sza od minimalnego wynagrodzenia za prac??.");

        page.addSpace(15);
        page.drawBoldVerse("O??WIADCZENIA I PODPIS ZLECENIODAWCY");
        page.drawInfo("1.Stwierdzam wykonanie zlecenia.");
        page.drawInfo("2.Zatwierdzam do wyp??aty.");
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
        page.drawDataRect("Stawka na godzin??:", QString::number(orderContractVariables.at(4).toFloat()) + " z??", 100, 200);
    }
    page.drawDataRect("Spos??b p??atno??ci:", orderBillVariables.at(19), 100, 200);

    if(orderBillVariables.at(21) == "uz")
        page.addSpace(40);
    else
        page.addSpace(100);
    page.drawOneLineVerses("Potwierdzam odbi??r kwoty " + QString::number(orderBillVariables.at(18).toFloat()) + " z??", "Podpis zleceniodawcy");

    progress->setValue(100);

    page.pageEnd();
    progress->reset();
}


void PrintDocuments::printJobListBill(QVector<Data::BillRecord>& records){
    PageDrawer page;
    page.setSaveFileName("Zbiorcza Lista P??ac - " + records.at(0).date.toString("dd.MM.yyyy") + ".pdf");
    page.setStartPoint(30, 100);
    page.setVerseOffset(20);
    page.setOrientation(QPageLayout::Orientation::Landscape);
    page.setColumnsCount(17);
    page.setTableOffset(25);
    page.pageStart();

    QDate date = records.at(0).date;
    page.drawHeader("Lista p??ac nr. " + QString::number(date.month()) + "/" + QString::number(date.year()), PageDrawer::Align::CENTER);
    page.drawVerse("za miesi??c " + date.toString("MMMM, yyyy"), PageDrawer::Align::CENTER);

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
    page.drawCell("Do\nWyp??aty", 60, 30, true);
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

    page.drawCell("Dane narastaj??co z poprzednich stron", 150, 30, true);
    page.drawCell("Sk??adki\nEmer.", 60, 30, true);
    page.drawCell("Sk??adki\nRent.", 60, 30, true);
    page.drawCell("Sk??adki\nChorob.", 60, 30, true);
    page.drawCell("Sk??adki\nWyp.", 60, 30, true);
    page.drawCell("Sk??adki\nZdrow.", 60, 30, true);
    page.drawCell("Fundusz\nPracy", 60, 30, true);
    page.drawCell("FG??P", 60, 30, true);
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
    page.drawCell("Potr??cenia nale??ne one pracownik??w", 150, 10);
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

    page.drawCell("Sk??adki nale??ne od pracownik??w", 150, 10);
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
    page.drawBoldVerse("Sporz??dzi??(data i podpis):");

    page.addSpace(20);
    page.drawBoldVerse("Zatwierdzi??(data i podpis):");

    page.addSpace(20);
    page.drawBoldVerse("Wyp??aci??(data i podpis):");

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
    page.setSaveFileName("Paski wynagrodze?? - " + records.at(0).date.toString("MMMM.yyyy") + ".pdf");
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
        page.drawCell("Sk??adki", 650, 10);

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
        page.drawCell("FG??P.", 72, 10);
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
        page.drawVerse("Osoba wystawiaj??ca: Piotr Strauss");

        posY += 30;
        currentBill++;
    }

    page.pageEnd();
}


void PrintDocuments::printPayCertificate(QVector<Data::BillRecord> &records){
    PageDrawer page;
    page.setStartPoint(0, 100);
    page.setVerseOffset(20);
    page.setSaveFileName("Za??wiadczenie o zatrudnieniu - " + employeeVariables.at(1) + " " + employeeVariables.at(3) + ".pdf");
    page.pageStart();
    page.drawBegin(companyVariables.at(3) + ", " + QDate::currentDate().toString("dd.MM.yyyy"));

    page.addSpace(40);
    page.drawHeader("Za??wiadczenie o zatrudnieniu", PageDrawer::Align::CENTER);

    page.addSpace(20);
    page.drawBoldVerse("Za??wiadcza si?? ??e:");

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
    page.drawBoldVerse("W nast??puj??cych miesi??cach otrzyma??/a wynagrodzenie:");

    page.addSpace(20);
    page.setColumnsCount(5);
    page.setVerseOffset(30);
    page.drawCell("Miesi??c, Rok", 120, 10, true);
    page.drawCell("Kwota Brutto", 120, 10, true);
    page.drawCell("Sk??adki ZUS", 120, 10, true);
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
    page.drawVerse("Niniejsze za??wiadczenie wydaj?? si?? na pro??b?? osoby zainteresowanej.");

    page.addSpace(100);
    page.drawOneLineVerses("(data i podpis  osoby upowa??nionej)", "");

    progress->setValue(100);
    page.pageEnd();
}


void PrintDocuments::printHouseCertificate(QVector<Data::BillRecord> &records){
    PageDrawer page;
    page.setStartPoint(0, 100);
    page.setVerseOffset(20);
    page.setSaveFileName("Za??wiadczenie o dodatku mieszkaniowym - " + employeeVariables.at(1) + " " + employeeVariables.at(3) + ".pdf");
    page.pageStart();
    page.drawBegin(companyVariables.at(3) + ", " + QDate::currentDate().toString("dd.MM.yyyy"));

    page.addSpace(40);
    page.drawHeader("Za??wiadczenie", PageDrawer::Align::CENTER);

    page.addSpace(20);
    page.drawDataNewVerseRect("Za??wiadcza si?? ??e Pan/i:", employeeVariables.at(1) + " " + employeeVariables.at(3), 200);
    page.drawDataNewVerseRect("Zamieszka??y/a:", employeeVariables.at(7) + " " + employeeVariables.at(6) + " " + employeeVariables.at(8) + " " + employeeVariables.at(9) + "/" + employeeVariables.at(10), 400);

    QDate startDate;
    QDate endDate;
    if(records.at(0).payType == "uop"){
        startDate = QDate::fromString(jobContractVariables.at(5), "yyyy-MM-dd");
        endDate = QDate::fromString(jobContractVariables.at(6), "yyyy-MM-dd");

        page.drawDataNewVerseRect("Jest zatrudniony/a w wymiarze czasu pracy:", jobContractVariables.at(3), 200);
        page.drawDataSameVerseRect("Stosunek pracy zawarto dnia:", jobContractVariables.at(5), 200);
        page.drawDataNewVerseRect("na czas:", QString::number(endDate.month() - startDate.month()) + " miesi??cy", 200);
    }
    else if(records.at(0).payType == "uz"){
        startDate = QDate::fromString(orderContractVariables.at(2), "yyyy-MM-dd");
        endDate = QDate::fromString(orderContractVariables.at(3), "yyyy-MM-dd");

        page.drawDataNewVerseRect("Jest zatrudniony/a w wymiarze czasu pracy:", orderContractVariables.at(5), 200);
        page.drawDataSameVerseRect("Stosunek pracy zawarto dnia:", orderContractVariables.at(2), 200);
        page.drawDataNewVerseRect("na czas:", QString::number(endDate.month() - startDate.month()) + " miesi??cy", 200);
    }
    else if(records.at(0).payType == "ud"){
        startDate = QDate::fromString(workContractVariables.at(4), "yyyy-MM-dd");
        endDate = QDate::fromString(workContractVariables.at(5), "yyyy-MM-dd");

        page.drawDataNewVerseRect("Jest zatrudniony/a w wymiarze czasu pracy:", workContractVariables.at(6), 200);
        page.drawDataSameVerseRect("Stosunek pracy zawarto dnia:", workContractVariables.at(4), 200);
        page.drawDataNewVerseRect("na czas: ", QString::number(endDate.month() - startDate.month()) + " miesi??cy", 200);
    }
    page.addSpace(20);
    page.drawBoldVerse("Doch??d wyp????cony w ostatnich miesi??cach wynosi:");

    page.setColumnsCount(5);
    page.setVerseOffset(35);
    page.setFontSize(8);
    page.drawCell("Miesi??c/Rok", 120, 20, true);
    page.drawCell("Kwota Brutto", 120, 20, true);
    page.drawCell("Sk??adki na ubez.\nspo??eczne", 120, 20, true);
    page.drawCell("Koszt uzyskania\nprzychodu", 120, 20, true);
    page.drawCell("Doch??d do opodatkowania", 130, 20, true);

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
    page.drawBoldVerse("Za??wiadczenie wydaje si?? w celu: Naliczenia dodatku mieszkaniowego");

    page.addSpace(40);
    page.drawOneLineVerses("", "podpis i piecz???? osoby upowa??nionej");

    page.addSpace(20);
    page.setFontSize(8);
    page.drawVerse("Pouczenie:");
    page.drawVerse("Za doch??d zgodnie z art. 3 ust. 3 o dodatkach mieszkaniowych z dnia 21 czerwca 2001r..(Dz. U. Nr. 71 z 2001r. poz. 734) uwa??a si?? wszelkie");
    page.drawVerse("przychody po odliczeniu koszt??w ich uzyskania oraz po odliczeniu sk??adek na ubezpieczenie emerytalne i rentowne oraz na ubezpieczenie");
    page.drawVerse("chorobowe, okre??lonych w przepisach o systemie ubezpiecze?? spo??ecznych, chyba ??e zosta??y ju?? zaliczone do koszt??w uzyskania przychodu.");

    page.pageEnd();
    progress->setValue(100);
}


void PrintDocuments::printFamilyOrMaintanceCertificate(const Data::BillRecord &record, int index){
    PageDrawer page;
    page.setStartPoint(0, 100);
    page.setVerseOffset(20);
    if(index == 0)
        page.setSaveFileName("Za??wiadczenie o zarobkach(Rodzinne) - " + employeeVariables.at(1) + " " + employeeVariables.at(3) + ".pdf");
    else if(index == 1)
        page.setSaveFileName("Za??wiadczenie o zarobkach(Alimentacyjne) - " + employeeVariables.at(1) + " " + employeeVariables.at(3) + ".pdf");

    page.pageStart();
    page.drawBegin(companyVariables.at(3) + ", " + QDate::currentDate().toString("dd.MM.yyyy"));

    page.addSpace(40);
    page.drawHeader("Za??wiadczenie o zarobkach", PageDrawer::Align::CENTER);

    page.setFontSize(8);
    if(index == 0)
        page.drawVerse("wydane dla potrzeb ??widcze?? rodzinnych i ??wiadczenia wychowawczego(500+) dla os??b zg??aszaj??cych doch??d uzsykany", PageDrawer::Align::CENTER);
    else if(index == 1)
        page.drawVerse("wydane dla potrzeb ??widcze?? z funduszu alimentacyjnego dla os??b zg??aszaj??cych doch??d uzyskany", PageDrawer::Align::CENTER);

    page.setFontSize(12);
    page.addSpace(30);
    page.drawDataNewVerseRect("Za??wiadcza si??:", employeeVariables.at(1) + " " + employeeVariables.at(3), 200);
    page.drawDataNewVerseRect("Nr. PESEL:", employeeVariables.at(11), 200);
    page.drawDataNewVerseRect("Zamieszka??y/a:", employeeVariables.at(7) + " " + employeeVariables.at(6) + " " + employeeVariables.at(8) + " " + employeeVariables.at(9) + "/" + employeeVariables.at(10), 400);
    page.drawDataNewVerseRect("Jest zatrudniony/a:", companyVariables.at(0) + " " + companyVariables.at(3) + " " + companyVariables.at(4) + " " + companyVariables.at(5), 400);

    page.addSpace(20);
    QString text = "  Na podstawie ";
    if(record.payType == "uop"){
        text += "umowy o prac?? w okresie od " + jobContractVariables.at(5) + " do " + jobContractVariables.at(6) + " w wymiarze " + jobContractVariables.at(3) + ".";
    }
    else if(record.payType == "uz"){
        text += "umowy zlecenie w okresie od " + orderContractVariables.at(2) + " do " + orderContractVariables.at(3) + " w wymiarze " + orderContractVariables.at(5) + ".";
    }
    else if(record.payType == "ud"){
        text += "umowy o dzie??o w okresie od " + workContractVariables.at(4) + " do " + workContractVariables.at(5) + " w wymiarze " + workContractVariables.at(6) + ".";
    }

    page.drawVerse(text);

    page.addSpace(20);
    page.drawDataNewVerseRect("Wynagrodzenie za:", record.docId, 200);
    page.drawDataNewVerseRect("Data wyp??aty wynagrodzenia:", record.date.toString("dd.MM.yyyy"), 200);

    page.addSpace(20);
    page.drawDataNewVerseRect("Przych??d:", QString::number(record.rewardBrutto), 200);
    page.drawDataNewVerseRect("Koszt uzyskania przychodu:", QString::number(record.incomeCost), 200);
    page.drawDataNewVerseRect("Nale??ny podatek dochodowy od os??b fizycznych:", QString::number(record.rate), 200);
    page.drawDataNewVerseRect("Sk??adki na ubezpieczenie spo??eczne:", QString::number(record.iSum), 200);
    page.drawDataNewVerseRect("Sk??adka na ubezpieczenie zdrowotne(9%):", QString::number(record.insurance), 200);
    page.drawDataNewVerseRect("Doch??d uzyskany:", QString::number(record.rewardBrutto - record.incomeCost - record.rate - record.iSum - record.insurance), 200);

    page.addSpace(130);
    page.drawOneLineVerses("", "(piecz???? zak??adu pracy)");

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
    page.drawDataNewVerseRect("Dni urlopowe(Pozosta??o): ", freeDays, 100, true);

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
    page.drawCell("Data wzi??cia", 100, 10, true);
    page.drawCell("Data zako??czenia", 100, 10, true);
    page.drawCell("Rodzaj urlopu", 100, 10, true);
    page.drawCell("Ilo???? dni", 100, 10, true);

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
    page.drawBoldVerse("Ewidencja niezdolno??ci do pracy:");

    page.setVerseOffset(21);
    page.setFontSize(8);
    page.resetCells();
    page.setColumnsCount(3);
    page.drawCell("Data wzi??cia", 100, 10, true);
    page.drawCell("Data zako??czenia", 100, 10, true);
    page.drawCell("Ilo???? dni", 100, 10, true);

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
        fullName = "Faktura sprzeda??y - " + fileName + ".pdf";
        header = "Faktura VAT nr. " + financeVariables.at(0);
    }
    else if(state == FinanceDialog::State::WZ){
        fullName = "Wydanie Zewn??trzne - " + fileName + ".pdf";
        header = "Wydanie Zewn??trzne nr. " + financeVariables.at(0);
    }
    else if(state == FinanceDialog::State::PZ){
        fullName = "Przyj??cie Zewn??trzne - " + fileName + ".pdf";
        header = "Przyj??cie Zewn??trzne nr. " + financeVariables.at(0);
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
    page.drawVerse("Data sprzeda??y:  " + financeVariables.at(1));
    page.drawVerse("Spos??b zap??aty:  " + financeVariables.at(3));
    page.drawVerse("Termin p??atno??ci:  " + financeVariables.at(2));

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
    page.drawCell("Nazwa towaru/us??ugi", cellWidth, cellHeight, true);
    page.drawCell("J.M", 40, cellHeight, true);
    page.drawCell("Ilo????", 50, cellHeight, true);
    page.drawCell("Rabat(%)", 60, cellHeight, true);
    page.drawCell("Cena netto(z??)", 80, cellHeight, true);
    page.drawCell("Cena netto\npo rabacie(z??)", 80, cellHeight, true);
    page.drawCell("Warto???? netto\n(z??)", 80, cellHeight, true);
    page.drawCell("VAT(%)", 50, cellHeight, true);
    page.drawCell("Warto???? VAT\n(z??)", 80, cellHeight, true);
    page.drawCell("Warto???? brutto\n(z??)", 80, cellHeight, true);

    for(int i = 0; i < items.at(0).size(); ++i){
        if(i != 0 && i % 12 == 0){
            page.toNextPage();
            page.setStartPoint(0, 100);
            page.drawCell("Nazwa towaru/us??ugi", cellWidth, cellHeight, true);
            page.drawCell("J.M", 40, cellHeight, true);
            page.drawCell("Ilo????", 50, cellHeight, true);
            page.drawCell("Rabat(%)", 60, cellHeight, true);
            page.drawCell("Cena netto(z??)", 80, cellHeight, true);
            page.drawCell("Cena netto\npo rabacie(z??)", 80, cellHeight, true);
            page.drawCell("Warto???? netto\n(z??)", 80, cellHeight, true);
            page.drawCell("VAT(%)", 50, cellHeight, true);
            page.drawCell("Warto???? VAT\n(z??)", 80, cellHeight, true);
            page.drawCell("Warto???? brutto\n(z??)", 80, cellHeight, true);
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
    page.drawDataNewVerseRect("Razem netto(z??):", QString::number(rtd(financeVariables.at(15).toFloat())), 200);
    page.drawDataNewVerseRect("Razem VAT(z??):", QString::number(rtd(financeVariables.at(16).toFloat())), 200);
    page.drawDataNewVerseRect("Razem brutto(z??):", QString::number(rtd(financeVariables.at(17).toFloat())), 200);

    page.addSpace(60);
    page.setFontSize(8);

    if(state == FinanceDialog::State::FS)
        page.drawOneLineVerses("Osoba upowa??niona do otrzymania faktury VAT", "Osoba upowa??niona do wystawienia faktury VAT");
    else
        page.drawOneLineVerses("          Odebra??          ", "          Wystawi??          ");

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
    page.setSaveFileName("O??wiadczenie Kompensaty - " + name + ".pdf");
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
    page.drawHeader("O??WIADCZENIE O KOMPENSACIE", PageDrawer::Align::CENTER);

    page.addSpace(20);
    page.drawVerse("Uprzejmie informujemy,??e na podstawie art. 498 Kodeksu Cywilnego dokonali??my kompensaty:");

    page.setColumnsCount(2);
    page.drawCell("naszych zobowi??za??, wynikaj??cych z:", 380);
    page.drawCell("z naszymi nale??no??ciami, wynikaj??cych z:", 380);
    page.resetCells();

    page.setVerseOffset(49);
    page.setColumnsCount(6);
    page.drawCell("Dokument\n(nazwa i numer)", 180, 30);
    page.drawCell("Data\nDokumentu", 100, 30);
    page.drawCell("Kwota(z??)", 100, 30);
    page.drawCell("Dokument\n(nazwa i numer)", 180, 30);
    page.drawCell("Data\nDokumentu", 100, 30);
    page.drawCell("Kwota(z??)", 100, 30);

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
    page.drawCell("na ????czn?? kwot??", 280, 10);
    page.drawCell(QString::number(buySum), 100, 10);
    page.drawCell("na ????czn?? kwot??", 280, 10);
    page.drawCell(QString::number(sellSum), 100, 10);
    page.setVerseOffset(20);

    page.addSpace(20);
    page.drawVerse("Po dokonaniu kompensaty do zap??aty na nasze/Pa??stwa dobro pozostaje kwota: " + QString::number(std::abs(balance)) + " z??");
    page.drawVerse("(s??ownie:                                                                                                                       )");

    page.addSpace(40);
    page.drawVerse("Nierozliczon?? w drodze kompensaty nale??no???? prosimy przela?? na konto bankowe o numerze");
    page.drawVerse("                                                                                           do dnia                                 r.");

    page.addSpace(40);
    page.drawVerse("Nierozliczone w drodze kompensaty zobowi??zanie zosta??o/zostanie uregulowane w/do dniu/dnia                          r.");

    page.addSpace(40);
    page.drawVerse("Prosimy o zgodne z nami ksi??gowanie.");

    page.addSpace(60);
    page.setFontSize(8);
    page.drawOneLineVerses("", "podpis osoby upowa??nionej do sk??adania o??wiadcze??");

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
    page.drawHeader("Postanowienia Og??lne", PageDrawer::Align::CENTER);

    page.addSpace(20);
    page.drawVerse("1. W przypadku nienale??ytego wykonania pracy, pracodawca ma prawo odmowy wyp??aty ca??o??ci lub cz????ci");
    page.drawVerse("    wynagrodzenia. Je??li pracownik wypowie umow?? i nie zawiadomi o tym fakcie pracodawcy na pi??mie");
    page.drawVerse("    najp????nirj w nast??pnym dniu roboczym, od daty zaprzestania wykonywania pracy to pracodawca jest");
    page.drawVerse("    uprawniony do zarz??dzania od pracownika zap??aty kary umownej wysoko??ci stanowi??cej 20% wynagrodzenia");
    page.drawVerse("    zasadniczego nale??nego za miesi??c, w kt??rym zaprzesta?? on wykonywania swych czynno??ci, postanowienie");
    page.drawVerse("    to nie wy????cza prawa pracodawcy do dochodzenia dalej id??cych roszcze?? odszkodowawczych z tego tytu??u.");

    page.addSpace(10);
    page.drawVerse("2. Pracownik jest zobowi??zany do stosowania si?? przyj??tych zasad i norm oraz procedur obowi??zuj??cych");
    page.drawVerse("    w firmie, dotycz??cych wykonywania pracy wed??ug potrzeb pracodawcy, naruszenie powy??szych obowi??zk??w");
    page.drawVerse("    przez pracownika, b??dzie skutkowa??o na??o??enia kary umownej wysoko??ci wskazanej wed??ug pracodawcy,");
    page.drawVerse("    nie wi??kszej ni?? 100z?? za ka??dy przypadek.");

    page.addSpace(10);
    page.drawVerse("3. Pracownik odpowiada w pe??nej wysoko??ci na zasadach okre??lonych przepisami kodeksu cywilnego za wszelkie");
    page.drawVerse("    szkody wyrz??dzone przez niego na rzecz pracodawcy lub jego klientom, kontrahentom zwi??zku z wykonywaniem");
    page.drawVerse("    albo nienale??ytym wykonaniem swych zada??.");

    page.addSpace(10);
    page.drawVerse("4. Pracownik mo??e powierzy?? wykonanie zleconych mu czynno??ci innej osobie jedynie za uprzedni?? pisemn?? zgod??");
    page.drawVerse("    Pracodawcy, Pracownik ten powinien wykonywa?? powierzone mu czynno??ci zachowanej nale??ytej staranno??ci");
    page.drawVerse("    wymaganej uwagi na szczeg??lny charakter. Pracownik ten zobowi??zuj?? si?? przy tym do takiego anga??owania");
    page.drawVerse("    wykonania danej pracy jakie b??dzie wymagane z uwagi na stopie?? trudno??ci wykonywanego zadania.");

    page.addSpace(10);
    page.drawVerse("5. Pracodawca nie odpowiada za szkody wyrz??dzone przez pracownika lub przez osoby trzecie wykonuj??ce prac??");
    page.drawVerse("    w zast??pstwie na rzecz pracodawcy.");

    page.addSpace(10);
    page.drawVerse("6. Pracownik o??wiadcza, ??e zna przepisy oraz zasady bezpiecze??stwa i higieny pracy obowi??zuj??cych przy");
    page.drawVerse("    wykonywanych czynno??ciach wynikaj??cych z umowy i o??wiadcza, ??e b??dzie ich przestrzega??, a tak??e o??wiadcza,");
    page.drawVerse("    ??e jego stan zdrowia pozwala na wykonywanie danej pracy, czym potwierdza te tre??ci swym podpisem.");

    page.addSpace(10);
    page.drawVerse("7. W sprawach nieuregulowanych w tej umowie zastosowanie maj?? przepisy Kodeksu Cywilnego oraz przepisy prawa");
    page.drawVerse("    Ubezpiecze?? Spo??ecznych i Zdrowotnych.");

    page.addSpace(10);
    page.drawVerse("8. Strony zgodnie ustalaj??, ??e w przypadku powstania sporu, kt??rego podstaw?? roszczenia wynikaj??ce postanowie??");
    page.drawVerse("    umowy z s??dem w??a??ciwym do rozpoznania sprawy b??dzie s??d miejscowy w??a??ciwy dla zak??adu pracy.");

    page.addSpace(10);
    page.drawVerse("9. Umow?? sporz??dzono w dw??ch jednobrzmi??cych egzemplarzach po jednym dla ka??dej ze stron.");

    page.drawFooter(currentDate);

    progressValue += 15;
    progress->setValue(progressValue);
}


void PrintDocuments::drawEndProvisionsPage(PageDrawer &page, const QString &currentDate){
    page.toNextPage();
    page.setStartPoint(0, 100);
    page.drawBegin(currentDate);

    page.addSpace(40);
    page.drawHeader("Postanowienia szczeg????owe", PageDrawer::Align::CENTER);

    page.addSpace(20);
    page.drawVerse("1. Pracownik jest zobowi??zany do zachowania tajemnicy odno??nie wszystkich dokument??w przedsi??biorstwa oraz");
    page.drawVerse("    innych informacji pozyskiwanych w zwi??zku z wykonywaniem na podstawie umowy jak r??wnie?? zachowanie");
    page.drawVerse("    najwy??szej staranno??ci w celu przed ujawnieniem ich osobom Nieupowa??nionym obowi??zek powy??sze obejmuje");
    page.drawVerse("    w szczeg??lno??ci poufne informacje dotycz??ce klient??w sposob??w negocjacji prowadzenia rozm??w z klientami");
    page.drawVerse("    warunku i tre??ci zawieranych w nim um??w poufne informacje odno??nie struktury organizacyjnej i zasady");
    page.drawVerse("    dzia??ania przedsi??biorstwa zleceniodawcy oraz inne informacje, w kt??rych ujawnienie mog??oby zagrozi??");
    page.drawVerse("    lub narusza?? interes lub narazi?? pracodawc?? na szkod??, naruszenie powy??szych obowi??zk??w w szczeg??lno??ci");
    page.drawVerse("    poprzez przekazywanie lub ujawnienie informacji osobom Nieupowa??nionym, jak i r??wnie?? innym nieuprawnione");
    page.drawVerse("    ich wykorzystywanie nak??adana jest kara umowna wysoko??ci 100z?? za ka??dy przypadek narusza co nie wy????cza");
    page.drawVerse("    mo??liwo??ci dochodzenia odszkodowania na zasadach og??lnych. Obowi??zki powy??sz?? bo maj?? okres trwania umowy");
    page.drawVerse("    przez 3 lata od jej ustalenia.");

    page.addSpace(10);
    page.drawVerse("2. Pracownik przyjmuj?? do wiadomo??ci, ??e wykonane zadania b??d?? monitorowane przez pracodaw??e oraz, ??e wszelkie");
    page.drawVerse("    ich kopiowanie rozpowszechnianie poza struktury firmy bez zgody zarz??du jest zabronione. Ponadto pracownik");
    page.drawVerse("    zobowi??zuj?? si?? zachowa?? tajemnic?? zawodow??, kt??ra zostanie opisana w osobnym dokumencie stanowi??cym");
    page.drawVerse("    integraln?? cz?????? powy??szej umowy.");

    page.addSpace(10);
    page.drawVerse("3. Pracownik wyra??a zgod?? na pobieranie oraz przetwarzanie danych osobowych przez zleceniodawc?? w zakresie");
    page.drawVerse("    obejmuj??cym: imienia, nazwiska innych danych w celu sporz??dzenia umowy oraz stworzenie kartoteki osobowej");
    page.drawVerse("    i archiwizacj?? tych danych na rzecz w/w pracodawcy.");

    page.addSpace(100);
    page.drawOneLineVerses("podpis pracodawcy", "podpis pracownika");

    page.addSpace(50);
    page.drawHeader("Postanowienia ko??cowe", PageDrawer::Align::CENTER);

    page.addSpace(40);
    page.drawVerse("1. Umowa ulega rozwi??zaniu w przypadku zaprzestania stosunku pracy.");
    page.addSpace(10);
    page.drawVerse("2. Wyp??ata ostatniego wynagrodzenia po zako??czeniu umowy, wskutek jej wyga??ni??cia, nast??puj?? bezgot??wkowo");
    page.drawVerse("    przelewem na wskazane przez zleceniobiorce konto bankowe lub przekazem pocztowym na adres pracownika,");
    page.drawVerse("    w terminie do 10 dni od sporz??dzenia \"Paska wynagrodzenia osobowego\". Wyp??ata te?? mo??e nast??pi?? w");
    page.drawVerse("    \"Kasie\" przedsi??biorstwa na co pracownik udziela zgod?? podpisuj??c dan?? umow??. Pokwitowaniem takiego");
    page.drawVerse("    rodzaju wyp??acenia wynagrodzenia b??dzie druk \"KW\" dla ka??dej ze stron.");

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
    page.drawHeader("Instrukta?? og??lny - cel i program", PageDrawer::Align::CENTER);
    page.drawVerse("Szkolenie z Bezpiecze??stwa i Higieny Pracy", PageDrawer::Align::CENTER);
    page.addSpace(20);
    page.drawVerse("Celem szkolenia jest zapoznanie pracownika w szczeg??lno??ci z:");
    page.addSpace(10);
    page.drawVerse("1. Podstawowymi przepisami bezpiecze??stwa i higieny pracy zawartymi w Kodeksie pracy,");
    page.drawVerse(" w uk??adach zbiorowych Pracy lub w regulaminach pracy, przepisami oraz z zasadami");
    page.drawVerse("bezpiecze??stwa i higieny pracy obowi??zuj??cymi w zak??adzie pracy, ");
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
    page.drawCell("Istota bezpiecze??stwa i higieny pracy.", 500, 10);
    page.drawCell("0,6", 100, 65);

    page.resetCells();
    page.setColumnsCount(2);
    page.setVerseOffset(55);
    page.drawCell("3.", 100, 40);
    page.drawCell("Zakres obowi??zk??w i uprawnienia pracodawcy, pracownik??w oraz poszczeg??lnych kom??rek\n"
                  "organizacyjnych zak??adu. Odpowiedzialno???? za naruszenie przepis??w lub zasad bezpiecze??stwa i\n"
                  "higieny pracy.", 500, 40);

    page.resetCells();
    page.setColumnsCount(3);
    page.setVerseOffset(25);
    page.drawCell("4.", 100, 10);
    page.drawCell("Zasady poruszania si?? na terenie zak??adu pracy." , 500, 10);
    page.drawCell("0,5", 100, 55);

    page.resetCells();
    page.setColumnsCount(2);
    page.setVerseOffset(45);
    page.drawCell("5.", 100, 30);
    page.drawCell("Zagro??enia wypadkowe i zagro??enia dla zdrowia wyst??puj??ce w zak??adzie i podstawowe ??rodki\n"
                  "zapobiegawcze.", 500, 30);

    page.resetCells();
    page.setColumnsCount(3);
    page.setVerseOffset(45);
    page.drawCell("6.", 100, 30);
    page.drawCell("Podstawowe zasady bezpiecze??stwa i higieny pracy zwi??zane z obs??ug?? urz??dze?? technicznych\n"
                  "oraz transportem wewn??trz zak??adowym." , 500, 30);
    page.drawCell("0,4", 100, 30);

    page.drawCell("7.", 100, 30);
    page.drawCell("Zasady przydzia??u odzie??y roboczej i obuwia roboczego (je??eli s?? one wymagane na\n"
                  "stanowisku pracy), oraz ??rodki ochrony indywidualnej w tym w odniesieniu do stanowiska pracy." , 500, 30);
    page.drawCell("0,5", 100, 80);

    page.resetCells();
    page.setColumnsCount(2);
    page.setVerseOffset(25);
    page.drawCell("8.", 100, 10);
    page.drawCell("Porz??dek i czysto???? w miejscu pracy - ich wp??yw na zdrowie i bezpiecze??stwo pracownika.", 500, 10);

    page.drawCell("9.", 100, 10);
    page.drawCell("Porz??dek i czysto???? w miejscu pracy - ich wp??yw na zdrowie i bezpiecze??stwo pracownika.", 500, 10);

    page.resetCells();
    page.setColumnsCount(3);
    page.setVerseOffset(25);
    page.drawCell("10.", 100, 10);
    page.drawCell("Podstawowe zasady ochrony przeciwpo??arowej oraz post??powanie w razie po??aru.", 500, 10);
    page.drawCell("1", 100, 35);

    page.resetCells();
    page.setColumnsCount(2);
    page.setVerseOffset(25);
    page.drawCell("11.", 100, 10);
    page.drawCell("Post??powanie w razie wypadku, w tym organizacja i zasay udzielania pierwszej pomocy.", 500, 10);

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
    page.drawHeader("O??wiadczenie pracownika", PageDrawer::Align::CENTER);
    page.drawVerse("o zapoznaniu si?? z dokonan?? oceny ryzyka zawodowego", PageDrawer::Align::CENTER);
    page.addSpace(10);
    page.drawVerse("Ja ni??ej podpisany:");
    page.addSpace(10);

    drawEmployeeData(page, employee);

    page.addSpace(10);
    page.drawDataVerse("Zatrudniony/a w: ", company);
    page.addSpace(10);
    page.drawVerse("Potwierdzam, ??e zosta??em/am w dniu " + date + " poinformowany/a przez pracodawc?? o ryzyku");
    page.drawVerse("zawodowym, kt??re wi????e si?? z wykonywan?? prze ze mnie prac?? na stanowisku: " + employee.at(15));
    page.addSpace(10);
    page.drawVerse("Ponadto pracodawca poinformowa?? mnie o zasadach ochrony przeciwpo??arowej");
    page.drawVerse("obowi??zuj??cych w zak??adzie pracy.");
    page.addSpace(10);
    page.drawVerse("W szczeg??lno??ci:");
    page.drawVerse("1. Postanowienia BHP, zadania oraz procedury w zakresie ochrony");
    page.drawVerse("przeciwpo??arowej odnosz??ce si?? do mojego stanowiska pracy.");
    page.drawVerse("2. Przyczyny zapobiegania powstaniu i rozprzestrzenianiu si?? po??ar??w.");
    page.drawVerse("3. Zasady zapobiegania si?? z podr??cznym sprz??tem ga??niczym, ga??nicami");
    page.drawVerse(" oraz innymi sprz??tami ga??niczymi.");
    page.drawVerse("4. Zasady post??powania na wypadek po??aru.");
    page.addSpace(10);
    page.drawVerse("Za??wiadczam, ??e w dniu " + date + " zosta??em/am zapoznany/a z wymogami");
    page.drawVerse("Bezpiecze??stwa i Higieny Pracy obowi??zuj??cych w zak??adzie.");
    page.addSpace(10);
    page.drawVerse("Instruktarzu udzieli?? mi:");
    page.addSpace(20);
    page.drawInfo("(podpis osoby udzielaj??cej instruktora)", PageDrawer::Align::CENTER);
    page.drawFooter(currentDate);

    progressValue += 15;
    progress->setValue(progressValue);
}


void PrintDocuments::drawHopePage(PageDrawer &page, const QString &currentDate){
    page.toNextPage();
    page.setStartPoint(0, 100);
    page.drawBegin(currentDate);
    page.addSpace(40);
    page.drawHeader("Klauzula poufno??ci informacji", PageDrawer::Align::CENTER);

    page.addSpace(20);
    page.drawVerse("1. Strony zobowi??zuj?? si??:");
    page.setFontSize(10);
    page.setVerseOffset(12);
    page.drawVerse("    1.1 Do zachowania w ??cis??ej tajemnicy oraz do nie przekazywania, nie ujawniania");
    page.drawVerse("        i nie wykorzystywania informacji stanowi??zych tajemnic?? przedsi??biorstwa drugiej");
    page.drawVerse("        Stronie, a tak??e wszelkich poufnych informacji i fakt??w, o kt??rych dowiem si??");
    page.drawVerse("        w trakci wzajemnej wps????pracy lub przy okazji wsp????pracy w zwi??zku z przygotowaniem");
    page.drawVerse("        oferty, niezale??nie od formy przekazania/pozyskania tych informacji i ich ??r??d??a.");
    page.drawVerse("        Przepis ten dotyczy w szczeg??lno??ci nie przekazywania, nie ujawniania i niewykorzystywania");
    page.drawVerse("        infromacji oraz dokumentacji okre??lonej w za????czonej \"DEKLARACJI w zakresie poufno??ci informacji\",");
    page.drawVerse("        stanowi??cyh tajemnic przedsi??biorstwa w my??l art. 11 ust. 4 Ustawy o Zwalczaniu Nieuczciwej");
    page.drawVerse("        Konkurencji z dnia 16 kwietnia 1993r.");
    page.addSpace(10);
    page.drawVerse("    1.2 Wykorzystywania informacji, o kt??rych mowa w ust. 1.1 jedynie w celach okre??lonych");
    page.drawVerse("        ustaleniami przez Strony w zwi??zki z przygotowaniem oferty.");
    page.addSpace(10);
    page.drawVerse("    1.3 Podj???? wszelkie niezb??dne kroki dla zapewnienia, ??e ??adna z os??b otrzymuj??cych informacje");
    page.drawVerse("        nie ujawni tych informacji, ani ich ??r??d??a zar??wno w ca??o??ci jak i cz????ci, stronom trzecim");
    page.drawVerse("        bez uzsykania przedniego wybranego upowa??nienia na pi??mie od Strony, kt??rej informacja lub");
    page.drawVerse("        ??r??d??o dotyczy. Strona, kt??ra przekazuje informacje drugiej Stronie, odpowiada za osoby, ");
    page.drawVerse("        kt??ry, te informacje zostaj?? udost??pnione/przekazane jak za w??asne dzia??anie lub zaniechanie, ");
    page.drawVerse("        w szczeg??lno??ci ponosi odpowiedzialno???? za zachowanie przestrzegania postanowienia ust. 1.1 i 1.2");
    page.drawVerse("        Ujawnia informacje, o kt??rych mowa w ust 1.1 jedynie tym pracownikom, wsp????pracownikom i ");
    page.drawVerse("        doradcom Stron, kt??rym b??d?? one niezb??dne do wykonania powierzonych im czynno????i i tylko w ");
    page.drawVerse("        zakresie w jakim odbiorca informacji musi mie?? dost??p dla cel??w okre??lonych w ust. 1.2.");

    page.setFontSize(12);
    page.setVerseOffset(20);
    page.addSpace(15);
    page.drawVerse("2. Postanowienia ust. 1.1 - 1.4 nie b??d?? mia??y zastosowania do tych informacji uzyskanych od drugiej Strony,");
    page.drawVerse("kt??re:");
    page.setFontSize(10);
    page.setVerseOffset(12);
    page.drawVerse("    2.1 S?? opublikowane, powszechnie znane lub urz??dowo podane do publicznej wiadomo??ci;");
    page.addSpace(10);
    page.drawVerse("    2.2 S?? znane danej Stronie przed przyst??pieniem do czynno??ci zwi??zanych z przygotowaniem oferty lub");
    page.drawVerse("        zosta??y uzyskane od osoby trzeciej zgodnie z prawem, bez ogranicze?? do ich ujawniania;");
    page.addSpace(10);
    page.drawVerse("    2.3 Zostan?? ujawnione przez jedn?? ze Stron za uprzednim podpisem na to zgody drugiej Stronie;");
    page.addSpace(10);
    page.drawVerse("    2.4 Zostan?? ujawnione przez jedn?? ze Stron ze wzgl??du na obowi??zuj??ce wymogi prawa lub zgodnie z");
    page.drawVerse("        prawomocnym orzeczeniem s??du lub prawomocn?? decyzj?? administracyjn?? z zastze??eniem, ??e podj??te");
    page.drawVerse("        zosta??y rozs??dne i zgodne z prawem kroki zmierzaj??ce do zachowania poufno??ci takich informacji;");
    page.addSpace(10);
    page.drawVerse("    2.5 Zosta??y uzyskane przez dan?? Stron?? niezale??nie od czynno??ci zwi??zanych z przygotowaniem oferty.");

    page.addSpace(15);
    page.setFontSize(12);
    page.setVerseOffset(20);
    page.drawVerse("    Czas trwania obowi??zku zachowania w tajemnicy informacji, o kt??rych mowa w ust. 1.1 obowi??zuj?? r??wnie??");
    page.drawVerse("po wyga??ni??ciu terminu procedury przetargowej.");

    page.addSpace(15);
    page.drawVerse("4. W przypadku naruszenia obowi??zku zachowania w tajemnicy informacji o kt??rych mowa w ust. 1.1 przez");
    page.drawVerse("jedn?? ze Stron, druga Strona b??dzie uprawniona do dochodzenia odszkodowania w pe??nej wysoko??ci i ");
    page.drawVerse("naprawienia szkody zgodnie z obowi??zuj??cymi przepisami Kodeksu cywilnego.");

    page.addSpace(15);
    page.drawVerse("5. Informacje, o kt??rych mowa w ust. 1 niniejszej Umowy mog?? stanowi?? informacje poufne w rozumieniu");
    page.drawVerse("Ustawy o obrocie instrumentami finansowymi z dnia 9 lipca 2005r. (Dz. U. Nr. 183, poz. 1538). Ujawnienie");
    page.drawVerse("ww. informacji poufnych podlega odpowiedzialno??ci karnej okre??lonej w Ustawie.");

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
    page.drawVerse("w zakresie poufno??ci informacji stanowi??cych tajemnic?? zak??adu", PageDrawer::Align::CENTER);

    page.addSpace(20);
    page.drawBoldVerse("Ja ni??ej podpisany:");

    page.addSpace(10);
    drawEmployeeData(page, employee, true);

    page.addSpace(10);
    page.drawDataNewVerseRect("Jako pracownik firmy:", company, 546, true);

    page.addSpace(30);
    page.drawBoldVerse("1. Informacje techniczne i technologiczne:");
    page.setFontSize(10);
    page.drawVerse("a) \"Know-How\" zwi??zane z procesem produkcji, technologii, ??rodkami produkcji, jako??ci produkt??w wytwarzanych w zak??adzie pracy.");
    page.drawVerse("b) Wszelkie infromacje techniczne zwi??zane z wyrobem, jego konstrukcji i zastosowanymi materia??ami.");
    page.drawVerse("c) Szczeg????owe informacje techniczne dotycz??ce stanu posiadania: wyposa??enia, narz??dzi, ??rodk??w produkcji i ??rodk??w kontroli.");
    page.drawVerse("d) Infromacje dotycz??ce lokalizacji i rodzaju pomieszcze??, zabezpiecze??(w tym zabezpiecze?? fizycznych i proceduralnych wej??cia");
    page.drawVerse("na teren organizacji oraz dost??pu do poszczeg??lnych pomieszcze?? i aktyw??w), jak i r??wnie?? wszelkich aktyw??w organizacji, kt??re");
    page.drawVerse("mog??yby w jakikolwiek spos??b zagrozi?? przedsi??biorstwu.");

    page.addSpace(30);
    page.drawBoldVerse("2. Informacje handlowe:");
    page.drawVerse("a) Stosowane ceny wyrob??w, koszty wytworzenia wyrob??w, ceny zakup??w materia????w i akcesori??w stosowanych w zak??adzie pracy.");
    page.drawVerse("b) Informacje dotycz??ce bazy dostawc??w i odbiorc??w materia????w, us??ug i innych oraz stosowanych cen.");
    page.drawVerse("c) Wszelkie informacje dotycz??ce zawartych um??w z dostawcami i odbiorcami");
    page.drawVerse("d) Informacje dotycz??ce rozm??w z kontrahentami, w szczeg??lno??ci z partnerami zagranicznymi.");

    page.addSpace(30);
    page.drawBoldVerse("3. Informacje ekonomiczno-finansowe lub inne posiadaj??ce warto???? gospodarcz??:");
    page.drawVerse("a) Informacje dotycz??cej kondycji finansowej firmy oraz ??r??de?? finansowania.");
    page.drawVerse("b) Informacje dotycz??ce organizacji sp????ki i jego stanu prawnego oraz um??w z udzia??owcami.");
    page.drawVerse("c) Informacje n/t stosowanych systemu zarz??dzania.");
    page.drawVerse("d) Informacje dotycz??ce wysoko??ci wynagrodze??, sposob??w motywacji i wynagrodzenia pracownik??w.");
    page.drawVerse("e) Dane osobowe zatrudnionego personelu, ich kwalifikacji oraz wykszta??cenia.");
    page.drawVerse("f) Informacje dotycz??ce stosowanych aplikacji informatycznych i oprogramowania.");
    page.drawVerse("g) Informacje dotycz??ce inwestycji i planowanych projekt??w.");
    page.drawVerse("h) Informacje dotycz??ce sposobu przesy??ania i szyfrowania danych.");

    page.addSpace(100);
    page.drawOneLineVerses("podpis sk??adaj??cego o??wiadczenie", "");

    page.setFontSize(12);

    progressValue += 15;
    progress->setValue(progressValue);
}
