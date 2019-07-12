#include "rectangleitem.h"

#include "rectangle.h"

RectangleItem::RectangleItem()
  : ProjectItem ("Rectangle")
  , mRectangle(nullptr)
{
}

RectangleItem::RectangleItem(Rectangle * rectangle)
  : ProjectItem ("Rectangle")
  , mRectangle(nullptr)
{
  setRectangle(rectangle);
}

void RectangleItem::handleItemChanged()
{
  mRectangle->setName(name().toStdString());
}

bool RectangleItem::loadAttributes(QXmlStreamReader & reader)
{
  return !reader.hasError();
}

bool RectangleItem::saveAttributes(QXmlStreamWriter & writer) const
{
  return !writer.hasError();
}

void RectangleItem::setRectangle(Rectangle *rectangle)
{
  if (mRectangle != nullptr) {
    disconnect(nullptr, nullptr, this, nullptr);
  }

  mRectangle = rectangle;
  if (mRectangle != nullptr) {
    setName(QString::fromStdString(mRectangle->name()));
    connect(this, SIGNAL(itemChanged()), SLOT(handleItemChanged()));
  }
}
