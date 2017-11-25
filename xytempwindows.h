#ifndef XYTEMPWINDOWS_H
#define XYTEMPWINDOWS_H

#include <QWidget>

class XYTempWindows : public QWidget
{
    Q_OBJECT
public:
    enum DIRECTION{BOTTOM, RIGHT, LEFT, TOP};
    explicit XYTempWindows(DIRECTION direction = BOTTOM, QWidget *parent = 0);

    QString getText() const;
    void setText(const QString &value);

    DIRECTION getDirection() const;
    void setDirection(const DIRECTION &value);

    int getKey() const;
    void setKey(int value);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPainterPath getPainterPath(const QRect &rect, DIRECTION direction);

private:
    DIRECTION direction;
    QRect     textRect;

    int       key;
    QString   text;

};

#endif // XYTEMPWINDOWS_H
