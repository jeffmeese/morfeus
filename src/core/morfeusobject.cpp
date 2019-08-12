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

MorfeusObject::MorfeusObject(const std::string & type, const std::string & name)
  : mId(createId())
  , mName(name)
  , mType(type)
{
}

MorfeusObject::MorfeusObject(const std::string & type, const std::string & id, const std::string & name)
  : mId(id)
  , mName(name)
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

std::string MorfeusObject::createName(int32_t number)
{
  std::ostringstream oss;
  oss << mType << number;
  return oss.str();
}

void MorfeusObject::print(std::ostream & output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, mType);
  if (!mName.empty())
    xmlutils::printValue(output, tabPos+2, "Name: ", mName);

  doPrint(output, tabPos+2);
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
