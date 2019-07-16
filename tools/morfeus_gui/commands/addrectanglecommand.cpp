#include "addrectanglecommand.h"

#include "project.h"
#include "rectangleitem.h"

AddRectangleCommand::AddRectangleCommand()
  : Command("Project.Add.Rectangle")
  , mProjectModified(false)
  , mProject(nullptr)
  , mRectangle(nullptr)
  , mParentItem(nullptr)
{
  setText("Add Rectangle");
}

void AddRectangleCommand::setProject(Project * project)
{
  mProject = project;
}

void AddRectangleCommand::setRectangle(Rectangle * rectangle)
{
  mRectangle = rectangle;
}

void AddRectangleCommand::setWorkspaceParent(QStandardItem * parentItem)
{
  mParentItem = parentItem;
}

void AddRectangleCommand::execute()
{
  mProjectModified = mProject->modified();
  std::unique_ptr<RectangleItem> rectangleItem(new RectangleItem(mRectangle));
  mProject->addItem(std::move(rectangleItem));
  mProject->setModified();
}

void AddRectangleCommand::unexecute()
{
  mProject->setModified(mProjectModified);
}
