#include "guiproject.h"

#include "projectitem.h"

GuiProject::GuiProject()
{
}

void GuiProject::addRectangle(std::unique_ptr<ProjectItem> rectItem)
{
  addItem(std::move(rectItem));
  emit rectangleAdded(itemAt(totalItems()-1));
}

