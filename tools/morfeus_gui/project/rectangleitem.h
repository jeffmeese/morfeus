#ifndef RECTANGLEITEM_H
#define RECTANGLEITEM_H

#include "projectitem.h"

class Rectangle;

class RectangleItem
    : public ProjectItem
{
  Q_OBJECT

public:
  RectangleItem();
  RectangleItem(Rectangle * rectangle);

public:
  void setRectangle(Rectangle * rectangle);

protected:
  bool loadAttributes(QXmlStreamReader & reader) override;
  bool saveAttributes(QXmlStreamWriter & writer) const override;

protected slots:
  void handleItemChanged();

private:
  Rectangle * mRectangle;
};

#endif // RECTANGLEITEM_H
