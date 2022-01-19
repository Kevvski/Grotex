#ifndef COMPANYWIDGET_H
#define COMPANYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVector>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "dbmanager.h"
#include "Data.h"
#include "companydialog.h"

class CompanyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompanyWidget(QWidget *parent = nullptr);
    static void loadCompanyData();

    static const bool isExists();

private:
    static bool dataExists;
    QVector<QLabel*> labels;
    QPushButton* changeDataBtn;

private:
    void initializeUI();
    void addToLayout();
    void connectUI();

private slots:
    int changeCompanyData();

};

#endif // COMPANYWIDGET_H
