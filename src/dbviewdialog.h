#ifndef DBVIEWDIALOG_H
#define DBVIEWDIALOG_H

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPrinter>
#include <QPainter>
#include "appstate.h"
#include "adduserdialog.h"
#include "addjobdialog.h"
#include "printtext.h"
#include "pagedrawer.h"
#include "companywidget.h"
#include "printdocuments.h"
#include "addfreedialog.h"
#include "searchsickdialog.h"
#include "addannexdialog.h"
#include "orderbilldialog.h"
#include "employeeinfodialog.h"
#include "companydialog.h"
#include "financedialog.h"

class DBViewDialog : public QDialog
{
    Q_OBJECT
public:
    enum class State{
        ADD_CONTRACT,
        ADD_SICKLEAVE,
        ADD_FREELEAVE,
        ADD_JOB_TIME_EVIDENCE,
        EMPLOYEE_LIST,
        JOB_CONTRACTS_LIST,
        ORDER_CONTRACTS_LIST,
        WORK_CONTRACTS_LIST,
        SICK_LEAVE_LIST,
        FREE_LEAVE_LIST,
        JOB_TIME_LIST,
        CHOOSE_TO_ANNEX,
        ANNEX_LIST,
        COUNT_ORDER_CONTRACT,
        BILLS_LIST,
        COUNT_WORK_CONTRACT,
        COUNT_JOB_CONTRACT,
        PAY_CERTIFICATE,
        PAY_CERTIFICATE_2, //Zaświadczenie o zatrudnieniu
        HOUSE_CERTIFICATE,
        FAMILY_CERTIFICATE,
        MEINTENANCE_CERTIFICATE,
        ADD_BUY_DOC,
        COMPENSATION_LIST,
        COMPANIES_LIST,
        FINANCE_DOC_LIST,
    };

    DBViewDialog(const State& state, const AppState::ContractState& contractState = AppState::ContractState::NONE);
    DBViewDialog(const State& state, const QString& id, int index = 0);
    DBViewDialog(const State& state, const FinanceDialog::State& financeState);

private:
    State state;
    AppState::ContractState contractState;
    FinanceDialog::State financeState;
    QString empId;
    int certificateIndex;

    QLabel* headerLabel;
    QTableView* view;
    QSqlTableModel* model;
    QPushButton* addBtn;
    QPushButton* editBtn;
    QPushButton* deleteBtn;
    QPushButton* cancelBtn;
    QPushButton* printBtn;
    QPushButton* printLPBtn;

private:
    void initializeUI(const State& state);
    void initializeEmployeeUI();
    void initializeCompaniesUI();

    void initializeEmployeeTable();
    void initializeJobTable();
    void initializeOrderTable();
    void initializeWorkTable();
    void initializeSickLeaveTable();
    void initializeFreeLeaveTable();
    void initializeJobTimeTable(const OrderBillDialog::State& state = OrderBillDialog::State::WORK_BILL);
    void initializeAnnexTable();
    void initializeCountOrderTable();
    void initializeCompaniesTable();
    void initializeFinanceDocsTable();
    void initializeAccountTable();

    void connectUIEmployee();
    void connectUICompanies();

    void addToLayout();
    void checkEmployeeIsWorking(const QString& id);

private slots:
    int addContract();
    int addSickLeave();
    int addFreeLeave();
    int addBuyDoc();
    int chooseBaseDoc();

    int changeEmployeeData();
    int changeJobContractData();
    int changeOrderContractData();
    int changeWorkContractData();
    int changeSickLeaveData();
    int changeFreeLeaveData();
    int changeJobTimeData();
    int changeAnnexData();
    int changeOrderBillData();
    int changeCompaniesData();
    int changeFinanceDocsData();

    void deleteEmployeeData();
    void deleteJobContractData();
    void deleteOrderContractData();
    void deleteWorkContractData();
    void deleteSickLeaveData();
    void deleteFreeLeaveData();
    void deleteJobTimeData();
    void deleteAnnexData();
    void deleteOrderBillData();
    void deleteCompaniesData();
    void deleteFinanceDocsData();

    void printJobContract();
    void printOrderContract();
    void printWorkContract();
    void printAnnex();
    void printOrderBill();
    void printPayCertificate();
    void printFinanceDoc();
    void printCompensationDoc();
    void resetAccount();

    int createSickEvidence();
    int createFreeEvidence();
    int createJobTimeEvidence();
    int searchJobTimeEvidences();
    int createAnnex();
    int createOrderBill();
    int createWorkBill();
    int createJobBill();
    int createJobBillList();
    int createPayCertificate();
    int openEmployeeStory();
};

#endif // DBVIEWDIALOG_H
