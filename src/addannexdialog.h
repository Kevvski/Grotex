#ifndef ADDANNEXDIALOG_H
#define ADDANNEXDIALOG_H

#include <QObject>
#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QSqlQuery>
#include <QListWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDate>
#include "Data.h"
#include "employeedatawidget.h"
#include "appstate.h"

class AddAnnexDialog : public QDialog
{
    Q_OBJECT

public:
    AddAnnexDialog(const AppState::ContractState& state, const QString& docId, const QString& empId, const QString& textId, bool changeData = false);

private:
    bool changeData;
    AppState::ContractState state;
    QString docId;
    QString employeeId;
    EmployeeDataWidget* empDataWidget;
    QLabel* changeLabel;
    QLabel* fromLabel;
    QLabel* onLabel;
    QComboBox* changeBox;
    QListWidget* changeList;
    QLineEdit* fromEdit;
    QLineEdit* onEdit;
    QPushButton* onBtn;
    QPushButton* addBtn;
    QPushButton* cancelBtn;

private:
    void initializeUI();
    void addToLayout();
    void connectUI();
    const QString getChangesAsString() const;

private slots:
    void addToList();
    void addAnnex();
    void changeAnnex();
};

#endif // ADDANNEXDIALOG_H
