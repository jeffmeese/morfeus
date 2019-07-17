#ifndef GUIPROJECT_H
#define GUIPROJECT_H

#include <project.h>

class ProjectItem;

class GuiProject
    : public Project
{
  Q_OBJECT

public:
  GuiProject();

public:
  void addRectangle(std::unique_ptr<ProjectItem> rectItem);

signals:
  void rectangleAdded(ProjectItem * item);
};

#endif // GUIPROJECT_H
