#include "renamecommand.h"

#include "projectitem.h"

#include <QDebug>

static const QString commandIdentifier("Command.Rename");

RenameCommand::RenameCommand(const QString & newName)
  : Command(commandIdentifier)
  , mItem(nullptr)
{
  setNewName(newName);
  setText("Rename");
}

QString RenameCommand::commandId()
{
  return commandIdentifier;
}

void RenameCommand::execute()
{
  qDebug() << QObject::tr(__FUNCTION__);
  if (mItem != nullptr) {
    mOldName = mItem->name();
    mItem->setName(mNewName);
  }
}

void RenameCommand::setItem(ProjectItem *item)
{
  mItem = item;
}

void RenameCommand::setNewName(const QString &name)
{
  mNewName = name;
}

void RenameCommand::unexecute()
{
  qDebug() << QObject::tr(__FUNCTION__);

  if (mItem != nullptr) {
    mItem->setName(mOldName);
  }
}
