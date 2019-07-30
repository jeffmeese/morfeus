#ifndef RECTANGLEITEM_H
#define RECTANGLEITEM_H

#include "projectitem.h"

#include "geometry/rectangle.h"

class RectangleItem
    : public ProjectItem
{
  Q_OBJECT

public:
  RectangleItem(std::unique_ptr<morfeus::geometry::Rectangle> rectangle);

public:
  const morfeus::geometry::Rectangle * rectangle() const;
  morfeus::geometry::Rectangle * rectangle();
  std::unique_ptr<morfeus::geometry::Rectangle> removeRectangle();

protected:
  bool loadAttributes(QXmlStreamReader & reader) override;
  bool saveAttributes(QXmlStreamWriter & writer) const override;

protected slots:
  void handleItemChanged();

private:
  std::unique_ptr<morfeus::geometry::Rectangle> mRectangle;
};

inline const morfeus::geometry::Rectangle * RectangleItem::rectangle() const
{
  return mRectangle.get();
}

inline morfeus::geometry::Rectangle * RectangleItem::rectangle()
{
  return mRectangle.get();
}

#endif // RECTANGLEITEM_H
