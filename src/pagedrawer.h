#ifndef PAGEDRAWER_H
#define PAGEDRAWER_H

#include <QPrinter>
#include <QPainter>
#include <QPoint>
#include <QDebug>
#include "printtext.h"

class PageDrawer
{
public:
    PageDrawer();

    enum class Align{
        CENTER,
        LEFT
    };

    void setSaveFileName(const QString& fileName);
    void setStartPoint(int x, int y);
    void setVerseOffset(int offset);
    void setTableOffset(int offset);
    void setFontSize(int size);
    void setColumnsCount(int count);
    void setOrientation(const QPageLayout::Orientation& orientation);
    void addSpace(int value);
    void toNextPage();
    void resetCells();
    void pageStart();
    void pageEnd();

    void drawBegin(const QString& text);
    void drawHeader(const QString& text, const Align& align = Align::LEFT);
    void drawVerse(const QString& text, const Align& align = Align::LEFT);
    void drawInfo(const QString& text, const Align& align = Align::LEFT, int move = 0);
    void drawBoldVerse(const QString& text, const Align& align = Align::LEFT);
    void drawOneLineVerses(const QString& firstText, const QString& secondText);
    void drawDataVerse(const QString& labelText, const QString& dataText);
    void drawDataRect(const QString& labelText, const QString& dataText, int width = 20, int move = 0);
    void drawDataNewVerseRect(const QString& labelText, const QString& dataText, int width = 20, bool move = false);
    void drawDataSameVerseRect(const QString& labelText, const QString& dataText, int width = 20, bool move = false);
    void drawCell(const QString& text, int width = 20, int height = 0, bool fillRect = false);
    void drawFooter(const QString& text);

    float getColumnsCount() const;
    const QPoint& getPenPosition() const;

private:
    QPrinter printer;
    QPainter painter;
    PrintText header;
    PrintText label;
    PrintText data;
    PrintText info;
    QPoint nextRow;
    int offset;
    int tableOffset;
    int columnsCount;
    int currentCell;
};

#endif // PAGEDRAWER_H
