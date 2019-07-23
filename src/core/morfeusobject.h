#ifndef MORFEUSOBJECT_H
#define MORFEUSOBJECT_H

#include "morfeus.h"
#include "rapidxml.hpp"
#include "xmlutils.h"

#include <cstdint>
#include <string>

class MorfeusObject
{
public:
  virtual ~MorfeusObject();

public:
  MORFEUS_LIB_DECL std::string id() const;
  MORFEUS_LIB_DECL std::string type() const;

protected:
  MorfeusObject(const std::string & type);
  MorfeusObject(const std::string & type, std::string id);

private:
  std::string createId();

private:
  std::string mId;
  std::string mType;
};

inline std::string MorfeusObject::id() const
{
  return mId;
}

inline std::string MorfeusObject::type() const
{
  return mType;
}

#endif // MORFEUSOBJECT_H
