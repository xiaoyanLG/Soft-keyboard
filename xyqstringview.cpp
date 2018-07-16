#include "xyqstringview.h"
#include <QEvent>
#include <QMouseEvent>
//#include <QDebug>

XYQStringView::XYQStringView(QWidget *parent)
    : QWidget(parent)
{
    unitMinWidth = -1;
    unitMinHeight = -1;
}
int XYQStringView::getUnitMinWidth() const
{
    return unitMinWidth;
}

void XYQStringView::setUnitMinWidth(int value)
{
    unitMinWidth = value;
}
int XYQStringView::getUnitMinHeight() const
{
    return unitMinHeight;
}

void XYQStringView::setUnitMinHeight(int value)
{
    unitMinHeight = value;
}

bool XYQStringView::event(QEvent *event)
{
    static bool pressed = false;
    static QPoint pressedPoint;
    if (event->type() == QEvent::MouseButtonPress)
    {
        pressed = true;
        pressRect = QRect();
        QMouseEvent *mouse_event = (QMouseEvent *)event;
        pressedPoint = mouse_event->globalPos();
        int index  = findcontainsMouseRect(mouse_event->pos(), pressRect);
        if (index != -1)
        {
            emit stringPressed(dataStrings.at(index),
                               mapToGlobal(QPoint(pressRect.x(), pressRect.y())));
            update();
        }
        return true;
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        if (pressed)
        {
            if (!pressRect.isEmpty())
            {
                int index = dataRects.indexOf(pressRect);
                emit clicked(dataStrings.at(index), index);
            }
            update();
            emit stringPressed("", QPoint());
        }
        pressed = false;
        pressRect = QRect();
        return true;
    }
    else if (pressed && event->type() == QEvent::MouseMove)
    {
        if (pressed)
        {
            QRect cur_rect;
            QMouseEvent *mouse_event = (QMouseEvent *)event;
            int index  = findcontainsMouseRect(mouse_event->pos(), cur_rect);
            if (index == -1)
            {
                emit stringPressed("", QPoint());
                pressed = false;
                pressRect = QRect();
                update();
            }
        }
        return true;
    }

    return QWidget::event(event);
}

int XYQStringView::findcontainsMouseRect(const QPoint &mousePos, QRect &rect)
{
    for (int i = 0; i < dataRects.size(); ++i)
    {
        if (dataRects.at(i).contains(mousePos))
        {
            rect = dataRects.at(i);
            return i;
        }
    }

    return -1;
}
