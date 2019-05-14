#include <QApplication>
#include "xyvirtualkeyboard.h"
#include "xyskin.h"
#include <QIcon>
#include <QDebug>

#define TEST
#ifdef TEST
#include <QWindow>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QInputMethodEvent>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    XYSKIN->writeSkinToFile("I:/DeskTop/test.skin");
    XYSKIN->loadSkinWithFile(qApp->applicationDirPath() + "/skin/xyinput.skin");
    XYVirtualKeyboard::getInstance()->initPinyinDictionary();
    XYVirtualKeyboard::getInstance()->show();

#ifdef TEST
    // 测试代码
    QWidget widget;
    QVBoxLayout *layout = new QVBoxLayout(&widget);
    layout->addWidget(new QLineEdit);
    layout->addWidget(new QTextEdit);
    layout->addWidget(new QLineEdit);
    widget.show();

    QObject::connect(qApp->inputMethod(), &QInputMethod::cursorRectangleChanged,
                     XYVirtualKeyboard::getInstance(), [](){
        QWindow *focusWindow = qApp->focusWindow();
        if (focusWindow && qApp->focusWidget() && !XYVirtualKeyboard::getInstance()->isVisible()) {
            QRect rect = qApp->inputMethod()->cursorRectangle().toRect().translated(focusWindow->position());
            XYVirtualKeyboard::getInstance()->move(rect.bottomLeft() + QPoint(0, 5));
            XYVirtualKeyboard::getInstance()->show();
        }
    });

    QObject::connect(XYVirtualKeyboard::getInstance(),
                     &XYVirtualKeyboard::send_preedit, [](const QString &text){
        QInputMethodEvent *event =
                new QInputMethodEvent(text, QList<QInputMethodEvent::Attribute>());
        const QApplication *app = qApp;
        QWidget *focusWindow = app ? app->focusWidget() : 0;
        if (focusWindow) {
            app->postEvent(focusWindow, event);
        }
    });
    QObject::connect(XYVirtualKeyboard::getInstance(),
                     &XYVirtualKeyboard::send_commit, [](const QString &text){
        QInputMethodEvent *event = new QInputMethodEvent;
        event->setCommitString(text);
        const QApplication *app = qApp;
        QWidget *focusWindow = app ? app->focusWidget() : 0;
        if (focusWindow) {
            app->postEvent(focusWindow, event);
        }
    });
    QObject::connect(XYVirtualKeyboard::getInstance(),
                     &XYVirtualKeyboard::keyClicked, [](int unicode, int key, Qt::KeyboardModifiers modifiers, bool press){
        QKeyEvent *event = new QKeyEvent(press ? QEvent::KeyPress : QEvent::KeyRelease,
                                         key,
                                         modifiers,
                                         QChar(unicode));
        const QApplication *app = qApp;
        QWidget *focusWindow = app ? app->focusWidget() : 0;
        if (focusWindow) {
            app->postEvent(focusWindow, event);
        }
    });
#endif

    a.setQuitOnLastWindowClosed(true);
    a.setWindowIcon(QIcon(":/Keyboard.ico"));
    return a.exec();
}
