#include "morfeusobject.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <iostream>

MorfeusObject::MorfeusObject()
  : mId(createId())
  , mNumber(-1)
{
}

MorfeusObject::MorfeusObject(int32_t number)
  : mId(createId())
  , mNumber(number)
{
}

MorfeusObject::MorfeusObject(const std::string & name)
  : mId(createId())
  , mName(name)
  , mNumber(-1)
{
}

MorfeusObject::MorfeusObject(const std::string & name, int32_t number)
  : mId(createId())
  , mName(name)
  , mNumber(number)
{
}

MorfeusObject::MorfeusObject(std::string id)
  : mId(id)
  , mNumber(-1)
{
}

MorfeusObject::MorfeusObject(std::string id, const std::string & name)
  : mId(id)
  , mName(name)
  , mNumber(-1)
{
}

MorfeusObject::MorfeusObject(std::string id, const std::string & name, int32_t number)
  : mId(id)
  , mName(name)
  , mNumber(number)
{
}

MorfeusObject::~MorfeusObject()
{
}

std::string MorfeusObject::createId()
{
  boost::uuids::random_generator gen;
  boost::uuids::uuid uuid = gen();
  return boost::uuids::to_string(uuid);
}

void MorfeusObject::print(int tabPos) const
{
  print(std::cout, tabPos);
}

std::ostream & operator<<(std::ostream & output, const MorfeusObject & object)
{
  object.print(output);
  return output;
}

void MorfeusObject::print(std::ostream & output, int tabPos) const
{
  doPrint(output, tabPos);
}

void MorfeusObject::readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  doXmlRead(document, node);
}

void MorfeusObject::writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  doXmlWrite(document, node);
}
