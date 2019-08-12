#include "addrectanglecommand.h"

#include "guiproject.h"
#include "rectangleitem.h"

#include <QDebug>

static const QString commandIdentifier("Project.AddRectangle");

AddRectangleCommand::AddRectangleCommand(GuiProject * project, std::unique_ptr<morfeus::model::geometry::Rectangle> rectangle)
  : Command(commandIdentifier)
  , mProjectModified(false)
  , mProject(project)
{
  mItem.reset(new RectangleItem(std::move(rectangle)));
  mItemId = mItem->id();
}

QString AddRectangleCommand::commandId()
{
  return commandIdentifier;
}

void AddRectangleCommand::execute()
{
  mProject->addRectangle(std::move(mItem));
}

void AddRectangleCommand::unexecute()
{
  mItem = mProject->removeItem(mItemId);
}
