#ifndef XYQSTRINGVIEW_H
#define XYQSTRINGVIEW_H

#include <QWidget>

class XYQStringView : public QWidget
{
    Q_OBJECT
public:
    explicit XYQStringView(QWidget *parent = 0);

    int getUnitMinWidth() const;
    void setUnitMinWidth(int value);

    int getUnitMinHeight() const;
    void setUnitMinHeight(int value);

signals:
    void stringPressed(const QString &text, const QPoint &pos);
    void clicked(const QString &text, int index);

protected:
    bool event(QEvent *event);

private:
    int findcontainsMouseRect(const QPoint &mousePos, QRect &rect);

protected:
    QList<QRect> dataRects;
    QStringList  dataStrings;

    int          unitMinWidth;
    int          unitMinHeight;
    bool         pressed;
    QRect        pressRect;

};

#endif // XYQSTRINGVIEW_H
