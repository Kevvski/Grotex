#include "pagedrawer.h"

PageDrawer::PageDrawer() : currentCell{0}, tableOffset{0}
{

}


void PageDrawer::setSaveFileName(const QString &fileName){
    printer.setOutputFileName(fileName);
}


void PageDrawer::setStartPoint(int x, int y){
    nextRow.setX(x);
    nextRow.setY(y);
}


void PageDrawer::setVerseOffset(int offset){
    this->offset = offset;
}


void PageDrawer::setTableOffset(int offset){
    tableOffset = offset;
}


void PageDrawer::setFontSize(int size){
    data.setFont("Times New Roman", size);
}


void PageDrawer::setColumnsCount(int count){
    columnsCount = count;
}


void PageDrawer::setOrientation(const QPageLayout::Orientation &orientation){
    printer.setPageOrientation(orientation);
}


void PageDrawer::addSpace(int value){
    nextRow.setY(nextRow.y() + value);
}


void PageDrawer::toNextPage(){
    if(!printer.newPage())
        qDebug() << "Not";
}


void PageDrawer::resetCells(){
    currentCell = 0;
}


void PageDrawer::pageStart(){
    const QString fontFamily = "Times New Roman";

    painter.begin(&printer);
    header.setFont(fontFamily, 20);
    header.setWeight(QFont::Bold);

    label.setFont(fontFamily);
    label.setWeight(QFont::Bold);

    data.setFont(fontFamily);

    info.setFont(fontFamily, 6);
    info.setText("a");
}


void PageDrawer::pageEnd(){
    painter.end();
}


void PageDrawer::drawBegin(const QString& text){
    data.setText("Pieczęć Firmy");
    painter.setFont(data.getFont());
    painter.drawText(0, data.getSize().height(), data());

    data.setText(text);
    painter.drawText(printer.pageRect().width() - data.getSize().width(), data.getSize().height(), data());
}


void PageDrawer::drawHeader(const QString &text, const Align &align){
    header.setText(text);
    painter.setFont(header.getFont());

    switch(align){
        case PageDrawer::Align::CENTER: painter.drawText(printer.pageRect().width() / 2 - header.getSize().width() / 2, nextRow.y(), header()); break;
        case PageDrawer::Align::LEFT: painter.drawText(0, nextRow.y(), header()); break;
    }

    nextRow.setY(nextRow.y() + offset);
}


void PageDrawer::drawVerse(const QString &text, const Align &align){
    data.setText(text);
    painter.setFont(data.getFont());

    switch(align){
        case PageDrawer::Align::CENTER: painter.drawText(printer.pageRect().width() / 2 - data.getSize().width() / 2, nextRow.y(), data()); break;
        case PageDrawer::Align::LEFT: painter.drawText(0, nextRow.y(), data()); break;
    }

    nextRow.setY(nextRow.y() + offset);
}


void PageDrawer::drawInfo(const QString &text, const Align &align, int move){
    info.setText(text);
    painter.setFont(info.getFont());

    switch(align){
        case PageDrawer::Align::CENTER: painter.drawText(printer.pageRect().width() / 2 - info.getSize().width() / 2 + move, nextRow.y(), info()); break;
        case PageDrawer::Align::LEFT: painter.drawText(0 + move, nextRow.y(), info()); break;
    }

    nextRow.setY(nextRow.y() + offset);
}


void PageDrawer::drawBoldVerse(const QString &text, const Align& align){
    label.setText(text);
    painter.setFont(label.getFont());

    switch(align){
        case PageDrawer::Align::CENTER: painter.drawText(printer.pageRect().width() / 2 - label.getSize().width() / 2, nextRow.y(), label()); break;
        case PageDrawer::Align::LEFT: painter.drawText(nextRow.x(), nextRow.y(), label()); break;
    }

    nextRow.setY(nextRow.y() + offset);
}


