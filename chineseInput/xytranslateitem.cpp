#include "xytranslateitem.h"

XYTranslateItem::XYTranslateItem(const QString &source,
                                 const QString &translate,
                                 const QString &complete,
                                 const QString &extra,
                                 int id,
                                 int   times,
                                 bool stick)
    : msSource(source),
      msTranslate(translate),
      msComplete(complete),
      msExtra(extra),
      miID(id),
      miTimes(times),
      mbStick(stick)
{

}

XYTranslateItem::~XYTranslateItem()
{

}

void XYTranslateItem::clear()
{
    msSource.clear();
    msTranslate.clear();
    msComplete.clear();
    msExtra.clear();
    miID = -1;
    miTimes = 1;
    mbStick = false;
}

