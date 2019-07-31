#ifndef MORFEUS_CORE_MORFEUSOBJECT_H
#define MORFEUS_CORE_MORFEUSOBJECT_H

#include "morfeus.h"

#include "xml/rapidxml.hpp"
#include "xml/xmlutils.h"

#include <string>

namespace morfeus {
namespace core {

class MorfeusObject
{
public:
  MORFEUS_LIB_DECL virtual ~MorfeusObject();

public:
  MORFEUS_LIB_DECL std::string id() const;
  MORFEUS_LIB_DECL std::string type() const;

public:
  MORFEUS_LIB_DECL void print(std::ostream & output, int tabPos = 0) const;
  MORFEUS_LIB_DECL void print(int tabPos = 0) const;
  MORFEUS_LIB_DECL void readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node);
  MORFEUS_LIB_DECL void writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const;
  MORFEUS_LIB_DECL friend std::ostream & operator<<(std::ostream & output, const MorfeusObject & object);

protected:
  MorfeusObject(const std::string & type);
  MorfeusObject(const std::string & type, std::string id);

protected:
  virtual void doPrint(std::ostream & output, int tabPos) const = 0;
  virtual void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) = 0;
  virtual void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const = 0;

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

}
}

#endif // MORFEUS_CORE_MORFEUSOBJECT_H
