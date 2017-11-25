#include "xyvdragabletranslateview.h"
#include "xyskin.h"
#include <QPainter>

XYVDragableTranslateView::XYVDragableTranslateView(QWidget *parent)
    : XYQStringView(parent)
{

}

void XYVDragableTranslateView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setFont(XYSKIN->candidateTextFont);

    QFontMetrics metrics(painter.font());
    int cur_x = 5;
    int text_w = 0;
    int text_h = qMax(metrics.height(), unitMinHeight);
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
    int cur_y = 0;
    for (int i = 0; i < dataStrings.size(); ++i)
    {
        text = dataStrings.at(i);
        text_w = metrics.width(text) + 15;
        if (unitMinWidth != -1)
        {
            text_w = qMax(unitMinWidth, text_w);
        }

        if (cur_x + text_w > width())
        {
            cur_y += text_h + 5;
            cur_x = 5;
        }
        text_rect = QRect(cur_x, cur_y, text_w, text_h);
        painter.drawText(text_rect, text, QTextOption(Qt::AlignCenter));
        dataRects.append(text_rect);

        cur_x += text_w + 13;
    }

    setFixedHeight(cur_y + text_h + 5);

    XYQStringView::paintEvent(event);
}

