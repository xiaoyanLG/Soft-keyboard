#ifndef XYVIRTUALKEYBOARD_H
#define XYVIRTUALKEYBOARD_H

#include <QWidget>
#include <QLabel>

class QStackedWidget;
class XYHDragableTranslateView;
class XYVDragableTranslateView;
class XYPushButton;
class XYDragableWidget;
class pinyin_im;

class XYMovableLabel: public QLabel
{
    Q_OBJECT
public:
    XYMovableLabel(QWidget *parent = Q_NULLPTR);

public slots:
    void setText(const QString &text);
    void clear();

signals:
    void textChanged(const QString &text);

protected:
    bool event(QEvent *event);
};

class XYVirtualKeyboard : public QWidget
{
    Q_OBJECT
public:
    static XYVirtualKeyboard *getInstance();
    void initPinyinDictionary();
    void switchLanguage();

    // 按键被按下
signals:
    void triangleBtnClicked();
    void keyClicked(int unicode, int key, Qt::KeyboardModifiers modifiers, bool press);

public slots:
    bool handleQKeyEvent(QKeyEvent *event);
    bool keyEventHandle(int unicode, int key, Qt::KeyboardModifiers modifiers, bool press);
    void showLetterWidget();
    void showNumberWidget();
    void showSymbols();
    void show();

private slots:
    void languageChanged();
    void caseChanged(bool checked);
    void triangleBtnClickedOP();
    void keyPressed(XYPushButton *key);
    void keyReleaseed(XYPushButton *key);
    void showTempWindow(const QString &text, const QPoint &pos);
    void showTempWindow(XYPushButton *key, bool press);
    void funcClicked(const QString &text, int index);
    void skinChanged();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
    void resizeRequest(QMouseEvent *event);

private:
    enum ResizeType{No, Top, Bottom};
    explicit XYVirtualKeyboard(QWidget *parent = Q_NULLPTR);
    static XYVirtualKeyboard *instance;             // 单例句柄

    QList<XYPushButton *>      allShiftChangeKeys;   // 字母按键
    XYPushButton              *switchLanguageBtn;    // 语言切换按钮
    XYDragableWidget          *translateHDragableWidget; // 水平候选词拖拽控件
    XYDragableWidget          *translateVDragableWidget; // 垂直候选词拖拽控件
    XYDragableWidget          *symbolDragableWidget;     // 符号拖拽控件
    XYDragableWidget          *funcDragableWidget;       // 功能拖拽控件
    QWidget                   *letterWidget;        // 字母控件窗口
    QWidget                   *numberWidget;        // 数字控件窗口
    QStackedWidget            *stackedWidget;       // 用来保存字母/数字控制的栈
    XYMovableLabel            *letterLabel;         // 输入字母显示控件,同时提供对键盘的移动操作
    XYHDragableTranslateView  *translateHView;      // 查询翻译内容展示控件
    XYVDragableTranslateView  *translateVView;      // 查询翻译内容展示控件
    XYVDragableTranslateView  *symbolView;          // 特殊符号展示控件
    XYHDragableTranslateView  *funcHView;           // 功能拖拽
    ResizeType                 resizeType;          // 拉伸类型
    QPoint                     lastResizePos;       // 上次拉伸位置

    QRect                      triangleBtnRect;    // 记录三角按钮的矩形框
    bool                       triangleBtnPressed; // 记录三角按钮是否按下


    // 以下变量及接口用于中文输入
signals:
    void send_preedit(const QString &); // send preedit string to input area
    void send_commit(const QString &); // send commit string to input area

private slots:
    bool a2zkey_clicked(int unicode, int key);
    bool backspace_clicked();
    bool space_clicked();
    bool enter_clicked();
    void search_begin(const QString &keywords);
    void search_closure();
    void clear_history();
    void loadLetterLabel();
    QStringList loadSymbols(const QString &file);
    void symbolSeleted(const QString &text, int index);
    void userSelectChinese(const QString &text, int index);

private:
    pinyin_im                *googlePinyin;            // google拼音接口类
    QString                   alreadyInputLetters;     // 用户输入的所有字母
    QStringList               alreadySelectTranslates; // 用户选择的所有中文词组

};

#endif // XYVIRTUALKEYBOARD_H
