#include "action.h"

Action::Action(const QString & id, QAction * qtAction)
  : QAction(nullptr)
  , mId(id)
  , mQtAction(nullptr)
  , mSupportsMultipleSelection(false)
{
  setText(id);
  setQtAction(qtAction);
  if (qtAction != nullptr) {
    setText(qtAction->text());
  }
}

Action::Action(const QString & id, const QString & text, QAction * qtAction)
  : QAction(nullptr)
  , mId(id)
  , mQtAction(nullptr)
  , mSupportsMultipleSelection(false)
{
  setText(text);
  setQtAction(qtAction);
}

void Action::doSetContext(const QList<int> & currentContext)
{
  Q_UNUSED(currentContext);
}

void Action::handleChanged()
{
  if (mQtAction != nullptr)
    setEnabled(mQtAction->isEnabled());
}

QString Action::id() const
{
  return mId;
}

QAction * Action::qtAction() const
{
  return mQtAction;
}

void Action::setCurrentContext(const QList<int> & currentContext)
{
  doSetContext(currentContext);
}

void Action::setQtAction(QAction *qtAction)
{
  if (mQtAction != nullptr) {
    disconnect(mQtAction, nullptr, this, nullptr);
    disconnect(this, nullptr, mQtAction, nullptr);
  }

  mQtAction = qtAction;

  if (mQtAction != nullptr) {
    connect(this, SIGNAL(triggered(bool)), mQtAction, SLOT(trigger()));
    connect(mQtAction, SIGNAL(changed()), SLOT(handleChanged()));
  }
}

void Action::setSupportsMultipleSelection(bool value)
{
  mSupportsMultipleSelection = value;
}

bool Action::supportsMultipleSelection() const
{
  return mSupportsMultipleSelection;
}

Separator::Separator()
  : Action("Separator")
{
  setSeparator(true);
  setSupportsMultipleSelection(false);
}
