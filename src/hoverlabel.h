#ifndef HOVERLABEL_H
#define HOVERLABEL_H

#include <QLabel>
#include <QString>

class HoverLabel : public QLabel
{
    Q_OBJECT

public:
    HoverLabel(QWidget* parent = nullptr);

    void setEnterStyle(const QString& text);
    void setLeaveStyle(const QString& text);

protected:
    virtual void enterEvent(QEvent* ev) override;
    virtual void leaveEvent(QEvent* ev) override;

private:
    QString enterStyleSheet;
    QString leaveStyleSheet;
};

#endif // HOVERLABEL_H
