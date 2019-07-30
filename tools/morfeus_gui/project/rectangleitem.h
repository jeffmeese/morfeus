#ifndef RECTANGLEITEM_H
#define RECTANGLEITEM_H

#include "projectitem.h"

#include "geometry/rectangle.h"

class RectangleItem
    : public ProjectItem
{
  Q_OBJECT

public:
  RectangleItem(std::unique_ptr<Morfeus::Geometry::Rectangle> rectangle);

public:
  const Morfeus::Geometry::Rectangle * rectangle() const;
  Morfeus::Geometry::Rectangle * rectangle();
  std::unique_ptr<Morfeus::Geometry::Rectangle> removeRectangle();

protected:
  bool loadAttributes(QXmlStreamReader & reader) override;
  bool saveAttributes(QXmlStreamWriter & writer) const override;

protected slots:
  void handleItemChanged();

private:
  std::unique_ptr<Morfeus::Geometry::Rectangle> mRectangle;
};

inline const Morfeus::Geometry::Rectangle * RectangleItem::rectangle() const
{
  return mRectangle.get();
}

inline Morfeus::Geometry::Rectangle * RectangleItem::rectangle()
{
  return mRectangle.get();
}

#endif // RECTANGLEITEM_H
