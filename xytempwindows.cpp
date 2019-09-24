#include "xytempwindows.h"
#include "xypushbutton.h"
#include "xyskin.h"
#include <QPainter>

XYTempWindows::XYTempWindows(XYTempWindows::DIRECTION direction, QWidget *parent)
    : QWidget(parent), direction(direction), key(0)
{
    this->setWindowFlags(Qt::FramelessWindowHint
                        | Qt::WindowStaysOnTopHint
                        | Qt::Tool);
#if QT_VERSION >= 0x050000
    this->setWindowFlags(this->windowFlags() | Qt::WindowDoesNotAcceptFocus);
#endif
    this->setAttribute(Qt::WA_TranslucentBackground);

    setFont(XYSKIN->popupButtonStyle->textFont);
}

void XYTempWindows::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    KeyButtonStyle *style = XYSKIN->popupButtonStyle;

    painter.setFont(style->textFont);
    QFont font = painter.font();
    QPainterPath path = getPainterPath(this->rect(), direction);

    painter.setPen(style->borderPen);
    if (!style->backImage.isNull())
    {
        painter.setBrush(style->backImage.scaled(textRect.size()));
    }
    else
    {
        painter.setBrush(style->backgroundColor);
    }
    painter.drawPath(path);
    painter.setPen(style->textPen);

    if (key == Qt::Key_Backspace
            || key == Qt::Key_Return
            || key == Qt::Key_Enter)
    {
        QPainterPath path;
        XYPushButton::drawIconWithKey(path, key, textRect);
        painter.setPen(painter.brush().color());
        painter.setBrush(style->textPen.color());
        painter.drawPath(path);
    }
    else if (key == Qt::Key_Menu)
    {
        QRect left = textRect;
        left.setWidth(left.width() / 2);
        QRect right = left;
        right.setX(left.x() + left.width());
        right.setWidth(left.width());
        QString left_text, right_text;
        if (XYPushButton::isChinese())
        {
            left_text = XYPushButton::EnText() + "/";
            right_text = XYPushButton::zhongText();
            font.setBold(false);
            font.setPixelSize(font.pixelSize() - 2);
            painter.setFont(font);
            painter.drawText(left, left_text, QTextOption(Qt::AlignRight|Qt::AlignVCenter));
            font.setBold(true);
            font.setPixelSize(font.pixelSize() + 2);
            painter.setFont(font);
            painter.drawText(right, right_text, QTextOption(Qt::AlignLeft|Qt::AlignVCenter));
        }
        else
        {
            left_text = XYPushButton::EnText();
            right_text = "/" + XYPushButton::zhongText();
            font.setBold(true);
            painter.setFont(font);
            painter.drawText(left, left_text, QTextOption(Qt::AlignRight|Qt::AlignVCenter));
            font.setBold(false);
            font.setPixelSize(font.pixelSize() - 2);
            painter.setFont(font);
            painter.drawText(right, right_text, QTextOption(Qt::AlignLeft|Qt::AlignVCenter));
        }
    }
    else if (key == Qt::Key_Space)
    {
        painter.drawText(textRect, "Space", QTextOption(Qt::AlignCenter));
    }
    else
    {
        painter.drawText(textRect, text, QTextOption(Qt::AlignCenter));
    }
}

QPainterPath XYTempWindows::getPainterPath(const QRect &rect, XYTempWindows::DIRECTION direction)
{
    QPainterPath path;
    int arrow_h = 5;
    switch (direction)
    {
    case TOP:
        arrow_h = qRound(rect.height() * 0.1);
        textRect = QRect(rect.x() + 1, rect.y() + arrow_h + 1,
                         rect.width() - 2, rect.height() - arrow_h - 2);
        path.addRoundedRect(textRect, 4, 4);
        path.moveTo(rect.x() + rect.width() / 2 - arrow_h / 2, rect.y() + arrow_h + 1);
        path.lineTo(rect.x() + rect.width() / 2, rect.y());
        path.lineTo(rect.x() + rect.width() / 2 + arrow_h / 2, rect.y() + arrow_h + 1);
        break;
    case BOTTOM:
        arrow_h = qRound(rect.height() * 0.1);
        textRect = QRect(rect.x() + 1, rect.y() + 1,
                         rect.width() - 2, rect.height() - arrow_h - 1);
        path.addRoundedRect(textRect, 4, 4);
        path.moveTo(rect.x() + rect.width() / 2 - arrow_h / 2, rect.y() + rect.height() - arrow_h);
        path.lineTo(rect.x() + rect.width() / 2, rect.y() + rect.height());
        path.lineTo(rect.x() + rect.width() / 2 + arrow_h / 2, rect.y() + rect.height() - arrow_h);
        break;
    case LEFT:
        arrow_h = qRound(rect.height() * 0.1);
        textRect = QRect(rect.x() + arrow_h, rect.y() + 1,
                         rect.width() - arrow_h - 2, rect.height() - 2);
        path.addRoundedRect(textRect, 4, 4);
        path.moveTo(rect.x() + arrow_h, rect.y() + rect.height() / 2 -  arrow_h / 2);
        path.lineTo(rect.x() - 2, rect.y() + rect.height() / 2);
        path.lineTo(rect.x() + arrow_h, rect.y() + rect.height() / 2 +  arrow_h / 2);
        break;
    case RIGHT:
        arrow_h = qRound(rect.height() * 0.1);
        textRect = QRect(rect.x() + 1, rect.y() + 1,
                         rect.width() - arrow_h - 2, rect.height() - 2);
        path.addRoundedRect(textRect, 4, 4);
        path.moveTo(rect.x() - 1 + rect.width() - arrow_h, rect.y() + rect.height() / 2 -  arrow_h / 2);
        path.lineTo(rect.x() - 1 + rect.width(), rect.y() + rect.height() / 2);
        path.lineTo(rect.x() - 1 + rect.width() - arrow_h, rect.y() + rect.height() / 2 +  arrow_h / 2);
        break;
    default:
        break;
    }
    return path.simplified();
}

int XYTempWindows::getKey() const
{
    return key;
}

void XYTempWindows::setKey(int value)
{
    key = value;
}

XYTempWindows::DIRECTION XYTempWindows::getDirection() const
{
    return direction;
}

void XYTempWindows::setDirection(const DIRECTION &value)
{
    direction = value;
}

QString XYTempWindows::getText() const
{
    return text;
}

void XYTempWindows::setText(const QString &value)
{
    text = value;
}


