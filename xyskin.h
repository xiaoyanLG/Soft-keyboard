#ifndef XYSKIN_H
#define XYSKIN_H

#include <QObject>
#include <QList>
#include <QColor>
#include <QPixmap>
#include <QFont>
#include <QPen>
#include <QDomElement>

struct KeyButtonStyle{
    KeyButtonStyle()
        :key(0), pressed(0)
    {}
    KeyButtonStyle(const KeyButtonStyle *other)
    {
        this->key = other->key;
        this->pressed = other->pressed;
        this->textFont = other->textFont;
        this->backgroundColor = other->backgroundColor;
        this->textPen = other->textPen;
        this->borderPen = other->borderPen;
        this->borderFont = other->borderFont;
        this->backImage = other->backImage;
    }

    int    key;            // 按钮键值
    int    pressed;        // 是否按下（0表示没有按下，非0表示按下）
    QFont  textFont;       // 文字字体
    QColor backgroundColor;// 背景颜色
    QPen   textPen;        // 文字画笔
    QPen   borderPen;      // 边框画笔
    QFont  borderFont;     // 边框字体
    QPixmap backImage;     // 背景图片
};

class XYSkin : public QObject
{
    Q_OBJECT
public:
    static XYSkin *getInstance();
    ~XYSkin();
    KeyButtonStyle *getStyleByKey(int key, bool pressed);
    void initWithNoSkinFile();
    bool loadSkinWithFile(const QString &filePath);
    bool writeSkinToFile(const QString &filePath);

signals:
    void skinChanged();

private:
    void loadIntStyle(const QDomElement &parent, int &value);
    void loadPixmapStyle(const QDomElement &parent, QPixmap &pixmap);
    void loadColorStyle(const QDomElement &parent, QColor &color);
    void loadFontStyle(const QDomElement &parent, QFont &font);
    void loadPenStyle(const QDomElement &parent, QPen &pen);
    void loadButtonStyle(const QDomElement &parent, KeyButtonStyle *btn);

    void writeIntStyle(QDomDocument &doc,QDomElement &parent,
                          int key,
                          const QString &title);
    void writePixmapStyle(QDomDocument &doc,QDomElement &parent,
                          const QPixmap &pixmap,
                          const QString &title);
    void writeColorStyle(QDomDocument &doc,QDomElement &parent,
                          const QColor &color,
                          const QString &title,
                          bool isAttribute = false);
    void writeFontStyle(QDomDocument &doc,QDomElement &parent,
                          const QFont &font,
                          const QString &title);
    void writePenStyle(QDomDocument &doc,QDomElement &parent,
                          const QPen &pen,
                          const QString &title);
    void writeButtonStyle(QDomDocument &doc,
                          QDomElement &parent,
                          KeyButtonStyle *btn,
                          const QString &title);

public:
    KeyButtonStyle          *defaultButtonStyle1;   // 默认按钮1风格
    KeyButtonStyle          *defaultButtonStyle1_pressed;   // 默认按钮1按下时风格
    KeyButtonStyle          *defaultButtonStyle2;           // 默认按钮2风格
    KeyButtonStyle          *defaultButtonStyle2_pressed;   // 默认按钮2按下时风格
    KeyButtonStyle          *popupButtonStyle;      // 弹出框风格，如果没有使用默认
    QList<KeyButtonStyle *> userDefButtonsStyle;    // 自定义按钮风格
    QColor                  topColor;               // 候选词背景色
    QPixmap                 topBKPixmap;            // 候选词背景图片
    QPen                    trianglePen;            // 下拉按钮图标画笔
    QColor                  trianglePressedBKColor; // 下拉按钮按下时的颜色
    QPixmap                 triangleBKPixmap;       // 下拉按钮图标
    QPixmap                 triangleBKPressedPixmap;// 下拉按钮按下时图标
    QColor                  bottomColor1;           // 底部显示按钮时的颜色
    QColor                  bottomColor2;           // 底部显示滚动窗口时候的颜色
    QPixmap                 bottomBKPixmap;         // 按钮背景图片
    QPen                    borderPen;              // 边框画笔
    QColor                  inputLettersColor;      // 用户输入字母的颜色
    QFont                   inputLettersFont;       // 用户输入字母的字体
    QFont                   candidateTextFont;      // 候选词的字体
    QPen                    candidateTextPen;       // 候选词的画笔
    QColor                  candidateTextPressedBKColor;  // 候选词按下时候的颜色

private:
    explicit XYSkin(QObject *parent = 0);
    static XYSkin *instance;
};

#define XYSKIN XYSkin::getInstance()
#endif // XYSKIN_H
