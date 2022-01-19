#ifndef DATABASEWIDGET_H
#define DATABASEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <QFile>
#include "dbmanager.h"
#include "Data.h"

class DatabaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DatabaseWidget(QWidget *parent = nullptr);

private:
    QLabel* headerLabel;
    QLabel* statusLabel;
    QListWidget* dbListView;
    QPushButton* addDbBtn;
    QPushButton* deleteDbBtn;
    QPushButton* openDbBtn;
    QPushButton* closeDbBtn;

    QDir dbDirectory;
    QStringList filesPaths;
    bool openedDirectory;

private:
    void initializeUI();
    void addToLayout();
    void loadFromDirectory();
    void connectUI();

    void addEmployeeVariables();

private slots:
    void addDB();
    void removeDB();
    void connectToDB();
    void closeConnectionToDB();
};

#endif // DATABASEWIDGET_H
