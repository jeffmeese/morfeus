#ifndef WORKSPACEMODELITEM_H
#define WORKSPACEMODELITEM_H

#include <QStandardItem>
#include <QList>

#include <Factory.h>
#include <Singleton.h>

class Action;
class AddRectangleCommand;
class Project;
class ProjectItem;
class RenameCommand;

class WorkspaceModelItem
    : public QObject
    , public QStandardItem
{
  Q_OBJECT

public:
  QString id() const;

public:
  QList<Action*> getActionList() const;

public:
  typedef Loki::SingletonHolder<Loki::Factory<WorkspaceModelItem, QString, LOKI_TYPELIST_1(ProjectItem*)> > Factory;

protected:
  WorkspaceModelItem(QString itemId);

protected:
  virtual QList<Action*> doGetActionList() const = 0;

private:
  QString mItemId;
};

#endif // WORKSPACEMODELITEM_H
