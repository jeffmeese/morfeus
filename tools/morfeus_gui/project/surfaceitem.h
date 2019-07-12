#ifndef SURFACEITEM_H
#define SURFACEITEM_H

#include "projectitem.h"

class SurfaceItem
    : public ProjectItem
{
public:
  SurfaceItem();

protected:
  bool loadAttributes(QXmlStreamReader & reader) override;
  bool saveAttributes(QXmlStreamWriter & writer) const override;
};

#endif // SURFACEITEM_H
