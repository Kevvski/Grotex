#ifndef COMPANYDIALOG_H
#define COMPANYDIALOG_H

#include <QDialog>
#include <QVector>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "dbmanager.h"
#include "Data.h"
#include "appstate.h"

class CompanyDialog : public QDialog
{
    Q_OBJECT
public:
    CompanyDialog(bool dataExists = false, const QString& id = "");

private:
    QString companyId;
    bool dataExists;
    QVector<QLabel*> labels;
    QVector<QLineEdit*> lineEdits;
    QPushButton* saveBtn;
    QPushButton* cancelBtn;

private:
    void initializeUI();
    void addToLayout();
    void connectUI();

private slots:
    void saveCompanyData();
    void saveCompaniesData();
};

#endif // COMPANYDIALOG_H
