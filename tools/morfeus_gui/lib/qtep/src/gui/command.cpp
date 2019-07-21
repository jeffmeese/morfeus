#include "command.h"

Command::Command(const QString & id)
  : mId(id)
{
}

Command::~Command()
{

}

QString Command::commandId() const
{
  return mId;
}

void Command::redo()
{
  execute();
}

void Command::undo()
{
  unexecute();
}
