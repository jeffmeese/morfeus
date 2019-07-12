#include "project.h"
#include "projectitem.h"

#include <QFile>
#include <QDebug>

Project::Project()
  : mModified(false)
  , mName("Project")
{
}

Project::~Project()
{
}

void Project::addItem(std::unique_ptr<ProjectItem> projectItem)
{
  projectItem->setProject(this);
  mItems.push_back(std::move(projectItem));
  emit itemAdded(mItems.back().get());
  setModified();
}

ProjectItem * Project::findItem(const QString & itemId)
{
  for (std::vector<ProjectItemPtr>::iterator itr = mItems.begin(); itr != mItems.end(); ++itr) {
    if (itr->get()->id() == itemId) {
      return itr->get();
    }
  }
  return nullptr;
}

const ProjectItem * Project::findItem(const QString & itemId) const
{
  for (std::vector<ProjectItemPtr>::const_iterator itr = mItems.begin(); itr != mItems.end(); ++itr) {
    if (itr->get()->id() == itemId) {
      return itr->get();
    }
  }
  return nullptr;
}

QList<ProjectItem*> Project::findItems(const QString & name) const
{
  QList<ProjectItem*> items;
  for (std::vector<ProjectItemPtr>::const_iterator itr = mItems.begin(); itr != mItems.end(); ++itr) {
    if (itr->get()->name() == name) {
      items.append(itr->get());
    }
  }
  return items;
}

ProjectItem * Project::itemAt(std::size_t index)
{
  return mItems.at(index).get();
}

const ProjectItem * Project::itemAt(std::size_t index) const
{
  return mItems.at(index).get();
}

bool Project::load(const QString & fileName)
{
  mFileName = QString();

  QFile file(fileName);
  if (!file.open(QFile::ReadOnly)) {
    return false;
  }

  QXmlStreamReader reader(&file);
  if (!loadXml(reader)) {
    return false;
  }

  mFileName = fileName;
  setModified(false);
  return true;
}

bool Project::loadXml(QXmlStreamReader & reader)
{
  return !reader.hasError();
}

void Project::newProject()
{
  mName = QString();
  mFileName = QString();
  mItems.clear();
  setModified(false);
}

bool Project::removeItem(const QString & itemId)
{
  for (std::vector<ProjectItemPtr>::iterator itr = mItems.begin(); itr != mItems.end(); ++itr) {
    if (itr->get()->id() == itemId) {
      emit itemAboutToBeRemoved(itr->get());
      mItems.erase(itr);
      emit itemRemoved();
      setModified();
      return true;
    }
  }
  return false;
}

bool Project::save(const QString & fileName)
{
  mFileName = QString();

  QFile file(fileName);
  if (!file.open(QFile::WriteOnly)) {
    return false;
  }

  QXmlStreamWriter writer(&file);
  writer.setAutoFormatting(true);
  writer.setAutoFormattingIndent(2);
  writer.writeStartDocument();
  if (!saveXml(writer)) {
    return false;
  }
  writer.writeEndDocument();

  mFileName = fileName;
  setModified(false);
  return true;
}

bool Project::saveXml(QXmlStreamWriter & writer) const
{
  writer.writeStartElement("Project");
  for (std::size_t i = 0; i < mItems.size(); i++) {
    ProjectItem * item = mItems.at(i).get();
    item->saveXml(writer);
  }
  writer.writeEndElement();
  return !writer.hasError();
}

void Project::setModified(bool value)
{
  mModified = value;
  emit modified(value);
}

void Project::setName(const QString & name)
{
  mName = name;
  emit nameChanged(name);
  setModified();
}

const ProjectItem * Project::takeItem(const QString & itemId) const
{
  for (std::vector<ProjectItemPtr>::const_iterator itr = mItems.begin(); itr != mItems.end(); ++itr) {
    if (itr->get()->id() == itemId) {
      const ProjectItem * item = itr->get();
      return item;
    }
  }
  return nullptr;
}

ProjectItem * Project::takeItem(const QString & itemId)
{
  for (std::vector<ProjectItemPtr>::iterator itr = mItems.begin(); itr != mItems.end(); ++itr) {
    if (itr->get()->id() == itemId) {
      ProjectItem * item = itr->get();
      return item;
    }
  }
  return nullptr;
}

std::size_t Project::totalItems() const
{
  return mItems.size();
}
