#ifndef XYKEYBOARDFILTER_H
#define XYKEYBOARDFILTER_H

//#include <QWSServer>
#include <QObject>

class KeyboardFilter: public QObject
{

};

//class XYKeyBoardFilter : public QWSServer::KeyboardFilter
class XYKeyBoardFilter : public KeyboardFilter
{
public:
    static XYKeyBoardFilter *getInstance();

    bool filter(int unicode, int keycode, int modifiers, bool isPress, bool autoRepeat);

    void postEvent(int unicode, int keycode, int modifiers, bool isPress);

private:
    explicit XYKeyBoardFilter();
    static XYKeyBoardFilter *instance;
};

#endif // XYKEYBOARDFILTER_H
