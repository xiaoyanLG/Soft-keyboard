#include <QApplication>
#include "xykeyboardfilter.h"
#include "xyvirtualkeyboard.h"
#include "xyskin.h"
#include <QIcon>

//#define TEST
#ifdef TEST
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QInputMethodEvent>

static void sendInputText(const QString &text)
{
    QInputMethodEvent *event = new QInputMethodEvent;
    event->setCommitString(text);
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
    QObject::connect(XYVirtualKeyboard::getInstance(), &XYVirtualKeyboard::send_commit, &sendInputText);
#endif

    a.setQuitOnLastWindowClosed(true);
    a.setWindowIcon(QIcon(":/Keyboard.ico"));
    return a.exec();
}
