#include "actionmanager.h"

#include "action.h"

ActionManager::ActionManager()
{
}

Action * ActionManager::action(const QString & id) const
{
  ActionMap::const_iterator itr = mActionMap.find(id);
  if (itr == mActionMap.end())
    return nullptr;

  return itr.value();
}

void ActionManager::handleContextChanged(const QList<int> & contexts)
{
  for (ActionMap::iterator itr = mActionMap.begin(); itr != mActionMap.end(); ++itr) {
    itr.value()->setCurrentContext(contexts);
  }
}

Action * ActionManager::registerAction(const QString & actionId, QAction * action, const QList<int> & contexts)
{
  if (mActionMap.contains(actionId)) {
    return mActionMap.find(actionId).value();
  }

  Action * newAction = new Action(actionId, action);
  newAction->setCurrentContext(contexts);
  mActionMap.insert(actionId, newAction);

  return newAction;
}

Action * ActionManager::registerAction(const QString & actionId, const QList<int> & contexts)
{
  return registerAction(actionId, nullptr, contexts);
}

bool ActionManager::unregisterAction(const QString & actionId)
{
  if (!mActionMap.contains(actionId))
    return false;

  mActionMap.remove(actionId);
  return true;
}
