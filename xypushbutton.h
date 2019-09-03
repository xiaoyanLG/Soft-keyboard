#ifndef XYPUSHBUTTON_H
#define XYPUSHBUTTON_H

#include <QLabel>
#include <QTimer>

class XYPushButton : public QLabel
{
    Q_OBJECT
public:
    explicit XYPushButton(const QString &text, int generalKey, int shiftKey = -1, bool small = false, QWidget *parent=0);
    ~XYPushButton();
    bool isChecked();
    static bool isChinese();
    static QString EnText();
    static QString zhongText();
    QSize sizeHint() const;

signals:
    void mousePressed(XYPushButton *btn);
    void mouseReleaseed(XYPushButton *btn);
    void clicked();
    void clicked(int unicode, int key, Qt::KeyboardModifiers modifiers, bool press);
    void checkedChanged(bool checked = false);

public slots:
    void setChecked(bool checked);
    void setCheckable(bool checkable);
    void startRepeatTimer();

protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    void mousePressedOP();
    void mouseReleaseedOP(bool alwaysEmit, bool contansMouse);

public:
    static void drawIconWithKey(QPainterPath &path, int key, const QRect &rect);

private:
    bool checkable;
    bool checked;
    bool pressed;
    bool isSmallAreaKey;   // 记录是否是小键盘区域的按键
    int repeatEmitTimer;
    QTimer timer;

    int shiftKey;
    int generalKey;

    static QList<XYPushButton *>  allKeyBtns;
    static bool capsLocked;
    static bool numLocked;
    static bool chinese;
    static Qt::KeyboardModifiers modifiers;

    friend class XYVirtualKeyboard;
};

#endif // XYPUSHBUTTON_H
