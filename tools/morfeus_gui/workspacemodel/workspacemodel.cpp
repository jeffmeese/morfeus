#include "workspacemodel.h"

#include "rectangle.h"
#include "rectangleitem.h"
#include "rectangleworkspaceitem.h"

WorkspaceModel::WorkspaceModel()
{
  Rectangle * rectangle = new Rectangle;
  RectangleItem * rectItem = new RectangleItem;
  rectItem->setRectangle(rectangle);

  RectangleWorkspaceItem * item = new RectangleWorkspaceItem(rectItem);
  invisibleRootItem()->appendRow(item);
}
