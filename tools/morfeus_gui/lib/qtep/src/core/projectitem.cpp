#include "projectitem.h"
#include "project.h"

#include <QUuid>

ProjectItem::ProjectItem(const QString & typeId)
  : mId(QUuid::createUuid().toString())
  , mTypeId(typeId)
  , mParent(nullptr)
  , mProject(nullptr)
{
  setName(mTypeId);
}

ProjectItem::ProjectItem(const QString & typeId, const QString & name)
  : mId(QUuid::createUuid().toString())
  , mTypeId(typeId)
  , mParent(nullptr)
  , mProject(nullptr)
{
  setName(name);
}

ProjectItem::ProjectItem(const QString & typeId, const QString & itemId, const QString & name)
  : mId(itemId)
  , mTypeId(typeId)
  , mParent(nullptr)
  , mProject(nullptr)
{
  setName(name);
}

ProjectItem::~ProjectItem()
{
}

void ProjectItem::addItem(std::unique_ptr<ProjectItem> projectItem)
{
  projectItem->setParent(this);
  projectItem->setProject(this->project());
  mItems.push_back(std::move(projectItem));
  emit itemAdded(mItems.back().get());
  if (mProject != nullptr) {
    mProject->setModified();
  }
}

ProjectItem * ProjectItem::findItem(const QString & itemId)
{
  for (std::vector<ProjectItemPtr>::iterator itr = mItems.begin(); itr != mItems.end(); ++itr) {
    if (itr->get()->id() == itemId) {
      return itr->get();
    }
  }
  return nullptr;
}

const ProjectItem * ProjectItem::findItem(const QString & itemId) const
{
  for (std::vector<ProjectItemPtr>::const_iterator itr = mItems.begin(); itr != mItems.end(); ++itr) {
    if (itr->get()->id() == itemId) {
      return itr->get();
    }
  }
  return nullptr;
}

QList<ProjectItem*> ProjectItem::findItems(const QString & name) const
{
  QList<ProjectItem*> items;
  for (std::vector<ProjectItemPtr>::const_iterator itr = mItems.begin(); itr != mItems.end(); ++itr) {
    if (itr->get()->name() == name) {
      items.append(itr->get());
    }
  }
  return items;
}

ProjectItem * ProjectItem::itemAt(std::size_t index)
{
  return mItems.at(index).get();
}

const ProjectItem * ProjectItem::itemAt(std::size_t index) const
{
  return mItems.at(index).get();
}

bool ProjectItem::loadChildren(QXmlStreamReader & reader)
{
  while (true) {
    bool success = reader.readNextStartElement();
    if (!success) {
      break;
    }

    // TODO: Need the factory here
    //QString elementName = reader.name().toString();
  }

  return !reader.hasError();
}

bool ProjectItem::loadXml(QXmlStreamReader & reader)
{
  QXmlStreamAttributes attrs = reader.attributes();
  if (attrs.hasAttribute("Name")) {
    QString name = attrs.value("Name").toString();
    setName(name);
  }

  loadAttributes(reader);
  loadChildren(reader);
  return !reader.hasError();
}

bool ProjectItem::removeItem(const QString & itemId)
{
  for (std::vector<ProjectItemPtr>::iterator itr = mItems.begin(); itr != mItems.end(); ++itr) {
    if (itr->get()->id() == itemId) {
      emit itemAboutToBeRemoved(itr->get());
      mItems.erase(itr);
      emit itemRemoved();
      return true;
    }
  }
  return false;
}

bool ProjectItem::saveChildren(QXmlStreamWriter & writer) const
{
  for (std::vector<ProjectItemPtr>::const_iterator itr = mItems.begin(); itr != mItems.end(); ++itr) {
    itr->get()->saveXml(writer);
  }

  return !writer.hasError();
}

bool ProjectItem::saveXml(QXmlStreamWriter & writer) const
{
  writer.writeStartElement(mTypeId);
  saveAttributes(writer);
  saveChildren(writer);
  writer.writeEndElement();
  return !writer.hasError();
}

void ProjectItem::setName(const QString & name)
{
  mName = name;
  emit itemChanged();
  if (mProject != nullptr) {
    mProject->setModified();
  }
}

void ProjectItem::setProject(Project * project)
{
  mProject = project;
}
