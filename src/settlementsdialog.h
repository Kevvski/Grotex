#ifndef SETTLEMENTSDIALOG_H
#define SETTLEMENTSDIALOG_H

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QSqlQuery>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include "Data.h"

class SettlementsDialog : public QDialog
{
    Q_OBJECT

public:
    enum class State{
        RECEIVABLES,
        COMMITMENT
    };

    SettlementsDialog(const SettlementsDialog::State& state);

private:
    SettlementsDialog::State state;
    QLabel* header;
    QTableWidget* table;
    QPushButton* cancelBtn;

private:
    void initializeUI();
    void addToLayout();
    void connectUI();

private:
    void initializeReceivables();
    void initializeCommitment();
};

#endif // SETTLEMENTSDIALOG_H
