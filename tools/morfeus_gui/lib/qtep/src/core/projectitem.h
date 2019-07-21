#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include "qtep.h"

#include <QObject>
#include <QString>
#include <QVector>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class Project;

class QTEP_LIB_DECL ProjectItem
    : public QObject
{
  Q_OBJECT

public:
  virtual ~ProjectItem();

public:
  QString id() const;
  QString name() const;
  QString typeId() const;
  ProjectItem * parent() const;
  Project * project() const;
  void setName(const QString & name);
  void setProject(Project * project);

public:
  void addItem(std::unique_ptr<ProjectItem> projectItem);
  ProjectItem * findItem(const QString & itemId);
  const ProjectItem * findItem(const QString & itemId) const;
  QList<ProjectItem*> findItems(const QString & name) const;
  ProjectItem * itemAt(std::size_t index);
  const ProjectItem * itemAt(std::size_t index) const;
  bool loadXml(QXmlStreamReader & reader);
  bool removeItem(const QString & itemId);
  bool saveXml(QXmlStreamWriter & writer) const;
  std::size_t totalItems() const;

signals:
  void itemAboutToBeRemoved(ProjectItem * item);
  void itemAdded(ProjectItem * item);
  void itemChanged();
  void itemRemoved();

protected:
  ProjectItem(const QString & typeId);
  ProjectItem(const QString & typeId, const QString & name);
  ProjectItem(const QString & typeId, const QString & itemId, const QString & name);

protected:
  bool loadChildren(QXmlStreamReader & reader);
  bool saveChildren(QXmlStreamWriter & writer) const;

protected:
  virtual bool loadAttributes(QXmlStreamReader & reader) = 0;
  virtual bool saveAttributes(QXmlStreamWriter & writer) const = 0;

private:
  typedef std::unique_ptr<ProjectItem> ProjectItemPtr;

private:
  QString mId;
  QString mName;
  QString mTypeId;
  ProjectItem * mParent;
  Project * mProject;
  std::vector<ProjectItemPtr> mItems;
};

inline QString ProjectItem::id() const
{
  return mId;
}

inline QString ProjectItem::name() const
{
  return mName;
}

inline ProjectItem * ProjectItem::parent() const
{
  return mParent;
}

inline Project * ProjectItem::project() const
{
  return mProject;
}

inline std::size_t ProjectItem::totalItems() const
{
  return mItems.size();
}

inline QString ProjectItem::typeId() const
{
  return mTypeId;
}

#endif // PROJECTITEM_H
