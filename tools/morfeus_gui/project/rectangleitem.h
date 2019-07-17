#ifndef RECTANGLEITEM_H
#define RECTANGLEITEM_H

#include "projectitem.h"

class Rectangle;

class RectangleItem
    : public ProjectItem
{
  Q_OBJECT

public:
  RectangleItem(std::unique_ptr<Rectangle> rectangle);

public:
  const Rectangle * rectangle() const;
  Rectangle * rectangle();
  std::unique_ptr<Rectangle> removeRectangle();

protected:
  bool loadAttributes(QXmlStreamReader & reader) override;
  bool saveAttributes(QXmlStreamWriter & writer) const override;

protected slots:
  void handleItemChanged();

private:
  std::unique_ptr<Rectangle> mRectangle;
};

inline const Rectangle * RectangleItem::rectangle() const
{
  return mRectangle.get();
}

inline Rectangle * RectangleItem::rectangle()
{
  return mRectangle.get();
}

#endif // RECTANGLEITEM_H
