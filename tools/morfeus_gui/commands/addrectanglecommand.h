#ifndef ADDRECTANGLECOMMAND_H
#define ADDRECTANGLECOMMAND_H

#include <command.h>

#include <QStandardItem>

class Project;
class Rectangle;

class AddRectangleCommand
    : public Command
{
public:
  AddRectangleCommand();

public:
  void setProject(Project * project);
  void setRectangle(Rectangle * rectangle);
  void setWorkspaceParent(QStandardItem * parentItem);

protected:
  void execute() override;
  void unexecute() override;

private:
  bool mProjectModified;
  Project * mProject;
  Rectangle * mRectangle;
  QStandardItem * mParentItem;
};

#endif // ADDRECTANGLECOMMAND_H
