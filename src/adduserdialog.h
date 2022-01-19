#ifndef ADDUSERDIALOG_H
#define ADDUSERDIALOG_H

#include <QObject>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QVector>
#include <QDebug>
#include "dbmanager.h"
#include "Data.h"

class AddUserDialog : public QDialog
{
    Q_OBJECT
public:
    AddUserDialog();
    AddUserDialog(QString id);

private:
    enum class State{
        NEW_EMPLOYEE,
        UPDATE_EMPLOYEE
    };

    State state;
    QVector<QLabel*> labels;
    QVector<QLineEdit*> lineEdits;
    QPushButton* addBtn;
    QPushButton* cancelBtn;

private:
    void initializeUI();
    void addToLayout();
    void connectUI();

private slots:
    void addEmployee();
    void updateEmployee();
};

#endif // ADDUSERDIALOG_H
