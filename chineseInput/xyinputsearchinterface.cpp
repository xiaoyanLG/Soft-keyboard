#include "xyinputsearchinterface.h"
#include "xydatabaseoperation.h"
#include <QDebug>
#include <QFile>

XYInputSearchInterface *XYInputSearchInterface::instance = Q_NULLPTR;
XYInputSearchInterface *XYInputSearchInterface::getInstance()
{
    if (instance == Q_NULLPTR)
    {
        instance = new XYInputSearchInterface;
    }
    return instance;
}

XYInputSearchInterface::XYInputSearchInterface(QObject *parent)
    : QObject(parent), mbEnglish(false)
{

}

XYInputSearchInterface::~XYInputSearchInterface()
{

}

bool XYInputSearchInterface::initInputBase(const QString &path)
{
    return XYDB->openDatabaseFile(path);
}

void XYInputSearchInterface::resetSearch()
{
    moCompleteItem.clear();
    moAutoCompleteItem.clear();
    msCurrentKeyWords.clear();
    curSearchedTranslates.clear();
    auto it = mmapTempItems.begin();
    while (mmapTempItems.end() != it)
    {
        qDeleteAll(it.value());
        it++;
    }
    mmapTempItems.clear();
}

QString XYInputSearchInterface::getCurLetters()
{
    QString letters = msCurrentKeyWords;
    return letters.replace("%", "");
}

QList<XYTranslateItem *> &XYInputSearchInterface::searchTranslates(const QString &keyword)
{
    QList<XYTranslateItem *> list;

    if (keyword.trimmed().isEmpty()) // 如果传入的词为空了，代表删完了，应该关闭输入窗口
    {
        msCurrentKeyWords.clear();
        curSearchedTranslates.clear();
        return curSearchedTranslates;
    }

    QString splitePY;
    bool find_new = false;
    if (mbEnglish)
    {
        splitePY = keyword;
        list = findItemsFromTemp(splitePY, false);
        if (list.isEmpty())
        {
            find_new = true;
            list = XYDB->findData(splitePY + "%", "", "userEnglishTable");
            list += XYDB->findData(splitePY + "%", "", "englishTable");
            deDuplication(list, true);
        }
    }
    else
    {
        int num = 0;
        splitePY = splitePinyin(keyword, num);
        splitePY.replace("\'", "%\'");
        list = findPossibleMust(splitePY);

        XYTranslateItem *autoTranslate = autoCreateWords(splitePY); // 智能造句
        if (autoTranslate)
        {
            list.prepend(autoTranslate);
        }
    }

    msCurrentKeyWords = splitePY;

    curSearchedTranslates = list;
    return curSearchedTranslates;
}

void XYInputSearchInterface::setChinese(bool ch)
{
    mbEnglish = !ch;
}

