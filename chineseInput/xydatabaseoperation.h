#ifndef XYDATABASEOPERATION_H
#define XYDATABASEOPERATION_H

#include <QObject>
#include <QSqlDatabase>
#include <QList>

class XYTranslateItem;
class XYDatabaseOperation : public QObject
{
    Q_OBJECT
public:
    static XYDatabaseOperation *getInstance();
    explicit XYDatabaseOperation(QObject *parent = Q_NULLPTR);
    ~XYDatabaseOperation();

    bool createDatabaseFile(const QString &filePath, const QString &passwd = "", bool fource = false);
    bool openDatabaseFile(const QString &filePath, const QString &passwd = "");

    bool createInputTable();
    bool insertData(XYTranslateItem *item, const QString &table);
    bool insertData(const QList<XYTranslateItem *> &list, const QString &table);
    bool delItem(XYTranslateItem *item);
    QList<XYTranslateItem *> findData(const QString &key,
                                      const QString &number,
                                      const QString &table,
                                      bool *haveFind = Q_NULLPTR, int max = 200);

private:
    static XYDatabaseOperation *DB;

};

#define XYDB XYDatabaseOperation::getInstance()
#endif // XYDATABASEOPERATION_H
