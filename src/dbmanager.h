#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVector>
#include <QDebug>
#include "checkcontractsdialog.h"

class DBManager
{
public:
    DBManager();

    static void setDBName(const QString& text, const QString& showText);
    static void addEmployeeVariables(const QStringList& variableList);
    static void addJobContractVariables(const QStringList& variableList);
    static void closeDB();

    static bool connectToDB();
    static bool isConnected();
    static const QString& getDBName();
    static const QString getEmployeeVariable(int index);
    static const QString getJobContractVariable(int index);

private:
    static bool connected;
    static QString dbName;
    static QString showingName;
    static QSqlDatabase db;
    static QVector<QString> employeeVariables;
    static QVector<QString> jobContractVariables;
};

#endif // DBMANAGER_H
