#ifndef ADDFREEWIDGET_H
#define ADDFREEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QDateEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QSqlQuery>
#include "Data.h"

class AddFreeWidget : public QWidget
{
    Q_OBJECT

public:
    enum class State{
        SICK_LEAVE,
        FREE_LEAVE
    };

    AddFreeWidget(const State& state, const QString& id, bool changeData = false);

    const QDateEdit* getStartDate() const;
    const QDateEdit* getEndDate() const;
    const QComboBox* getFreeComboBox() const;

private:
    State state;
    QLabel* startDateLabel;
    QLabel* endDateLabel;
    QLabel* freeTypeLabel;
    QDateEdit* startDateEdit;
    QDateEdit* endDateEdit;
    QComboBox* freeTypeBox;
    //Type of free days

private:
    void initializeUI(const QString& id, bool changeData);
    void addToLayout();
};

#endif // ADDFREEWIDGET_H
