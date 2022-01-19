#ifndef DATATABLEDIALOG_H
#define DATATABLEDIALOG_H

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "Data.h"
#include "companywidget.h"
#include "printdocuments.h"

class DataTableDialog : public QDialog
{
    Q_OBJECT

public:
    enum class State{
        SICK_LEAVE,
        FREE_LEAVE,
        JOB_TIME_EVIDENCE,
        JOB_BILL_LIST
    };

    DataTableDialog(const DataTableDialog::State& state, QVector<Data::EvidenceRecord>& records, const QString& date);
    DataTableDialog(const DataTableDialog::State& state, QVector<Data::JobEvidenceRecord>& records, const QString& date);
    DataTableDialog(const DataTableDialog::State& state, QVector<Data::BillRecord>& records, const QString& date);

private:
    State state;
    QLabel* headerLabel;
    QTableWidget* table;
    QPushButton* printBtn;
    QPushButton* cancelBtn;
    QVector<Data::EvidenceRecord> records;
    QVector<Data::JobEvidenceRecord> jobEvidenceRecords;
    QVector<Data::BillRecord> billRecords;
    QString date;

private:
    void initializeUI();
    void addToLayout();
    void connectUI();

    void initializeLeaveTable();
    void initializeJobTimeEvidenceTable();
    void initializeJobBillTable();

private slots:
    void printSickEvidence();
    void printFreeEvidence();
    void printJobTimeEvidence();
    void printJobListBill();
};

#endif // DATATABLEDIALOG_H