QString XYInputSearchInterface::splitePinyin(const QString &pinyin, int &num)
{
    static QString shenmu = "bpmfdtnlgkhjqxzcsywr";
    static QStringList zcs = QString("z c s").split(" ");
    static QStringList yunmuA = QString("a o e ai ao ou ei er an ang en eng").split(" ");

    QString result;

    if (pinyin.contains("\'"))
    {
        bool endfgf = pinyin.endsWith("\'");
        QStringList children = pinyin.split("\'", QString::SkipEmptyParts);
        for (int i = 0; i < children.size(); ++i)
        {
            int cur_num = 0;
            if (!result.isEmpty() && !result.endsWith("%\'")) // 每次进入一定的新的字的拼音
            {
                result += "%\'";
            }
            result += splitePinyin(children.at(i), cur_num);
            num += cur_num;
        }
        if (endfgf)
        {
            result += "\'";
        }
        return result;
    }
    int cur_index = 0;
    while (cur_index < pinyin.size())
    {
        if (shenmu.contains(pinyin.at(cur_index))) // 是声母
        {
            int ym = 1;
            int h = 0; // zh ch sh标记
            // zh ch sh 多加一位
            if (zcs.contains(pinyin.at(cur_index)) && cur_index + 1 < pinyin.size() && pinyin.at(cur_index + 1) == 'h')
            {
                h = 1;
                ym++;
            }

            const QStringList &yunmus = getYunMuByShengMu(pinyin.at(cur_index));

            // 贪心查找 （尽可能长的找到满足的）
            while ( (ym + cur_index) < pinyin.size() )
            {
                bool find = false;
                for (int i = 0; i < yunmus.size(); ++i)
                {
                    QString c_py = yunmus.at(i);
                    if (c_py.startsWith(pinyin.mid(cur_index + 1 + h, ym - h)))
                    {
                        find = true;
                    }
                }
                if (!find)
                {
                    if (ym > h)
                    {
                        ym--;
                    }
                    break;
                }
                ym++;
            }

            if (!result.isEmpty() && !result.endsWith("%\'"))
            {
                result += "%\'";
            }
            result += pinyin.mid(cur_index, ym + 1);
            cur_index += ym;
        }
        else
        {
            if ( result.size() > 1 &&
                    (result.endsWith("g") // 如果是特殊的几个韵母结束的，到这里应该截取下来，重新匹配
                    || result.endsWith("n")
                    || result.endsWith("r")) )
            {
                int last_index = result.lastIndexOf("%\'");
                QString last;
                if (last_index != -1)
                {
                    last = result.mid(last_index + 2);
                }
                else
                {
                    last = result;
                }
                last.remove(last.size() - 1, 1);
                if (!last.isEmpty() && getYunMuByShengMu(last.at(0)).contains(last.mid(1))) // 判断截取之后是否是可以组合的拼音
                {
                    result.remove(result.size() - 1, 1);
                    cur_index -= 1;
                    continue;
                }
            }

            if (yunmuA.contains(pinyin.at(cur_index)))
            {
                int ym = 0;
                while ((ym + cur_index) < pinyin.size())
                {
                    bool find = false;
                    for (int i = 0; i < yunmuA.size(); ++i)
                    {
                        QString c_py = yunmuA.at(i);
                        if (c_py.startsWith(pinyin.mid(cur_index, ym + 1)))
                        {
                            find = true;
                        }
                    }
                    if (find)
                    {
                        ym++;
                    }
                    else
                    {
                        break;
                    }
                }

                if (!result.isEmpty() && !result.endsWith("%\'"))
                {
                    result += "%\'";
                }
                result += pinyin.mid(cur_index, ym);
                cur_index += ym - 1;
            }
            else
            {
                if (!result.isEmpty() && !result.endsWith("%\'"))
                {
                    result += "%\'";
                }
                result += pinyin.at(cur_index);
            }
        }

        num++;
        cur_index++;
    }
    if (num == 0)
    {
        num++;
    }

    return result;
}

void XYInputSearchInterface::deDuplication(QList<XYTranslateItem *> &items, bool del)
{
    QList<XYTranslateItem *> temp;
    for (int i = 0; i < items.size(); ++i)
    {
        XYTranslateItem *item = items.at(i);
        bool find = false;
        for (int j = 0; j < temp.size(); ++j)
        {
            if (mbEnglish)
            {
                if (item->msComplete == temp.at(j)->msComplete)
                {
                    find = true;
                    if (del)
                    {
                        delete item;
                    }
                    break;
                }
            }
            else
            {
                if (item->msTranslate == temp.at(j)->msTranslate)
                {
                    find = true;
                    if (del)
                    {
                        delete item;
                    }
                    break;
                }
            }
        }
        if (!find)
        {
            temp.append(item);
        }
    }
    items = temp;
}

XYTranslateItem *XYInputSearchInterface::autoCreateWords(const QString &keyword)
{
    moAutoCompleteItem.clear();
    QString exists = keyword;
    QMap<QString, QList<XYTranslateItem *> >::iterator it = mmapTempItems.find(exists);
    while(it == mmapTempItems.end() || it.value().isEmpty())
    {
        if (exists.contains("%\'"))
        {
            exists = exists.mid(0, exists.lastIndexOf("%\'"));
        }
        else
        {
            break;
        }

        it = mmapTempItems.find(exists);
    }

    if (exists == keyword || it == mmapTempItems.end() || it.value().isEmpty())
    {
        return Q_NULLPTR;
    }

    XYTranslateItem *comAll = &moAutoCompleteItem;
    comAll->msComplete = it.value().at(0)->msComplete;
    comAll->msTranslate = it.value().at(0)->msTranslate;
    QString keys = keyword.mid(exists.size());
    if (keys.startsWith("%\'"))
    {
        keys.remove(0, 2);
    }

    int k_nums = keys.split("%\'", QString::SkipEmptyParts).size();
    int f_nums = 0;
    while (k_nums > f_nums)
    {
        for (int i = 0; i < f_nums; ++i)
        {
            if (!keys.contains("%\'"))
            {
                keys.clear();
            }
            else
            {
                keys = keys.mid(keys.indexOf("%\'") + 2);
            }
        }
        if (keys.isEmpty())
        {
            break;
        }

        QList<XYTranslateItem *> list = findPossibleMust(keys);
        if (!list.isEmpty())
        {
            comAll->msComplete += "\'";
            comAll->msComplete += list.at(0)->msComplete;

            comAll->msTranslate += list.at(0)->msTranslate;

            f_nums = list.at(0)->msComplete.split("\'").size();
        }
        else
        {
            break;
        }
    }

    return &moAutoCompleteItem;
}