void PageDrawer::drawOneLineVerses(const QString &firstText, const QString &secondText){
    QPen pen;
    pen.setWidth(2);
    painter.setPen(pen);

    data.setText(firstText);
    painter.setFont(data.getFont());
    painter.drawText(100, nextRow.y(), data());
    painter.drawLine(100, nextRow.y() - data.getSize().height(), 100 + data.getSize().width(), nextRow.y() - data.getSize().height());

    data.setText(secondText);
    painter.drawText(printer.pageRect().width() - data.getSize().width() - 100, nextRow.y(), data());
    painter.drawLine(printer.pageRect().width() - data.getSize().width() - 100, nextRow.y() - data.getSize().height(),
                     printer.pageRect().width() - data.getSize().width() - 100 + data.getSize().width(), nextRow.y()  - data.getSize().height());

    nextRow.setY(nextRow.y() + offset);
}


void PageDrawer::drawDataVerse(const QString &labelText, const QString &dataText){
    nextRow.setX(0);

    label.setText(labelText);
    painter.setFont(label.getFont());
    painter.drawText(nextRow.x(), nextRow.y(), label());

    data.setText(dataText);
    painter.setFont(data.getFont());
    painter.drawText(label.getSize().width() + 10, nextRow.y(), data());

    nextRow.setY(nextRow.y() + offset);
}


void PageDrawer::drawDataRect(const QString &labelText, const QString &dataText, int width, int move){
    data.setText(labelText);
    painter.setFont(data.getFont());
    painter.drawText(nextRow.x(), nextRow.y(), data());

    const int dataWidth = data.getSize().width();

    QPen pen;
    pen.setWidth(2);
    painter.setPen(pen);
    if(move == 0)
        painter.drawRect(QRect(nextRow.x() + data.getSize().width() + 8, nextRow.y() - (data.getSize().height() - 4), width, data.getSize().height() + 1));
    else
        painter.drawRect(QRect(nextRow.x() + move, nextRow.y() - (data.getSize().height() - 4), width, data.getSize().height() + 1));

    data.setText(dataText);
    painter.setFont(data.getFont());
    if(move == 0)
        painter.drawText(nextRow.x() + dataWidth + 8 + (width / 2) - (data.getSize().width() / 2), nextRow.y(), data());
    else
        painter.drawText(nextRow.x() + move + (width / 2) - (data.getSize().width() / 2), nextRow.y(), data());

    nextRow.setY(nextRow.y() + offset);
    nextRow.setX(0);
}


void PageDrawer::drawDataNewVerseRect(const QString &labelText, const QString &dataText, int width, bool move){
    label.setText(labelText);
    painter.setFont(label.getFont());
    painter.drawText(nextRow.x(), nextRow.y(), label());

    data.setText(dataText);
    painter.setFont(data.getFont());
    if(!move)
        painter.drawText(nextRow.x() + label.getSize().width() + 8 + (width / 2) - (data.getSize().width() / 2), nextRow.y(), data());
    else
        painter.drawText(nextRow.x() + 200 + (width / 2) - (data.getSize().width() / 2), nextRow.y(), data());

    QPen pen;
    pen.setWidth(2);
    painter.setPen(pen);
    if(!move)
        painter.drawRect(QRect(nextRow.x() + label.getSize().width() + 8, nextRow.y() - (data.getSize().height() - 4), width, data.getSize().height() + 1));
    else
        painter.drawRect(QRect(nextRow.x() + 200, nextRow.y() - (data.getSize().height() - 4), width, data.getSize().height() + 1));

    nextRow.setY(nextRow.y() + offset);
    nextRow.setX(0);
}


