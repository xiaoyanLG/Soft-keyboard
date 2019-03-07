#ifndef XYVDRAGABLEERTICALTRANSLATEVIEW_H
#define XYVDRAGABLEERTICALTRANSLATEVIEW_H

#include "xyqstringview.h"

class XYVDragableTranslateView : public XYQStringView
{
    Q_OBJECT
public:
    explicit XYVDragableTranslateView(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);

    friend class XYVirtualKeyboard;
};

#endif // XYVDRAGABLEERTICALTRANSLATEVIEW_H
