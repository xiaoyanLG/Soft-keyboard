#include <QApplication>
#include "xykeyboardfilter.h"
#include "xyvirtualkeyboard.h"
#include "xyskin.h"
#include <QIcon>

#define TEST
#ifdef TEST
#include <QDebug>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QInputMethodEvent>

static void sendPreeditText(const QString &text)
{
    QInputMethodEvent *event =
            new QInputMethodEvent(text, QList<QInputMethodEvent::Attribute>());
    qApp->postEvent(qApp->focusWidget(), event);
}

static void sendCommitText(const QString &text)
{
    QInputMethodEvent *event = new QInputMethodEvent;
    event->setCommitString(text);
    qApp->postEvent(qApp->focusWidget(), event);
}

static void sendKeyEvent(int unicode, int key, Qt::KeyboardModifiers modifiers, bool press)
{
    QKeyEvent *event = new QKeyEvent(press ? QEvent::KeyPress : QEvent::KeyRelease,
                                     key,
                                     modifiers,
                                     QChar(unicode));
    qApp->postEvent(qApp->focusWidget(), event);
}

#endif
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    XYSKIN->writeSkipToFile("I:/DeskTop/test.skin");
    XYSKIN->loadSkipWithFile(qApp->applicationDirPath() + "/skin/xyinput.skin");
    XYVirtualKeyboard::getInstance()->show();

#ifdef TEST
    // 测试代码
    QWidget widget;
    QVBoxLayout *layout = new QVBoxLayout(&widget);
    layout->addWidget(new QLineEdit);
    layout->addWidget(new QTextEdit);
    layout->addWidget(new QLineEdit);
    widget.show();
    QObject::connect(XYVirtualKeyboard::getInstance(),
                     &XYVirtualKeyboard::send_preedit, &sendPreeditText);
    QObject::connect(XYVirtualKeyboard::getInstance(),
                     &XYVirtualKeyboard::send_commit, &sendCommitText);
    QObject::connect(XYVirtualKeyboard::getInstance(),
                     &XYVirtualKeyboard::keyClicked, &sendKeyEvent);
#endif

    a.setQuitOnLastWindowClosed(true);
    a.setWindowIcon(QIcon(":/Keyboard.ico"));
    return a.exec();
}
