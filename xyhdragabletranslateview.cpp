#include "xyhdragabletranslateview.h"
#include "xydragablewidget.h"
#include "xyskin.h"
#include <QPainter>
//#include <QDebug>

XYHDragableTranslateView::XYHDragableTranslateView(QWidget *parent)
    : XYQStringView(parent)
{

}

void XYHDragableTranslateView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setFont(XYSKIN->candidateTextFont);

    QFontMetrics metrics(painter.font());
    int cur_x = 10;
    int text_w = 0;
    QString text;
    QRect text_rect;
    dataRects.clear();

    // 先绘制按下的矩形
    if (!pressRect.isEmpty())
    {
        painter.setPen(XYSKIN->candidateTextPressedBKColor);
        painter.setBrush(XYSKIN->candidateTextPressedBKColor);
        painter.drawRect(pressRect);
    }

    // 绘制所有的文字
    painter.setPen(XYSKIN->candidateTextPen);
    for (int i = 0; i < dataStrings.size(); ++i)
    {
        text = dataStrings.at(i);
        text_w = metrics.width(text) + 15;
        if (unitMinWidth != -1)
        {
            text_w = qMax(unitMinWidth, text_w);
        }

        text_rect = QRect(cur_x, 0, text_w, height());
        painter.drawText(text_rect, text, QTextOption(Qt::AlignCenter));
        dataRects.append(text_rect);
        cur_x += text_w + 15;
    }

    setFixedWidth(cur_x);

    XYQStringView::paintEvent(event);
}
