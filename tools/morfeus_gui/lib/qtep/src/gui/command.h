#ifndef COMMAND_H
#define COMMAND_H

#include "qtep.h"

#include <QUndoCommand>

class QTEP_LIB_DECL Command
    : public QUndoCommand
{
public:
  virtual ~Command() override;

public:
   QString commandId() const;

public:
  virtual void redo() override;
  virtual void undo() override;

protected:
  Command(const QString & id);
  Command(const Command & command) = delete;
  Command & operator=(const Command & command) = delete;

protected:
  virtual void execute() = 0;
  virtual void unexecute() = 0;

private:
  QString mId;
};

#endif // COMMAND_H
