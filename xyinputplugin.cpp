#include "xyinputplugin.h"
#include "xyplatforminputcontext.h"
#include <QDebug>

QPlatformInputContext *XYInputPlugin::create(const QString &system, const QStringList &paramList)
{
    qDebug() << Q_FUNC_INFO << system << paramList;
    if (system.compare(QLatin1String("xyinput"), Qt::CaseInsensitive) == 0) {
        return new XYPlatformInputContext();
    }
    return nullptr;
}
