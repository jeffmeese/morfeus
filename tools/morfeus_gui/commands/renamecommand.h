#ifndef RENAMECOMMAND_H
#define RENAMECOMMAND_H

#include "command.h"

class ProjectItem;

class RenameCommand
    : public Command
{
public:
  RenameCommand(const QString & newName);

public:
  void setItem(ProjectItem * item);
  void setNewName(const QString & name);

public:
  static QString commandId();

protected:
  void execute() override;
  void unexecute() override;

private:
  ProjectItem * mItem;
  QString mNewName;
  QString mOldName;
};

#endif // RENAMECOMMAND_H
