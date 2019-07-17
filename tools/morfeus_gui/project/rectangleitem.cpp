#include "rectangleitem.h"

#include "rectangle.h"

RectangleItem::RectangleItem(std::unique_ptr<Rectangle> rectangle)
  : ProjectItem ("Rectangle")
  , mRectangle(std::move(rectangle))
{
  setName(QString::fromStdString(mRectangle->name()));
  connect(this, SIGNAL(itemChanged()), SLOT(handleItemChanged()));
}

void RectangleItem::handleItemChanged()
{
  mRectangle->setName(name().toStdString());
}

bool RectangleItem::loadAttributes(QXmlStreamReader & reader)
{
  return !reader.hasError();
}

std::unique_ptr<Rectangle> RectangleItem::removeRectangle()
{
  return std::move(mRectangle);
}

bool RectangleItem::saveAttributes(QXmlStreamWriter & writer) const
{
  if (mRectangle.get() != nullptr) {
    writer.writeAttribute("x", QString::number(mRectangle->center().x()));
    writer.writeAttribute("y", QString::number(mRectangle->center().y()));
    writer.writeAttribute("width", QString::number(mRectangle->width()));
    writer.writeAttribute("height", QString::number(mRectangle->height()));
  }

  return !writer.hasError();
}
