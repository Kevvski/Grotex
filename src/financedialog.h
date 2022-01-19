#ifndef FINANCEDIALOG_H
#define FINANCEDIALOG_H

#include <QObject>
#include <QDialog>
#include <QVector>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSqlQuery>
#include <QTableWidget>
#include <QDateEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include "Data.h"

class FinanceDialog : public QDialog
{
    Q_OBJECT

public:
    enum class State{
        WZ,
        PZ,
        FS,
        FZ
    };

    FinanceDialog(const State& state, const QString& id, bool change = false, bool base = false);

private:
    State state;
    QString companyId;
    bool changeData;
    bool isBaseDoc;
    QVector<QLabel*> labels;
    QVector<QLineEdit*> lineEdits;
    QDateEdit* dateEdit;
    QDateEdit* payEdit;
    QComboBox* payTypeBox;
    QCheckBox* statusCheck;
    QTableWidget* table;
    QPushButton* addRecordBtn;
    QPushButton* saveBtn;
    QPushButton* cancelBtn;

private:
    void initializeUI();
    void addToLayout();
    void connectUI();
    QString getColumnAsString(int index);
    float getColumnSum(int index);

private slots:
    void addRecord();
    void addDoc();
    void changeDoc();
};

#endif // FINANCEDIALOG_H
