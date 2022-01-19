#include "dbmanager.h"

bool DBManager::connected = false;
QString DBManager::dbName = "";
QString DBManager::showingName = "";
QSqlDatabase DBManager::db;
QVector<QString> DBManager::employeeVariables;
QVector<QString> DBManager::jobContractVariables;

DBManager::DBManager()
{
}


void DBManager::setDBName(const QString &text, const QString& showText){
    dbName = text;
    showingName = showText;
}


void DBManager::addEmployeeVariables(const QStringList &variableList){
    for(auto& variable : variableList){
        employeeVariables.push_back(variable);
    }
}


void DBManager::addJobContractVariables(const QStringList &variableList){
    for(auto& variable : variableList){
        jobContractVariables.push_back(variable);
    }
}


void DBManager::closeDB(){
    connected = false;
    db.close();
    employeeVariables.clear();
    jobContractVariables.clear();
}


bool DBManager::connectToDB(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    if(!db.open()){
        connected = false;
        return false;
    }

    CheckContractsDialog* dialog = new CheckContractsDialog;
    dialog->checkContracts();
    connected = true;
    return true;
}


bool DBManager::isConnected(){
    return connected;
}


const QString& DBManager::getDBName(){
    return showingName;
}


const QString DBManager::getEmployeeVariable(int index){
    if(index >= 0 && index < employeeVariables.size())
        return employeeVariables.at(index) + ":";
}


const QString DBManager::getJobContractVariable(int index){
    if(index >= 0 && index < jobContractVariables.size())
        return jobContractVariables.at(index) + ":";
}

