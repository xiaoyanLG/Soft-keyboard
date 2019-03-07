#include "xydragablewidget.h"
#include <QScrollBar>
#include <QEvent>
#include <QMouseEvent>

XYDragableWidget::XYDragableWidget(QWidget *centerWidget, XYDragableWidget::DIRECTION dire, QWidget *parent)
    : QWidget(parent), filterObjsPressed(false), mouseSensitivity(1), centerWidget(centerWidget), direction(dire)
{
    this->centerWidget = centerWidget;
    this->centerWidget->setParent(this);
    this->centerWidget->installEventFilter(this);
}

void XYDragableWidget::resizeEvent(QResizeEvent *event)
{
    if (direction == HORIZONTAL)
    {
        this->centerWidget->setFixedHeight(this->height());
        this->centerWidget->setMinimumWidth(this->width());
    }
    else
    {
        this->centerWidget->setMinimumHeight(this->height());
        this->centerWidget->setFixedWidth(this->width());
    }

    QWidget::resizeEvent(event);
}

bool XYDragableWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (centerWidget == obj)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouse_event = (QMouseEvent *)event;
            filterObjsPressed = true;
            filterObjscurPressedPoint = filterObjsLastPressedPoint = mouse_event->globalPos();
        }
        else if (event->type() == QEvent::MouseButtonRelease)
        {
            filterObjsPressed = false;
            filterObjsLastPressedPoint = QPoint();
        }
        else if (event->type() == QEvent::Wheel)
        {
            QWheelEvent *wheel_event = (QWheelEvent *)event;
            if (direction == HORIZONTAL)
            {
                if (wheel_event->delta() < 0)
                {
                    if (centerWidget->pos().x() - 1 >= width() - centerWidget->width())
                    {
                        centerWidget->move(centerWidget->pos().x() - mouseSensitivity * 2, centerWidget->pos().y());
                    }
                }
                else if (wheel_event->delta() > 0)
                {
                    if (centerWidget->pos().x() + 1 <= 0)
                    {
                        centerWidget->move(centerWidget->pos().x() + mouseSensitivity * 2, centerWidget->pos().y());
                    }
                }
            }
            else
            {
                if (wheel_event->delta() < 0)
                {
                    if (centerWidget->pos().y() - 1 >= height() - centerWidget->height())
                    {
                        centerWidget->move(centerWidget->pos().x(), centerWidget->pos().y() - mouseSensitivity * 2);
                    }
                }
                else if (wheel_event->delta() > 0)
                {
                    if (centerWidget->pos().y() + 1 <= 0)
                    {
                        centerWidget->move(centerWidget->pos().x(), centerWidget->pos().y() + mouseSensitivity * 2);
                    }
                }
            }
        }
        else if (event->type() == QEvent::MouseMove && filterObjsPressed)
        {
            QMouseEvent *mouse_event = (QMouseEvent *)event;
            if (direction == HORIZONTAL)
            {
                if (qAbs(filterObjscurPressedPoint.x() - mouse_event->globalPos().x()) > 10) // 边界值
                {
                    if (filterObjsLastPressedPoint.x() > mouse_event->globalPos().x() + 5)
                    {
                        if (centerWidget->pos().x() - 1 >= width() - centerWidget->width())
                        {
                            centerWidget->move(centerWidget->pos().x() - mouseSensitivity, centerWidget->pos().y());
                        }
                        filterObjsLastPressedPoint = mouse_event->globalPos();
                    }
                    else if (filterObjsLastPressedPoint.x() < mouse_event->globalPos().x() - 5)
                    {
                        if (centerWidget->pos().x() + 1 <= 0)
                        {
                            centerWidget->move(centerWidget->pos().x() + mouseSensitivity, centerWidget->pos().y());
                        }
                        filterObjsLastPressedPoint = mouse_event->globalPos();
                    }
                }
            }
            else
            {
                if (qAbs(filterObjscurPressedPoint.y() - mouse_event->globalPos().y()) > 10) // 边界值
                {
                    if (filterObjsLastPressedPoint.y() > mouse_event->globalPos().y() + 5)
                    {
                        if (centerWidget->pos().y() - 1 >= height() - centerWidget->height())
                        {
                            centerWidget->move(centerWidget->pos().x(), centerWidget->pos().y() - mouseSensitivity);
                        }
                        filterObjsLastPressedPoint = mouse_event->globalPos();
                    }
                    else if (filterObjsLastPressedPoint.y() < mouse_event->globalPos().y() - 5)
                    {
                        if (centerWidget->pos().y() + 1 <= 0)
                        {
                            centerWidget->move(centerWidget->pos().x(), centerWidget->pos().y() + mouseSensitivity);
                        }
                        filterObjsLastPressedPoint = mouse_event->globalPos();
                    }
                }
            }
        }
    }

    return QWidget::eventFilter(obj, event);
}

int XYDragableWidget::getMouseSensitivity() const
{
    return mouseSensitivity;
}

void XYDragableWidget::setMouseSensitivity(int value)
{
    mouseSensitivity = value;
}

