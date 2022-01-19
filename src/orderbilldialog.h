#ifndef ORDERBILLDIALOG_H
#define ORDERBILLDIALOG_H

#include <QObject>
#include <QDialog>
#include <QSqlQuery>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QMessageBox>
#include "Data.h"

class OrderBillDialog : public QDialog
{
    Q_OBJECT

public:
    enum class State{
        ORDER_BILL,
        WORK_BILL,
        JOB_BILL
    };

    OrderBillDialog(const QString& docId, const QString& evId, const OrderBillDialog::State& state, bool changeData = false);

private:
    State state;
    bool changeData;
    QString docId;
    QString evidenceId;
    QVector<QLabel*> labels;
    QVector<QCheckBox*> checks;
    QComboBox* payBox;
    QComboBox* rateBox;
    QComboBox* schemaBox;
    QLineEdit* accidentEdit;
    QPushButton* addBtn;
    QPushButton* cancelBtn;
    QVector<float> prices;

private:
    void initializeUI();
    void addToLayout();
    void connectUI();
    void calculateBill(float brutto);

private slots:
    void changeDealRate(bool value);
    void changeZeroPit(bool value);
    void setSchemat(int index);
    void saveOrderBill();
    void updateOrderBill();
};

#endif // ORDERBILLDIALOG_H
