#ifndef CASHWIDGET_H
#define CASHWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QTableWidget>
#include <QSqlQuery>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include "Data.h"

class CashWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CashWidget(QWidget *parent = nullptr);

private:
    bool change;
    float recordSum;
    QLabel* balanceHeaderLabel;
    QLabel* balanceLabel;
    QLabel* dateLabel;
    QLabel* aboutLabel;
    QLabel* costLabel;
    QLabel* compensationLabel;
    QLabel* listLabel;
    QLabel* lastAddLabel;
    QLabel* incomeLabel;
    QLabel* incomeValueLabel;
    QLabel* leaveLabel;
    QLabel* leaveValueLabel;
    QDateEdit* dateEdit;
    QLineEdit* aboutEdit;
    QLineEdit* costEdit;
    QCheckBox* compensationCheckBox;
    QTableWidget* table;
    QPushButton* addRecordBtn;
    QPushButton* incomeBtn;
    QPushButton* leaveBtn;
    QPushButton* incomeListBtn;
    QPushButton* leaveListBtn;

private:
    void initializeUI();
    void addToLayout();
    void connectUI();
    void addRecord();
    void addIncome();
    void addLeave();

private slots:
    void addCash();
};

#endif // CASHWIDGET_H
