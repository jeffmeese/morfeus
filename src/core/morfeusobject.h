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
  MORFEUS_LIB_DECL std::string name() const;
  MORFEUS_LIB_DECL int32_t number() const;
  MORFEUS_LIB_DECL void setName(const std::string & name);
  MORFEUS_LIB_DECL void setNumber(int32_t value);

public:
  MORFEUS_LIB_DECL void print(std::ostream & output, int tabPos = 0) const;
  MORFEUS_LIB_DECL void print(int tabPos = 0) const;
  MORFEUS_LIB_DECL void readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node);
  MORFEUS_LIB_DECL void writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const;
  MORFEUS_LIB_DECL friend std::ostream & operator<<(std::ostream & output, const MorfeusObject & object);

protected:
  MorfeusObject();
  MorfeusObject(int32_t number);
  MorfeusObject(const std::string & name);
  MorfeusObject(const std::string & name, int32_t number);
  MorfeusObject(std::string id);
  MorfeusObject(std::string id, const std::string & name);
  MorfeusObject(std::string id, const std::string & name, int32_t number);

protected:
  virtual void doPrint(std::ostream & output, int tabPos = 0) const = 0;
  virtual void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) = 0;
  virtual void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const = 0;

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
