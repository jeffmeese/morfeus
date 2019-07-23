#ifndef MATERIAL_H
#define MATERIAL_H

#include "morfeus.h"
#include "morfeusobject.h"

#include "factory.h"

#include <map>

class Material
    : public MorfeusObject
{
  class MaterialFactory;

public:
  enum Direction
  {
    xx = 0,
    xy,
    xz,
    yx,
    yy,
    yz,
    zx,
    zy,
    zz
  };

public:
  MORFEUS_LIB_DECL std::string name() const;
  MORFEUS_LIB_DECL dcomplex value(Direction direction) const;
  MORFEUS_LIB_DECL void setName(const std::string & name);
  MORFEUS_LIB_DECL void setValue(Direction direction, dcomplex value);

public:
  MORFEUS_LIB_DECL void print(std::ostream & output, int tabPos = 0) const;
  MORFEUS_LIB_DECL void print(int tabPos = 0) const;
  MORFEUS_LIB_DECL void readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node);
  MORFEUS_LIB_DECL void writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const;
  MORFEUS_LIB_DECL friend std::ostream & operator<<(std::ostream & output, const Material & object);

public:
  MORFEUS_LIB_DECL static MaterialFactory & factory();
  //MORFEUS_LIB_DECL static Material * create(const std::string & type);
  //MORFEUS_LIB_DECL static bool registerType(const std::string & type, boost::function<Material*()> creator);

protected:
  MORFEUS_LIB_DECL Material(const std::string & type);
  MORFEUS_LIB_DECL Material(const std::string & type, const std::string & name);
  MORFEUS_LIB_DECL Material(const std::string & type, const std::string & id, const std::string & name);

protected:
  virtual void doPrint(std::ostream & output, int tabPos = 0) const = 0;
  virtual void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) = 0;
  virtual void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const = 0;

private:
  class MaterialFactory
  {
    friend class Excitation;

  public:
    MORFEUS_LIB_DECL Material * create(const std::string & type);
    MORFEUS_LIB_DECL bool registerType(const std::string & type, boost::function<Material*()> creator);

  private:
    Factory<Material*, std::string, boost::function<Material*()> > mFactory;
  };

private:
  void init();

private:
  std::string mName;
  dcomplex mValues[9];
};

inline std::string Material::name() const
{
  return mName;
}

inline void Material::setName(const std::string &name)
{
  mName = name;
}

inline void Material::setValue(Direction direction, dcomplex value)
{
  mValues[direction] = value;
}

inline dcomplex Material::value(Direction direction) const
{
  return mValues[direction];
}

#endif // MATERIAL_H
