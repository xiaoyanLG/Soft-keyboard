#ifndef XYINPUTPLUGIN_H
#define XYINPUTPLUGIN_H

#include <qpa/qplatforminputcontextplugin_p.h>
#include <QStringList>

class XYInputPlugin : public QPlatformInputContextPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QPlatformInputContextFactoryInterface_iid FILE "xyinput.json")

public:
    QPlatformInputContext *create(const QString&, const QStringList&);
};

#endif // XYINPUTPLUGIN_H
