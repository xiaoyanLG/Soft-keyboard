#ifndef XYHDRAGABLETRANSLATEVIEW_H
#define XYHDRAGABLETRANSLATEVIEW_H

#include "xyqstringview.h"

class XYHDragableTranslateView : public XYQStringView
{
    Q_OBJECT
public:
    explicit XYHDragableTranslateView(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);

    friend class XYVirtualKeyboard;
};

#endif // XYHDRAGABLETRANSLATEVIEW_H
