#include "morfeusobject.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <iostream>

namespace morfeus {
namespace core {

MorfeusObject::MorfeusObject(const std::string & type)
  : mId(createId())
  , mType(type)
{
}

MorfeusObject::MorfeusObject(const std::string & type, std::string id)
  : mId(id)
  , mType(type)
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

void MorfeusObject::print(std::ostream & output, int tabPos) const
{
  doPrint(output, tabPos);
}

void MorfeusObject::print(int tabPos) const
{
  print(std::cout);
}

void MorfeusObject::readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  doXmlRead(document, node);
}

void MorfeusObject::writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  doXmlWrite(document, node);
}

std::ostream & operator<<(std::ostream & output, const MorfeusObject & object)
{
  object.print(output);
  return output;
}

}
}
