#ifndef CUSTOMERSWIDGET_H
#define CUSTOMERSWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include "Data.h"
#include "dbmanager.h"
#include "doclistdialog.h"
#include "companydialog.h"
#include "settlementsdialog.h"


class CustomersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomersWidget(QWidget *parent = nullptr);

private:
    QPushButton* addCompanyBtn;
    QPushButton* addWZBtn;
    QPushButton* addPZBtn;
    QPushButton* addSellBtn;
    QPushButton* addBuyBtn;
    QPushButton* addCompensationBtn;
    QPushButton* receivablesBtn;
    QPushButton* commitmentBtn;
    QPushButton* countVATBtn;
    QPushButton* countZUSBtn;
    QPushButton* countPIT4Btn;
    QPushButton* docListBtn;

private:
    void initializeUI();
    void addToLayout();
    void connectUI();

private slots:
    int addCompany();
    int addFZ();
    int addFS();
    int addWZ();
    int addPZ();
    int addCompensation();
    int openReceivables();
    int openCommitment();
    int openDocList();
};

#endif // CUSTOMERSWIDGET_H
