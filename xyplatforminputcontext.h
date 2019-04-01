#ifndef XYPLATFORMINPUTCONTEXT_H
#define XYPLATFORMINPUTCONTEXT_H

#include <qevent.h>
#include <qpa/qplatforminputcontext.h>
#include <QPointer>
#include <QLocale>

class XYVirtualKeyboard;
class XYPlatformInputContext : public QPlatformInputContext
{
    Q_OBJECT
public:
    explicit XYPlatformInputContext();
    ~XYPlatformInputContext();

    virtual bool isValid() const;

    virtual void reset();
    virtual void commit();
    virtual void update(Qt::InputMethodQueries queries);
    virtual void invokeAction(QInputMethod::Action action, int cursorPosition);
    virtual QRectF keyboardRect() const;

    virtual bool isAnimating() const;

    virtual void showInputPanel();
    virtual void hideInputPanel();
    virtual bool isInputPanelVisible() const;

    virtual QLocale locale() const;
    void setLocale(QLocale locale);
    virtual Qt::LayoutDirection inputDirection() const;
    void setInputDirection(Qt::LayoutDirection direction);

    QObject *focusObject();
    virtual void setFocusObject(QObject *object);

    virtual bool eventFilter(QObject *object, QEvent *event);

private:
    XYVirtualKeyboard  *m_inputPanel;
    QRect               m_anchorRect;
    QObject            *m_focusObject;
    QLocale             m_locale;
    QEvent             *m_filterEvent;
    bool                m_visible;
    Qt::LayoutDirection m_inputDirection;
};

#endif // XYPLATFORMINPUTCONTEXT_H
