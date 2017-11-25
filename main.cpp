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
#endif

    a.setQuitOnLastWindowClosed(true);
    a.setWindowIcon(QIcon(":/Keyboard.ico"));
    return a.exec();
}
