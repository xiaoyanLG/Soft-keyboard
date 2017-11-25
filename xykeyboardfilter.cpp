#include "xykeyboardfilter.h"
#include "xyvirtualkeyboard.h"

XYKeyBoardFilter *XYKeyBoardFilter::instance = NULL;
XYKeyBoardFilter *XYKeyBoardFilter::getInstance()
{
    if (instance == NULL)
    {
        instance = new XYKeyBoardFilter;
    }
    return instance;
}

XYKeyBoardFilter::XYKeyBoardFilter()
{

}

bool XYKeyBoardFilter::filter(int unicode, int keycode, int modifiers, bool isPress, bool autoRepeat)
{
    if (isPress && modifiers == Qt::ControlModifier && keycode == Qt::Key_Space)
    {
        XYVirtualKeyboard::getInstance()->switchLanguage();
    }
    else
    {
        XYVirtualKeyboard::getInstance()->keyClicked(unicode, keycode, (Qt::KeyboardModifiers)modifiers, isPress);
    }
    return true;
}

void XYKeyBoardFilter::postEvent(int unicode, int keycode, int modifiers, bool isPress)
{
//    QWSServer::sendKeyEvent(unicode, keycode, (Qt::KeyboardModifiers)modifiers, isPress, false);
}

