#ifndef DATA_H
#define DATA_H

#include <QString>
#include <QSize>
#include    <QDate>

namespace Data {
    const QString defaultButtonSheet = "QPushButton{"
                   "color: white;"
                   "font-size: 15px;"
                   "font-weight: bold;"
                   "background-color: #1c45a6;"
                   "border: 1px solid #1c45a6;"
                   "border-radius: 20px;"
                   "}"
                   "QPushButton:hover:!pressed{"
                   "background-color: #2667ff;"
                   "}";

    const QString boldLabel = "QLabel{ color: #1c45a6; font-size: 15px; font-weight: bold; }";

    const QString boldLabelGreater = "QLabel{ color: #1c45a6; font-size: 25px; font-weight: bold; }";

    const QString defaultLineEdit = "QLineEdit{ font-size: 15px; border: 2px solid #1c45a6; border-radius: 8px; padding: 5px; }";

    const QString defaultDateEdit = "QDateEdit{ font-size: 15px; border: 2px solid #1c45a6; border-radius: 8px; padding: 5px; }";

    const QSize defaultButtonSize {100, 40};

    struct EvidenceRecord{
        int id;
        int employeeId;
        int daysCount;
        QString name;
        QString surname;
        QString pesel;
        QString type;
        QDate startDate;
        QDate endDate;
    };

    struct JobEvidenceRecord{
        int id;
        int employeeId;
        int sum;
        int daysSum;
        QString name;
        QString surname;
        QString month;
        QString year;
        QString days;
        QString hours;
        QString type;
        int employeeHours;
    };

    struct BillRecord{
        int id;
        QString docId;
        float rewardBrutto;
        float basicBrutto;
        float freeBrutto;
        float sickBrutto;
        float iOld;
        float iRent;
        float iDisease;
        float iSum;
        float iOldP;
        float iRentP;
        float iAccident;
        float iSumP;
        float incomeCost;
        float incomeCostAfter;
        float rate;
        float insurance;
        float insuranceAdd;
        float insuranceSub;
        float jobFund;
        float fgsp;
        float rewardNetto;
        QString payType;
        QDate date;
        QString employeeName;
    };

    struct EmployeeInfoRecord{
        QString type;
        QString startDate;
        QString endDate;
        QString daysCount;
        QString jobType;
    };
}

#endif // DATA_H
