#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "appstate.h"
#include "menuappwidget.h"
#include "cadrewidget.h"
#include "customerswidget.h"
#include "databasewidget.h"
#include "companywidget.h"
#include "cashwidget.h"
#include "dbmanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    MenuAppWidget* menuWidget;

private slots:
    void toCompanyWidget();
    void toEmployeeWidget();
    void toCustomersWidget();
    void toDatabasesInfo();
    void toCashWidget();
};
#endif // MAINWINDOW_H
