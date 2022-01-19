
#ifndef PRINTDOCUMENTS_H
#define PRINTDOCUMENTS_H

#include <QSqlQuery>
#include <QSqlRecord>
#include <QVector>
#include <QChar>
#include <QProgressDialog>
#include <QDate>
#include "pagedrawer.h"
#include "dbmanager.h"
#include "financedialog.h"
#include "Data.h"

class PrintDocuments
{
public:
    PrintDocuments();

    void setEmployeeID(const QString& id);
    void setDocumentID(const QString& id);
    void setOrderBillID(const QString& id);
    void loadDocuments();

    void printJobContract();
    void printOrderContract();
    void printWorkContract();
    void printSickLeaveEvidence(const QString& date, QVector<Data::EvidenceRecord>& records);
    void printFreeLeaveEvidence(const QString& date, QVector<Data::EvidenceRecord>& records);
    void printJobTimeEvidence(const QString& date, const Data::JobEvidenceRecord& record);
    void printAnnex();
    void printOrderBill(int hoursSum, int daysSum, int freeDaysSum, int sickDaysSum);
    void printJobListBill(QVector<Data::BillRecord>& records);
    void printJobBill(QVector<Data::BillRecord>& records);
    void printPayCertificate(QVector<Data::BillRecord>& records);
    void printHouseCertificate(QVector<Data::BillRecord>& records);
    void printFamilyOrMaintanceCertificate(const Data::BillRecord& record, int index);
    void printEmployeeInfo(QVector<Data::EmployeeInfoRecord>& jobRecords, QVector<Data::EmployeeInfoRecord>& freeRecords, QVector<Data::EmployeeInfoRecord>& sickRecords, QString& freeDays);
    void printSellDoc(const FinanceDialog::State& state);
    void printCompensationDoc();

private:
    QString employeeID;
    QString docId;
    QString orderBillId;
    bool decision;
    QVector<QString> employeeVariables;
    QVector<QString> jobContractVariables;
    QVector<QString> orderContractVariables;
    QVector<QString> workContractVariables;
    QVector<QString> companyVariables;
    QVector<QString> jobTimeVariables;
    QVector<QString> annexVariables;
    QVector<QString> orderBillVariables;
    QVector<QString> financeVariables;
    QVector<QString> compensationVariables;
    QProgressDialog* progress;
    int progressValue;

private:
    void drawEmployeeData(PageDrawer& page, QVector<QString>& employee, bool currentData = false);
    void drawStartProvisionsPage(PageDrawer& page, const QString& currentDate);
    void drawEndProvisionsPage(PageDrawer& page, const QString& currentDate);
    void drawBHPPage(PageDrawer& page, const QString& currentDate);
    void drawBHPDeclarationPage(PageDrawer& page, QVector<QString>& employee, const QString& currentDate, const QString& date, const QString& company);
    void drawHopePage(PageDrawer& page, const QString& currentDate);
    void drawHopeDeclarationPage(PageDrawer& page, QVector<QString>& employee, const QString& currentDate, const QString& company);
};

#endif // PRINTDOCUMENTS_H
