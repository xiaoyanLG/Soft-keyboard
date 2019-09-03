#include "xypushbutton.h"
#include "xyskin.h"
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
//#include <QDebug>

bool XYPushButton::capsLocked = false;
bool XYPushButton::numLocked = false;
bool XYPushButton::chinese = false;
QList<XYPushButton *> XYPushButton::allKeyBtns;
Qt::KeyboardModifiers XYPushButton::modifiers = 0;
XYPushButton::XYPushButton(const QString &text, int generalKey, int shiftKey, bool small, QWidget *parent)
    : QLabel(text, parent),
      generalKey(generalKey),
      shiftKey(shiftKey),
      isSmallAreaKey(small),
      checked(false),
      pressed(false),
      checkable(false),
      repeatEmitTimer(0)
{
    timer.setSingleShot(true);
    timer.setInterval(2000);
    setMinimumHeight(30);
    setAlignment(Qt::AlignCenter);
    allKeyBtns.append(this);
    connect(&timer, SIGNAL(timeout()), this, SLOT(startRepeatTimer()));
}

XYPushButton::~XYPushButton()
{

}

bool XYPushButton::isChecked()
{
    return checked;
}

bool XYPushButton::isChinese()
{
    return chinese;
}

QString XYPushButton::EnText()
{
    return QString::fromUtf8("En");
}

QString XYPushButton::zhongText()
{
#if QT_VERSION < 0x050000
            return QString::fromUtf8("中");
#else
            return QStringLiteral("中");
#endif
}

QSize XYPushButton::sizeHint() const
{
    return QSize(60, 38);
}

void XYPushButton::setCheckable(bool checkable)
{
    this->checkable = checkable;
}

void XYPushButton::startRepeatTimer()
{
    if (pressed)
    {
        repeatEmitTimer = startTimer(100);
    }
    else
    {
        if (repeatEmitTimer != 0)
        {
            killTimer(repeatEmitTimer);
            repeatEmitTimer = 0;
        }
    }
}

void XYPushButton::setChecked(bool checked)
{
    this->checked = checked;
    update();
}

void XYPushButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    KeyButtonStyle *style;
    if (pressed || (checkable && checked))
    {
        style = XYSKIN->getStyleByKey(generalKey, true);
    }
    else
    {
        style = XYSKIN->getStyleByKey(generalKey, false);
    }

    // 设置字体大小
    QString draw_text = text();
    QFont font = style->textFont;
    QPainterPath draw_path;

    if (!style->backImage.isNull())
    {
        painter.setBrush(style->backImage.scaled(size()));
    }
    else
    {
        painter.setBrush(style->backgroundColor);
    }

    painter.setPen(style->borderPen);
    painter.drawRoundedRect(rect().x() + 1,
                            rect().y() + 1,
                            rect().width() - 2,
                            rect().height() - 2,
                            4, 4);

    painter.setPen(style->textPen);
    if (generalKey == Qt::Key_Menu)
    {
        QRect left = rect();
        left.setWidth(left.width() / 2);
        QRect right = left;
        right.setX(left.x() + left.width());
        right.setWidth(left.width());

        QString left_text, right_text;
        if (chinese)
        {
            font.setPixelSize(font.pixelSize() - 4);
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
            font.setPixelSize(font.pixelSize() - 4);
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

    font.setPixelSize(style->textFont.pixelSize());
    if (draw_text.size() > 1)
    {
        font.setPixelSize(font.pixelSize() - 4);
    }
    painter.setFont(font);

    switch (generalKey)
    {
    case Qt::Key_Shift:
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        drawIconWithKey(draw_path, generalKey, rect());
        break;
    case Qt::Key_Backspace:
        drawIconWithKey(draw_path, generalKey, rect());
        break;
    }

    if (!draw_text.isEmpty() && generalKey != Qt::Key_Menu)
    {
        painter.drawText(rect(), text(), QTextOption(Qt::AlignCenter));
    }
    if (!draw_path.isEmpty())
    {
        painter.setPen(painter.brush().color());
        painter.setBrush(style->textPen.color());
        painter.drawPath(draw_path);
    }
}

void XYPushButton::timerEvent(QTimerEvent *event)
{
    if (repeatEmitTimer == event->timerId())
    {
        if (!pressed)
        {
            killTimer(repeatEmitTimer);
            repeatEmitTimer = 0;
            return;
        }
        int unicode = generalKey;
        switch (generalKey)
        {
        case Qt::Key_Tab:
            unicode = 0x09;
            break;
        case Qt::Key_Backspace:
            unicode = 0x08;
            break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            unicode = 0x0A;
            break;
        default:
            break;
        }

        if (unicode > 255)
        {
            unicode = 0;
        }

        if (Qt::Key_A <= generalKey && generalKey <= Qt::Key_Z)
        {
            if (modifiers & Qt::ShiftModifier)
            {
                unicode = generalKey;
            }
            else
            {
                unicode = generalKey + 32;
            }
        }
        emit clicked(unicode, generalKey, modifiers, true);
    }
}

void XYPushButton::mousePressEvent(QMouseEvent *event)
{
    event->accept();
    if (event->button() == Qt::LeftButton)
    {
        mousePressedOP();
    }
}

void XYPushButton::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
    if (event->button() == Qt::LeftButton)
    {
        if (rect().contains(event->pos()))
        {
            mouseReleaseedOP(false, true);
        }
        else
        {
            mouseReleaseedOP(false, false);
        }
    }

    emit mouseReleaseed(this);
    timer.stop();
    if (repeatEmitTimer != 0)
    {
        killTimer(repeatEmitTimer);
        repeatEmitTimer = 0;
    }
    pressed = false;
    update();
}

