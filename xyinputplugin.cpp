#include "xyinputplugin.h"
#include "xyplatforminputcontext.h"
#include <QDebug>

QPlatformInputContext *XYInputPlugin::create(const QString &system, const QStringList &paramList)
{
    if (system.compare(QLatin1String("xyinput"), Qt::CaseInsensitive) == 0) {
        return new XYPlatformInputContext();
    }
    return Q_NULLPTR;
}
