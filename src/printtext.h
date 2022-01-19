#ifndef PRINTTEXT_H
#define PRINTTEXT_H

#include <QFont>
#include <QFontMetrics>

class PrintText
{
public:
    PrintText();

    void setText(const QString& text);
    void setFont(const QString& family, int textSize = 12);
    void setWeight(int weight);

    const QFont& getFont() const;
    const QSize& getSize() const;
    QString& operator()();

private:
    QFont font;
    QString text;
    QSize size;
};

#endif // PRINTTEXT_H