void PageDrawer::drawDataSameVerseRect(const QString &labelText, const QString &dataText, int width, bool move){
    label.setText(labelText);
    painter.setFont(label.getFont());
    painter.drawText(nextRow.x(), nextRow.y(), label());

    data.setText(dataText);
    painter.setFont(data.getFont());
    if(!move)
        painter.drawText(nextRow.x() + label.getSize().width() + 8 + (width / 2) - (data.getSize().width() / 2), nextRow.y(), data());
    else
        painter.drawText(nextRow.x() + 200 + (width / 2) - (data.getSize().width() / 2), nextRow.y(), data());

    QPen pen;
    pen.setWidth(2);
    painter.setPen(pen);
    if(!move){
        painter.drawRect(QRect(nextRow.x() + label.getSize().width() + 8, nextRow.y() - (data.getSize().height() - 4), width, data.getSize().height() + 1));
        nextRow.setX(nextRow.x() + label.getSize().width() + 12 + width);
    }
    else{
        painter.drawRect(QRect(nextRow.x() + 200, nextRow.y() - (data.getSize().height() - 4), width, data.getSize().height() + 1));
        nextRow.setX(nextRow.x() + 204 + width);
    }
}


void PageDrawer::drawCell(const QString &text, int width, int height, bool fillRect){
    currentCell++;

    QStringList textList = text.split("\n");

    data.setText(text);
    painter.setFont(data.getFont());

    QPen pen;
    pen.setWidth(2);
    painter.setPen(pen);
    if(fillRect)
        painter.fillRect(nextRow.x() + tableOffset, nextRow.y() - (data.getSize().height() - 4), width, data.getSize().height() + 1 + height, QColor(200, 200, 200));

    painter.drawRect(nextRow.x() + tableOffset, nextRow.y() - (data.getSize().height() - 4), width, data.getSize().height() + 1 + height);

    if(textList.size() == 1){
        if(height == 0)
            painter.drawText(nextRow.x() + (width / 2) - (data.getSize().width() / 2) + tableOffset, nextRow.y(), data());
        else
            painter.drawText(nextRow.x() + (width / 2) - (data.getSize().width() / 2) + tableOffset, nextRow.y() + (height / 2), data());
    }
    else{
        const int textHeight = data.getSize().height() * textList.size();
        int localY = (nextRow.y() + (height / 2)) - (textHeight / 2) + 5;

        for(int i = 0; i < textList.size(); ++i){
            data.setText(textList.at(i));
            painter.drawText(nextRow.x() + (width / 2) - (data.getSize().width() / 2) + tableOffset, localY, data());
            localY += data.getSize().height() + 2;
        }
    }

    if(currentCell >= columnsCount){
        nextRow.setX(0);
        nextRow.setY(nextRow.y() + offset);
        currentCell = 0;
    }
    else
        nextRow.setX(nextRow.x() + width);
}


void PageDrawer::drawFooter(const QString &text){
    QPen pen;
    pen.setWidth(2);
    painter.setPen(pen);
    int startPosY = printer.pageRect().height() - label.getSize().height() * 4;
    const int posX = printer.pageRect().width() - 200;

    data.setText(text);
    painter.setFont(data.getFont());
    painter.drawText(0, startPosY, data());
    painter.drawText(posX, startPosY, data());

    info.setText("(data i podpis pracodawcy lub osoby reprezentującej");
    painter.drawLine(0, startPosY + info.getSize().height(), info.getSize().width(), startPosY + info.getSize().height());
    painter.drawLine(posX, startPosY + info.getSize().height(), posX + info.getSize().width(), startPosY + info.getSize().height());

    startPosY += offset;
    painter.setFont(info.getFont());
    painter.drawText(0, startPosY, info());

    info.setText("(data i podpis pracownika)");
    painter.drawText(posX, startPosY, info());

    startPosY += offset;
    info.setText("pracodawcę albo osoby upoważnionej do");
    painter.drawText(0, startPosY, info());

    startPosY += offset;
    info.setText("składania oświadczeń w imieniu pracodawcy)");
    painter.drawText(0, startPosY, info());
}


float PageDrawer::getColumnsCount() const {
    return columnsCount;
}


const QPoint& PageDrawer::getPenPosition() const{
    return nextRow;
}
