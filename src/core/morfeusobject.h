#ifndef MORFEUSOBJECT_H
#define MORFEUSOBJECT_H

#include "morfeus.h"
#include "rapidxml.hpp"

#include <cstdint>
#include <string>

#include <boost/property_tree/ptree.hpp>

class MorfeusObject
{
protected:
  typedef boost::property_tree::ptree ptree;

public:
  MORFEUS_LIB_DECL std::string id() const;
  MORFEUS_LIB_DECL std::string name() const;
  MORFEUS_LIB_DECL int32_t number() const;
  MORFEUS_LIB_DECL void setName(const std::string & name);
  MORFEUS_LIB_DECL void setNumber(int32_t value);

public:
  MORFEUS_LIB_DECL void readFromXml(ptree & tree);
  MORFEUS_LIB_DECL void writeToXml(ptree & tree) const;

protected:
  MorfeusObject();
  MorfeusObject(const std::string & name);
  MorfeusObject(const std::string & name, int32_t number);
  MorfeusObject(std::string id);
  MorfeusObject(std::string id, const std::string & name);
  MorfeusObject(std::string id, const std::string & name, int32_t number);

protected:
  virtual void doReadFromXml(ptree & tree) = 0;
  virtual void doWriteToXml(ptree & tree) const = 0;

private:
  std::string createId();

private:
  std::string mId;
  std::string mName;
  int32_t mNumber;
};

inline std::string MorfeusObject::id() const
{
  return mId;
}

inline std::string MorfeusObject::name() const
{
  return mName;
}

inline int32_t MorfeusObject::number() const
{
  return mNumber;
}

inline void MorfeusObject::setName(const std::string &name)
{
  mName = name;
}

inline void MorfeusObject::setNumber(int32_t value)
{
  mNumber = value;
}

#endif // MORFEUSOBJECT_H
