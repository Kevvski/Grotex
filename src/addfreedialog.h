#ifndef ADDFREEDIALOG_H
#define ADDFREEDIALOG_H

#include <QObject>
#include <QDialog>
#include <QPushButton>
#include <QMessageBox>
#include "employeedatawidget.h"
#include "addfreewidget.h"

class AddFreeDialog : public QDialog
{
    Q_OBJECT

public:
    AddFreeDialog(const AddFreeWidget::State& state, const QString& employeeID, const QString& sickId = "", bool changeData = false);

private:
    AddFreeWidget::State state;
    QString employeeID;
    QString sickId;
    EmployeeDataWidget* empDataWidget;
    AddFreeWidget* freeDataWidget;
    QPushButton* saveBtn;
    QPushButton* cancelBtn;

private:
    void initializeUI(bool changeData);
    void addTolayout();
    void connectUI(bool changeData);

private slots:
    void addSickLeave();
    void addFreeLeave();

    void changeSickLeave();
    void changeFreeLeave();
};

#endif // ADDFREEDIALOG_H
