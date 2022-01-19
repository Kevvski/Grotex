#ifndef CHECKCONTRACTSDIALOG_H
#define CHECKCONTRACTSDIALOG_H

#include <QObject>
#include <QDialog>
#include <QVector>
#include <QStringList>
#include <QLabel>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVBoxLayout>
#include "Data.h"

class CheckContractsDialog : public QDialog
{
    Q_OBJECT

public:
    CheckContractsDialog();

    void checkContracts();

private:
    QLabel* headerLabel;
    QVector<QLabel*> labels;
    QPushButton* okBtn;

private:
    struct Contract{
        int id;
        int employeeId;
        QString textId;
        QDate endDate;
    };

    void initializeUI();
    void addToLayout();
    void connectUI();
    bool searchContracts(const QString& tableName);
};

#endif // CHECKCONTRACTSDIALOG_H
