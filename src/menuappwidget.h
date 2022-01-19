#ifndef MENUAPPWIDGET_H
#define MENUAPPWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

class MenuAppWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MenuAppWidget(QWidget *parent = nullptr);

    QPushButton* getCompanyBtn();
    QPushButton* getEmplyeesBtn();
    QPushButton* getCustomersBtn();
    QPushButton* getDatabasesBtn();
    QPushButton* getCashBtn();

private:
    QLabel* logoLabel;
    QPushButton* companyButton;
    QPushButton* employeesButton;
    QPushButton* customersButton;
    QPushButton* databasesButton;
    QPushButton* cashButton;
};

#endif // MENUAPPWIDGET_H
