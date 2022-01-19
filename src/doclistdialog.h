#ifndef DOCLISTDIALOG_H
#define DOCLISTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include "Data.h"
#include "dbviewdialog.h"
#include "appstate.h"

class DocListDialog : public QDialog
{
    Q_OBJECT

public:
    DocListDialog();

private:
    QLabel* headerLabel;
    QComboBox* docsBox;
    QPushButton* openBtn;
    QPushButton* cancelBtn;

private:
    void initializeUI();
    void addToLayout();
    void connectUI();

private slots:
    int openListEmployees();
    int openListCustomers();
};

#endif // DOCLISTDIALOG_H