QList<XYTranslateItem *> XYInputSearchInterface::findItemsFromTemp(const QString &keyword, bool force)
{
    QList<XYTranslateItem *> list;
    if (force || mmapTempItems.find(keyword) != mmapTempItems.end())
    {
        QString delsuf = keyword.mid(0, keyword.lastIndexOf("%"));
        QMap<QString, QList<XYTranslateItem *> >::iterator it = mmapTempItems.begin();
        while (mmapTempItems.end() != it)
        {
            QString last_key = it.key();
            if (delsuf.startsWith(last_key))
            {
                list = it.value() + list;
            }
            it++;
        }
    }

    return list;
}

QList<XYTranslateItem *> XYInputSearchInterface::findPossibleMust(const QString &keyword, int max)
{
    QStringList words = keyword.split("%\'");
    QList<XYTranslateItem *> results;
    QString key;
    for (int i = 0; i < words.size(); ++i)
    {
        if (!key.isEmpty())
        {
            key += "%\'";
        }
        key += words.at(i);

        QList<XYTranslateItem *> list;
        QMap<QString, QList<XYTranslateItem *> >::iterator it = mmapTempItems.find(key);
        bool find = false;
        if (it != mmapTempItems.end())
        {
            find = true;
            list = it.value();
        }

        if (!find)
        {
            bool haveFind = false;
            list = XYDB->findData(key + "%", QString::number(i + 1), "userPinyin", &haveFind, max);
            if (i == 0)
            {
                QList<XYTranslateItem *> single = XYDB->findData(key + "%", "", "singlePinyin", &haveFind, max);

                for (int i = 0; i < single.size(); ++i)
                {
                    XYTranslateItem *singleItem = single.at(i);
                    QStringList singles = singleItem->msTranslate.split(" ", QString::SkipEmptyParts);
                    for (int j = 0; j < singles.size(); ++j)
                    {
                        if (list.size() > 200)
                        {
                            break;
                        }
                        list.append(new XYTranslateItem("singlePinyin", singles.at(j), singleItem->msComplete));
                    }
                }
                qDeleteAll(single);
            }
            list += XYDB->findData(key + "%", QString::number(i + 1), "basePinyin", &haveFind, max);

            deDuplication(list, true);
            if (haveFind)
            {
                mmapTempItems.insert(key, list);
            }
        }
        if (!list.isEmpty())
        {
            results = list + results;
        }
    }

    return results;
}

void XYInputSearchInterface::saveItem(XYTranslateItem *item)
{
    if (item && !item->msComplete.isEmpty()) // 保存用户词库
    {
        item->miTimes += 1;
        if (item->msSource.toLower().contains("english"))
        {
            XYDB->insertData(item, "userEnglishTable");
        }
        else
        {
            item->msExtra = QString::number(item->msTranslate.size());
            XYDB->insertData(item, "userPinyin");
        }
    }
}

void XYInputSearchInterface::clearTemp()
{
    QMap<QString, QList<XYTranslateItem *> >::iterator it = mmapTempItems.begin();
    while (mmapTempItems.end() != it)
    {
        qDeleteAll(it.value());
        it++;
    }
    mmapTempItems.clear();
}

