#include "surfaceitem.h"

SurfaceItem::SurfaceItem()
  : ProjectItem("Surface")
{

}

bool SurfaceItem::loadAttributes(QXmlStreamReader & reader)
{
  return !reader.hasError();
}

bool SurfaceItem::saveAttributes(QXmlStreamWriter & writer) const
{
  return !writer.hasError();
}
