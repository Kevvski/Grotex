#ifndef ADDJOBDIALOG_H
#define ADDJOBDIALOG_H

#include <QObject>
#include <QDialog>
#include <QVector>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDate>
#include "appstate.h"
#include "employeedatawidget.h"
#include "addjobwidget.h"
#include "Data.h"

class AddJobDialog : public QDialog
{
    Q_OBJECT

public:
    AddJobDialog(const QString& employeeID, const QString& docId, const AppState::ContractState& state, bool changeData = false);

private:
    AppState::ContractState contractState;
    QString employeeID;
    QString docID;
    EmployeeDataWidget* empDataWidget;
    AddJobWidget* addJobWidget;

    QVector<QLabel*> labels;
    QPushButton* saveBtn;
    QPushButton* cancelBtn;
    QPushButton* printBtn;

private:
    void initializeUI(bool changeData);
    void addToLayout();
    void connectUI(bool changeData);

private slots:
    void saveJobContract();
    void saveOrderContract();
    void saveWorkContract();

    void editJobContract();
    void editOrderContract();
    void editWorkContract();
};

#endif // ADDJOBDIALOG_H
