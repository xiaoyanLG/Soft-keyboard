#include "xyqstringview.h"
#include <QEvent>
#include <QMouseEvent>
//#include <QDebug>

XYQStringView::XYQStringView(QWidget *parent)
    : QWidget(parent), pressed(false)
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
    if (event->type() == QEvent::MouseButtonPress)
    {
        pressed = true;
        pressRect = QRect();
        QMouseEvent *mouse_event = (QMouseEvent *)event;
        int index  = findcontainsMouseRect(mouse_event->pos(), pressRect);
        if (index != -1)
        {
            emit stringPressed(dataStrings.at(index),
                               mapToGlobal(QPoint(pressRect.x(), pressRect.y())));
            update(pressRect);
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
            update(pressRect);
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
                update(pressRect);
                pressRect = QRect();
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
