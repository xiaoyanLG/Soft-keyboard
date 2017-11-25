#ifndef XYVERTICALTRANSLATEVIEW_H
#define XYVERTICALTRANSLATEVIEW_H

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

#endif // XYVERTICALTRANSLATEVIEW_H
