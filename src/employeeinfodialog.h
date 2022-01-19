#ifndef EMPLOYEEINFODIALOG_H
#define EMPLOYEEINFODIALOG_H

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QSqlQuery>
#include <QScrollArea>
#include <QMessageBox>
#include "employeedatawidget.h"
#include "Data.h"
#include "printdocuments.h"

class EmployeeInfoDialog : public QDialog
{
    Q_OBJECT

public:
    EmployeeInfoDialog(const QString& id);

private:
    QString employeeId;
    QScrollArea* scroll;
    EmployeeDataWidget* empWidget;
    QLabel* infoHeader;
    QLabel* jobHeader;
    QLabel* freeHeader;
    QLabel* sickHeader;
    QLabel* freeDaysCountLabel;
    QLabel* freeSumLabel;
    QLabel* sickSumLabel;
    QTableWidget* jobStoryTable;
    QTableWidget* freeStoryTable;
    QTableWidget* sickStoryTable;
    QPushButton* printBtn;
    QPushButton* cancelBtn;

private:
    void initializeUI();
    void addToLayout();
    void connectUI();

private slots:
    void printInfo();
};

#endif // EMPLOYEEINFODIALOG_H
