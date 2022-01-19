#ifndef EMPLOYEEDATAWIDGET_H
#define EMPLOYEEDATAWIDGET_H

#include <QWidget>
#include <QSqlQuery>
#include <QVector>
#include <QLabel>
#include <QGridLayout>
#include "dbmanager.h"
#include "Data.h"

class EmployeeDataWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EmployeeDataWidget(const QString& employeeID, bool workContract = false, QWidget *parent = nullptr);

private:
    QVector<QLabel*> labels;

private:
    void initializeUI(const QString& employeeID);
    void addToLayout(bool workContract);

};

#endif // EMPLOYEEDATAWIDGET_H
