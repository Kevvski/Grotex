#ifndef COMPENSATIONDIALOG_H
#define COMPENSATIONDIALOG_H

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "dbviewdialog.h"
#include "Data.h"

class CompensationDialog : public QDialog
{
    Q_OBJECT

public:
    CompensationDialog();

private:
    QLabel* label;
    QLabel* chargeLabel;
    QLabel* compensationLabel;
    QCheckBox* chargeCheckBox;
    QCheckBox* commitmentCheckBox;
    QPushButton* chooseBtn;
    QPushButton* cancelBtn;

private:
    void initializeUI();
    void addToLayout();
    void connectUI();

private slots:
    int chooseDoc();
};

#endif // COMPENSATIONDIALOG_H