void XYPushButton::mouseMoveEvent(QMouseEvent *event)
{
    if (pressed)
    {
        if (!rect().contains(event->pos()))
        {
            if (timer.isActive())
            {
                timer.stop();
                if (repeatEmitTimer != 0)
                {
                    killTimer(repeatEmitTimer);
                    repeatEmitTimer = 0;
                }
            }
            emit mouseReleaseed(this);
        }
        else
        {
            timer.start();
            emit mousePressed(this);
        }
    }
}

void XYPushButton::mousePressedOP()
{
    emit mousePressed(this);
    pressed = true;
    switch (generalKey)
    {
    case Qt::Key_Shift:
        break;
    case Qt::Key_Meta:
        break;
    case Qt::Key_Control:
        break;
    case Qt::Key_Alt:
        break;
    case Qt::Key_CapsLock:
        break;
    case Qt::Key_NumLock:
        break;
    case Qt::Key_Menu:
        break;
    default:
        timer.start();
        break;
    }
    update();
}

void XYPushButton::mouseReleaseedOP(bool alwaysEmit, bool contansMouse)
{
    if (contansMouse)
    {
        if (checkable)
        {
            checked = !checked;
            emit checkedChanged(checked);
        }
        bool general_key = false;
        int key = generalKey;
        int unicode = key;
        if (checked)
        {
            switch (generalKey)
            {
            case Qt::Key_Shift:
                modifiers |= Qt::ShiftModifier;
                break;
            case Qt::Key_Meta:
//                modifiers |= Qt::MetaModifier;
                break;
            case Qt::Key_Control:
                modifiers |= Qt::ControlModifier;
                break;
            case Qt::Key_Alt:
                modifiers |= Qt::AltModifier;
                break;
            case Qt::Key_CapsLock:
                capsLocked = true;
                break;
            case Qt::Key_NumLock:
                numLocked = true;
                break;
            case Qt::Key_Menu:
                break;
            default:
                general_key = true;
                break;
            }
        }
        else
        {
            switch (generalKey)
            {
            case Qt::Key_Shift:
                modifiers &= !Qt::ShiftModifier;
                break;
            case Qt::Key_Meta:
//                modifiers &= !Qt::MetaModifier;
                break;
            case Qt::Key_Control:
                modifiers &= !Qt::ControlModifier;
                break;
            case Qt::Key_Alt:
                modifiers &= !Qt::AltModifier;
                break;
            case Qt::Key_CapsLock:
                capsLocked = false;
                break;
            case Qt::Key_NumLock:
                numLocked = false;
                break;
            case Qt::Key_Menu:
                chinese = !chinese;
                break;
            case Qt::Key_Tab:
                unicode = 0x09;
                break;
            case Qt::Key_Backspace:
                unicode = 0x08;
                break;
            case Qt::Key_Return:
            case Qt::Key_Enter:
                unicode = 0x0A;
                break;
            default:
                general_key = true;
                break;
            }
        }

        if (Qt::Key_A <= generalKey && generalKey <= Qt::Key_Z)
        {
            if (capsLocked || (modifiers & Qt::ShiftModifier))
            {
                unicode = generalKey;
            }
            else
            {
                unicode = generalKey + 32;
            }
        }

        if (unicode > 255)
        {
            unicode = 0;
        }

        if (checkable)
        {
            clicked(unicode, key, modifiers, checked);
        }
        else
        {
            clicked(unicode, key, modifiers, true);
            clicked(unicode, key, modifiers, false);
        }
        emit clicked();
    }

    if (alwaysEmit)
    {
        emit mouseReleaseed(this);
        timer.stop();
        if (repeatEmitTimer != 0)
        {
            killTimer(repeatEmitTimer);
            repeatEmitTimer = 0;
        }
        pressed = false;
        update();
    }
}

