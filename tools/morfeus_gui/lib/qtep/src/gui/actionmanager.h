#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include "qtep.h"

#include <QAction>
#include <QList>
#include <QMap>

class Action;

class QTEP_LIB_DECL ActionManager
    : public QObject
{
  Q_OBJECT

public:
  ActionManager();

public:
  Action * action(const QString & id) const;
  Action * registerAction(const QString & actionId, const QList<int> & contexts = QList<int>());
  Action * registerAction(const QString & actionId, QAction * action, const QList<int> & contexts = QList<int>());
  bool unregisterAction(const QString & actionId);

public slots:
  void handleContextChanged(const QList<int> & contexts);

private:
  typedef QMap<QString, Action*> ActionMap;

private:
  ActionMap mActionMap;
};

#endif // ACTIONMANAGER_H
