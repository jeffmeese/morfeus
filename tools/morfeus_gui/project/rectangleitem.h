#ifndef RECTANGLEITEM_H
#define RECTANGLEITEM_H

#include "projectitem.h"

#include <model/geometry/rectangle.h>

class RectangleItem
    : public ProjectItem
{
  Q_OBJECT

public:
  RectangleItem(std::unique_ptr<morfeus::model::geometry::Rectangle> rectangle);

public:
  const morfeus::model::geometry::Rectangle * rectangle() const;
  morfeus::model::geometry::Rectangle * rectangle();
  std::unique_ptr<morfeus::model::geometry::Rectangle> removeRectangle();

protected:
  bool loadAttributes(QXmlStreamReader & reader) override;
  bool saveAttributes(QXmlStreamWriter & writer) const override;

protected slots:
  void handleItemChanged();

private:
  std::unique_ptr<morfeus::model::geometry::Rectangle> mRectangle;
};

inline const morfeus::model::geometry::Rectangle * RectangleItem::rectangle() const
{
  return mRectangle.get();
}

inline morfeus::model::geometry::Rectangle * RectangleItem::rectangle()
{
  return mRectangle.get();
}

#endif // RECTANGLEITEM_H
