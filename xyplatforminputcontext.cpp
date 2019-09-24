#define QT_NO_DEBUG_OUTPUT

#include "xyplatforminputcontext.h"
#include "xyvirtualkeyboard.h"
#include "xypushbutton.h"

#include <QWindow>
#include <QGuiApplication>
#include <QDebug>

XYPlatformInputContext::XYPlatformInputContext() :
    m_focusObject(Q_NULLPTR),
    m_locale(),
    m_filterEvent(Q_NULLPTR),
    m_visible(false),
    m_inputDirection(m_locale.textDirection())
{
    m_inputPanel = XYVirtualKeyboard::getInstance(); 
    m_inputPanel->initPinyinDictionary();

    QObject::connect(m_inputPanel, &XYVirtualKeyboard::send_preedit,
                     this, [this](const QString &text){
        if (focusObject()) {
            QInputMethodEvent event(text, QList<QInputMethodEvent::Attribute>());
            m_filterEvent = &event;
            qApp->sendEvent(focusObject(), &event);
            m_filterEvent = Q_NULLPTR;
        }
    });
    QObject::connect(m_inputPanel, &XYVirtualKeyboard::send_commit,
                     this, [this](const QString &text){
        if (focusObject()) {
            QInputMethodEvent event;
            event.setCommitString(text);
            m_filterEvent = &event;
            qApp->sendEvent(focusObject(), &event);
            m_filterEvent = Q_NULLPTR;
        }
    });
    QObject::connect(m_inputPanel, &XYVirtualKeyboard::keyClicked,
                     this, [this](int unicode, int key, Qt::KeyboardModifiers modifiers, bool press){
        if (focusObject()) {
            QKeyEvent event(press ? QEvent::KeyPress : QEvent::KeyRelease,
                            key,
                            modifiers,
                            QChar(unicode));
            m_filterEvent = &event;
            qApp->sendEvent(focusObject(), &event);
            m_filterEvent = Q_NULLPTR;
        }
    });

    QObject::connect(qApp->inputMethod(), &QInputMethod::anchorRectangleChanged,
                     this, [this](){
        const QGuiApplication *app = qApp;
        QWindow *focusWindow = app ? app->focusWindow() : Q_NULLPTR;
        if (focusWindow) {
            this->m_anchorRect = app->inputMethod()->anchorRectangle().toRect().translated(focusWindow->position());
        }
    });
}

XYPlatformInputContext::~XYPlatformInputContext()
{
}

bool XYPlatformInputContext::isValid() const
{
    return true;
}

void XYPlatformInputContext::reset()
{
    qDebug() << Q_FUNC_INFO;
}

void XYPlatformInputContext::commit()
{
    qDebug() << Q_FUNC_INFO;
}

void XYPlatformInputContext::update(Qt::InputMethodQueries queries)
{
    qDebug() << Q_FUNC_INFO;
}

void XYPlatformInputContext::invokeAction(QInputMethod::Action action, int cursorPosition)
{
    qDebug() << Q_FUNC_INFO;
}

QRectF XYPlatformInputContext::keyboardRect() const
{
    qDebug() << Q_FUNC_INFO;
    return QRectF();
}

bool XYPlatformInputContext::isAnimating() const
{
    qDebug() << Q_FUNC_INFO;
    return false;
}

void XYPlatformInputContext::showInputPanel()
{
    qDebug() << Q_FUNC_INFO;
    m_inputPanel->move(m_anchorRect.bottomLeft() + QPoint(0, 10));
    m_inputPanel->show();
}

void XYPlatformInputContext::hideInputPanel()
{
    qDebug() << Q_FUNC_INFO;
    m_inputPanel->close();
}

bool XYPlatformInputContext::isInputPanelVisible() const
{
    qDebug() << Q_FUNC_INFO;
    return m_inputPanel ? m_inputPanel->isVisible() : false;
}

QLocale XYPlatformInputContext::locale() const
{
    qDebug() << Q_FUNC_INFO;
    return m_locale;
}

void XYPlatformInputContext::setLocale(QLocale locale)
{
    qDebug() << Q_FUNC_INFO;
    if (m_locale != locale) {
        m_locale = locale;
        emitLocaleChanged();
    }
}

Qt::LayoutDirection XYPlatformInputContext::inputDirection() const
{
    qDebug() << Q_FUNC_INFO;
    return m_inputDirection;
}

void XYPlatformInputContext::setInputDirection(Qt::LayoutDirection direction)
{
    qDebug() << Q_FUNC_INFO;
}

QObject *XYPlatformInputContext::focusObject()
{
    qDebug() << Q_FUNC_INFO;
    return m_focusObject;
}

void XYPlatformInputContext::setFocusObject(QObject *object)
{
    qDebug() << Q_FUNC_INFO;
    if (m_focusObject != object) {
        if (m_focusObject)
            m_focusObject->removeEventFilter(this);
        m_focusObject = object;
        if (m_focusObject)
            m_focusObject->installEventFilter(this);
    }
    update(Qt::ImQueryAll);
}

bool XYPlatformInputContext::eventFilter(QObject *object, QEvent *event)
{
    if (event != m_filterEvent
            && event->type() == QEvent::KeyPress
            && object == m_focusObject
            && m_inputPanel
            && XYPushButton::isChinese())
    {
        return m_inputPanel->handleQKeyEvent(static_cast<QKeyEvent *>(event));
    }

    return false;
}
