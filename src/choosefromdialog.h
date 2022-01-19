#ifndef CHOOSEFROMDIALOG_H
#define CHOOSEFROMDIALOG_H

#include <QObject>
#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include "Data.h"
#include "dbviewdialog.h"

class ChooseFromDialog : public QDialog
{
    Q_OBJECT

public:
    ChooseFromDialog();

private:
    QComboBox* chooseBox;
    QPushButton* chooseBtn;
    QPushButton* cancelBtn;

private:
    void initializeUI();
    void addToLayout();
    void connectUI();

private slots:
    int chooseContract();
};

#endif // CHOOSEFROMDIALOG_H
