#include "morfeusobject.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <iostream>

namespace Morfeus {

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

}
