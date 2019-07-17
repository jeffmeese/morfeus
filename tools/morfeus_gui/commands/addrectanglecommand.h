#ifndef ADDRECTANGLECOMMAND_H
#define ADDRECTANGLECOMMAND_H

#include <command.h>

#include <memory>

#include <QStandardItem>

class GuiProject;
class Rectangle;
class ProjectItem;

class AddRectangleCommand
    : public Command
{
public:
  AddRectangleCommand(GuiProject * project, std::unique_ptr<Rectangle> rectangle);

public:
  static QString commandId();

protected:
  void execute() override;
  void unexecute() override;

private:
  bool mProjectModified;
  GuiProject * mProject;
  QString mItemId;
  std::unique_ptr<ProjectItem> mItem;
};

#endif // ADDRECTANGLECOMMAND_H