const QStringList &XYInputSearchInterface::getYunMuByShengMu(const QChar &shenmu)
{
    static QMap<QChar, QStringList> Allyunmus;
    auto it = Allyunmus.find(shenmu);
    if ( it != Allyunmus.end()) {
        return it.value();
    }
    
    QStringList yunmu;
    switch (shenmu.toLatin1()) // 单独为每一个声母指定可匹配的韵母
    {
    case 'b':
        yunmu = QString("a o ai ei ao an en ang eng i ie iao ian in ing u").split(" ", QString::SkipEmptyParts);
        break;
    case 'p':
        yunmu = QString("a o ai ei ao ou an en ang eng i ie iao ian in ing u").split(" ", QString::SkipEmptyParts);
        break;
    case 'm':
        yunmu = QString("a e o ai ei ao an en ang eng i ie iao iu ian in ing u").split(" ", QString::SkipEmptyParts);
        break;
    case 'f':
        yunmu = QString("a o ei ou an en ang eng u").split(" ", QString::SkipEmptyParts);
        break;
    case 'd':
        yunmu = QString("a o ai ei ao ou an en ang eng i ia ie iao ian iu ian ing u uo ui uan un ong").split(" ", QString::SkipEmptyParts);
        break;
    case 't':
        yunmu = QString("a e ai ei ao ou an ang eng i ie iao ian ing u uo ui uan un ong").split(" ", QString::SkipEmptyParts);
        break;
    case 'n':
    case 'l':
        yunmu = QString("a e o ai ei ao ou an en ang eng i ia ie iao ian iu ian in ing u uo ui uan un ong v ve").split(" ", QString::SkipEmptyParts);
        break;
    case 'g':
    case 'k':
    case 'h':
        yunmu = QString("a e ai ei ao ou an en ang eng u ua uo uai ui uan un uang ong").split(" ", QString::SkipEmptyParts);
        break;
    case 'j':
    case 'q':
    case 'x':
        yunmu = QString("i ia ie iao iu ian in iang ing v u ue ve van uan un vn iong").split(" ", QString::SkipEmptyParts);
        break;
    case 'z':
        yunmu = QString("a e i ai ei ao ou an en ang eng u ua uo uai ui uan un uang ong").split(" ", QString::SkipEmptyParts);
        break;
    case 'c':
        yunmu = QString("a e i ai ao ou an en ang eng u ua uo uai ui uan un uang ong").split(" ", QString::SkipEmptyParts);
        break;
    case 's':
        yunmu = QString("a e i ai ei ao ou an en ang eng u ua uo uai ui uan un uang ong").split(" ", QString::SkipEmptyParts);
        break;
    case 'r':
        yunmu = QString("e i ao ou an en ang eng u ua uo ui uan un uang ong").split(" ", QString::SkipEmptyParts);
        break;
    case 'y':
        yunmu = QString("a e i o ao ou an in ang ing u ue uan un ong").split(" ", QString::SkipEmptyParts);
        break;
    case 'w':
        yunmu = QString("a o ai ei an en ang eng u").split(" ", QString::SkipEmptyParts);
        break;
    default:
        break;
    }

    it = Allyunmus.insert(shenmu, yunmu);
    return it.value();
}

QList<XYTranslateItem *> &XYInputSearchInterface::completeInput(const QString &text, int index, QString &showText)
{
    if (index < curSearchedTranslates.size())
    {
        completeInput(text, showText, curSearchedTranslates.at(index));
    }
    else
    {
        curSearchedTranslates.clear();
    }
    return curSearchedTranslates;
}

QList<XYTranslateItem *> &XYInputSearchInterface::completeInput(const QString &text, QString &showText, XYTranslateItem *item)
{
    if (!text.isEmpty()) // 如果为空直接退出
    {
        if (!mbEnglish && item)
        {
            if (!moCompleteItem.msComplete.isEmpty())
            {
                moCompleteItem.msComplete += "\'";
            }
            moCompleteItem.msComplete += item->msComplete;
            moCompleteItem.msTranslate += item->msTranslate;
            QStringList all = msCurrentKeyWords.split("%\'", QString::SkipEmptyParts);
            int com = moCompleteItem.msComplete.split("\'", QString::SkipEmptyParts).size();
            int remain = all.size() - com;
            if (remain > 0)
            {
                QString key;
                for (int i = com; i < all.size(); ++i)
                {
                    if (!key.isEmpty())
                    {
                        key += "%\'";
                    }
                    key += all.at(i);
                }
                saveItem(item);
                curSearchedTranslates = findPossibleMust(key);
                showText = moCompleteItem.msTranslate + key.replace("%", "");
            }
            else
            {
                if (moCompleteItem.msTranslate == item->msTranslate)
                {
                    moCompleteItem.miTimes = item->miTimes + 1;
                }
                else
                {
                    saveItem(item);
                }

                moCompleteItem.msExtra = QString::number(moCompleteItem.msTranslate.size());
                XYDB->insertData(&moCompleteItem, "userPinyin");
                curSearchedTranslates.clear();
            }
        }
        else
        {
            saveItem(item);
            if (mbEnglish && item == Q_NULLPTR)
            {
                XYTranslateItem *temp = new XYTranslateItem;
                temp->msComplete = text;
                XYDB->insertData(temp, "userEnglishTable");
                delete temp;
            }
            curSearchedTranslates.clear();
        }
    }
    return curSearchedTranslates;
}