void XYPushButton::drawIconWithKey(QPainterPath &path, int key, const QRect &rect)
{
    int beginX;
    int beginY;
    qreal x_10 = rect.width() / 10;
    qreal y_10 = rect.height() / 10;
    switch (key)
    {
    case Qt::Key_Shift:
        beginX = rect.width() * 0.62;
        beginY = rect.height() * 0.47;
        path.moveTo(beginX, beginY);
        path.lineTo(beginX + x_10 * 1, beginY - y_10 * 2.1);
        path.lineTo(beginX + x_10 * 2, beginY);
        path.lineTo(beginX + x_10 * 1.4, beginY);
        path.lineTo(beginX + x_10 * 1.4, beginY + y_10 * 2.8);
        path.lineTo(beginX + x_10 * 0.6, beginY + y_10 * 2.8);
        path.lineTo(beginX + x_10 * 0.6, beginY);
        path.lineTo(beginX, beginY);
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        beginX = rect.width() * 0.23;
        beginY = rect.height() * 0.5;
        path.moveTo(beginX, beginY);
        path.lineTo(beginX + x_10 * 1.4, beginY - y_10 * 2.5);
        path.lineTo(beginX + x_10 * 1.4, beginY - y_10 * 1);
        path.lineTo(beginX + x_10 * 5.4 - y_10 * 2.5, beginY - y_10 * 1);
        path.lineTo(beginX + x_10 * 5.4 - y_10 * 2.5, beginY - y_10 * 2.5);
        path.lineTo(beginX + x_10 * 5.4, beginY - y_10 * 2.5);
        path.lineTo(beginX + x_10 * 5.4, beginY + y_10 * 1);
        path.lineTo(beginX + x_10 * 1.4, beginY + y_10 * 1);
        path.lineTo(beginX + x_10 * 1.4, beginY + y_10 * 2.5);
        path.lineTo(beginX, beginY);
        break;
    case Qt::Key_Backspace:
        beginX = rect.width() * 0.23;
        beginY = rect.height() * 0.5;
        path.moveTo(beginX, beginY);
        path.lineTo(beginX + x_10 * 1.4, beginY - y_10 * 2.5);
        path.lineTo(beginX + x_10 * 1.4, beginY - y_10 * 1);
        path.lineTo(beginX + x_10 * 5.4, beginY - y_10 * 1);
        path.lineTo(beginX + x_10 * 5.4, beginY + y_10 * 1);
        path.lineTo(beginX + x_10 * 1.4, beginY + y_10 * 1);
        path.lineTo(beginX + x_10 * 1.4, beginY + y_10 * 2.5);
        path.lineTo(beginX, beginY);
        break;
    }
}

