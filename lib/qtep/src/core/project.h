#ifndef PROJECT_H
#define PROJECT_H

#include "qtep.h"

#include <vector>

#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class ProjectItem;

class QTEP_LIB_DECL Project
    : public QObject
{
  Q_OBJECT

public:
  Project();
  virtual ~Project();

public:
  QString fileName() const;
  bool modified() const;
  QString name() const;
  void setModified(bool modified = true);
  void setName(const QString & name);

public:
  void addItem(std::unique_ptr<ProjectItem> item);
  ProjectItem * findItem(const QString & itemId);
  const ProjectItem * findItem(const QString & itemId) const;
  QList<ProjectItem*> findItems(const QString & name) const;
  ProjectItem * itemAt(std::size_t index);
  const ProjectItem * itemAt(std::size_t index) const;
  bool load(const QString & fileName);
  bool loadXml(QXmlStreamReader & reader);
  std::unique_ptr<ProjectItem> removeItem(const QString & itemId);
  bool save(const QString & fileName);
  bool saveXml(QXmlStreamWriter & writer) const;
  std::size_t totalItems() const;

signals:
  void itemAboutToBeRemoved(ProjectItem * item);
  void itemAdded(ProjectItem * item);
  void itemRemoved();
  void modified(bool modified);
  void nameChanged(const QString & newName);

private:
  typedef std::unique_ptr<ProjectItem> ProjectItemPtr;

private:
  bool mModified;
  QString mFileName;
  QString mName;
  std::vector<ProjectItemPtr> mItems;
};

inline QString Project::fileName() const
{
  return mFileName;
}

inline bool Project::modified() const
{
  return mModified;
}

inline QString Project::name() const
{
  return mName;
}

#endif // PROJECT_H
