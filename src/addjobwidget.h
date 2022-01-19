#ifndef ADDJOBWIDGET_H
#define ADDJOBWIDGET_H

#include <QWidget>
#include <QVector>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QDate>
#include <QDateEdit>
#include <QCheckBox>
#include <QListWidget>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QComboBox>
#include "Data.h"
#include "companywidget.h"
#include "appstate.h"

class AddJobWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AddJobWidget(const AppState::ContractState& state, const QString& employeeID = "", const QString& docID = "", bool changeData = false, QWidget *parent = nullptr);

    const QString getLineEditText(int index) const;
    const QString getJobsText() const;
    const QString getEntrustsText() const;
    const QDateEdit* getDateEdit();
    const QComboBox* getMaxDays() const;
    const bool hasDecision() const;
    const bool isEmptyLineEdits() const;
    const bool isEmptyLists() const;

private:
    AppState::ContractState contractState;
    QVector<QLabel*> labels;
    QVector<QLineEdit*> lineEdits;
    QDateEdit* dateEdit;
    QCheckBox* decision;
    QPushButton* addJobBtn;
    QPushButton* addEntrustsBtn;
    QListWidget* jobsList;
    QListWidget* entrustsList;
    QComboBox* maxDaysBox;

private:
    void initializeJobUI(const QString& employeeID, const QString& docID, bool changeData);
    void initializeOrderUI(const QString& employeeID, const QString& docID, bool changeData);
    void initializeWorkUI(const QString& employeeID, const QString& docID, bool changeData);
    void addToLayout();
    void connectUI();

private slots:
    void addToJobList();
    void addToEntrustsList();
};

#endif // ADDJOBWIDGET_H
