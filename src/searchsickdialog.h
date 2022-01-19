#ifndef SEARCHSICKDIALOG_H
#define SEARCHSICKDIALOG_H

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlQuery>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDate>
#include <QMessageBox>
#include <QDebug>
#include <QVector>
#include <QTableWidget>
#include <QHeaderView>
#include "Data.h"
#include "datatabledialog.h"
#include "employeedatawidget.h"

class SearchSickDialog : public QDialog
{
    Q_OBJECT

public:
    enum class State{
        SICK_LEAVE,
        FREE_LEAVE,
        JOB_TIME_EVIDENCE,
        SEARCH_JOB_TIME,
        CREATE_JOB_BILL_LIST
    };

    SearchSickDialog(const SearchSickDialog::State& state, const QString& id = "", const QString& docId = "", bool changeData = false);

private:
    bool changeData;
    QString employeeID;
    QString id;
    EmployeeDataWidget* empWidget;
    QLabel* monthLabel;
    QLabel* yearLabel;
    QLabel* typeLabel;
    QComboBox* monthBox;
    QComboBox* typeBox;
    QLineEdit* yearEdit;
    QTableWidget* table;
    QPushButton* searchBtn;
    QPushButton* cancelBtn;

private:
    struct EVRecord{
        int id;
        QDate date;
    };

    void initializeUI(const SearchSickDialog::State& state);
    void addToLayout(const SearchSickDialog::State& state);
    void connectUI(const SearchSickDialog::State& state);
    const QString getDaysAsString() const;
    const QString getHourCountAsString() const;
    const int getHoursSum(int hours) const;
    const int getDaysSum() const;
    const int getFreeDaysSum() const;
    const int getSickDaysSum() const;

private slots:
    int searchSickLeave();
    int searchFreeLeave();
    int searchJobTime();
    int searchJobBills();
    void createJobTimeEvidence();
    void saveJobTimeEvidence();
    void refreshTable(int index);
};

#endif // SEARCHSICKDIALOG_H
