#ifndef XYINPUTSEARCHINTERFACE_H
#define XYINPUTSEARCHINTERFACE_H

#include <QObject>
#include <QStringList>
#include <QMap>
#include "xytranslateitem.h"

class XYInputSearchInterface : public QObject
{
    Q_OBJECT
public:
    static XYInputSearchInterface *getInstance();
    ~XYInputSearchInterface();
    bool initInputBase(const QString &path);
    void resetSearch();
    QString getCurLetters();

signals:

public slots:
    QList<XYTranslateItem *> &searchTranslates(const QString &keyword);
    void setChinese(bool ch);

    // 分割拼音，比如women-->wo%'men,返回分割后的字符串(并加上%模糊查找)，并带回有效的字数
    QString splitePinyin(const QString &pinyin, int &num);
    void deDuplication(QList<XYTranslateItem *> &items, bool del = true); // 删除重复的字词
    XYTranslateItem *autoCreateWords(const QString &keyword);  // 自动造词
    QList<XYTranslateItem *> findItemsFromTemp(const QString &keyword, bool force = true);
    QList<XYTranslateItem *> findPossibleMust(const QString &keyword, int max = 200);
    void saveItem(XYTranslateItem *item);
    void clearTemp();
    const QStringList &getYunMuByShengMu(const QChar &shenmu);
    QList<XYTranslateItem *> &completeInput(const QString &text, int index, QString &showText);
    QList<XYTranslateItem *> &completeInput(const QString &text, QString &showText, XYTranslateItem *item = Q_NULLPTR);

private:
    explicit XYInputSearchInterface(QObject *parent = Q_NULLPTR);
    static XYInputSearchInterface *instance;
    bool             mbEnglish;
    XYTranslateItem  moCompleteItem;
    XYTranslateItem  moAutoCompleteItem;
    QString          msCurrentKeyWords;
    QList<XYTranslateItem *> curSearchedTranslates;
    QMap<QString, QList<XYTranslateItem *> > mmapTempItems; // 存储一次输入中查询过的词组

    friend class XYVirtualKeyboard;
};

#endif // XYINPUTSEARCHINTERFACE_H
