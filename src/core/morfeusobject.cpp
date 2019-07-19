#include "morfeusobject.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

MorfeusObject::MorfeusObject()
  : mId(createId())
  , mNumber(-1)
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

std::string MorfeusObject::createId()
{
  boost::uuids::random_generator gen;
  boost::uuids::uuid uuid = gen();
  return boost::uuids::to_string(uuid);
}

void MorfeusObject::readFromXml(ptree & tree)
{
  doReadFromXml(tree);
}

void MorfeusObject::writeToXml(ptree & tree) const
{
  doWriteToXml(tree);
}
