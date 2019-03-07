#ifndef XYDRAGWIDGET_H
#define XYDRAGWIDGET_H

#include <QWidget>

class XYDragableWidget : public QWidget
{
    Q_OBJECT
public:
    enum DIRECTION{HORIZONTAL, VERTICAL};
    explicit XYDragableWidget(QWidget *centerWidget, DIRECTION dire, QWidget *parent = 0);

    int getMouseSensitivity() const;
    void setMouseSensitivity(int value);

protected:
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

private:
    bool      filterObjsPressed;
    QPoint    filterObjsLastPressedPoint;
    QPoint    filterObjscurPressedPoint;

private:
    int        mouseSensitivity; // 拖动灵敏度
    QWidget   *centerWidget;     // 中心拖拽控件
    DIRECTION  direction;        // 拖拽类型
};

#endif // XYDRAGWIDGET_H
