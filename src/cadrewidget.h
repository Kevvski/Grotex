#ifndef CADREWIDGET_H
#define CADREWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFont>
#include <QMessageBox>
#include <QDir>
#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include "adduserdialog.h"
#include "doclistdialog.h"
#include "dbviewdialog.h"
#include "imagebutton.h"
#include "choosefromdialog.h"

class CadreWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CadreWidget(QWidget *parent = nullptr);

private:
    AppState::ContractState contractState;
    ImageButton* addEmployeeBtn;
    ImageButton* addJobBtn;
    ImageButton* addOrderBtn;
    ImageButton* addWorkBtn;
    ImageButton* jobTimeBtn;
    ImageButton* addLeaveBtn;
    ImageButton* addVacationBtn;
    ImageButton* createAnnexBtn;
    ImageButton* countOrderBtn;
    ImageButton* countWorkBtn;
    ImageButton* createPaymentListBtn;
    ImageButton* employeesListBtn;

private:
    void initializeUI();
    void addToLayout();
    void connectUI();

private slots:
    int addUser();
    int addJobContract();
    int addOrderContract();
    int addWorkContract();
    int addSickLeave();
    int addFreeLeave();
    int createAnnex();
    int addJobTimeEvidence();
    int countOrderContract();
    int countWorkContract();
    int countJobContract();
    int showEmployeesList();
};

#endif // CADREWIDGET_H
