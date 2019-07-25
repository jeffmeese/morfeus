#ifndef SHAPE_H
#define SHAPE_H

#include "morfeus.h"
#include "morfeusobject.h"

#include "factory.h"
#include "mesherpolygon.h"
#include "segment.h"
#include "vertex.h"

#include <string>
#include <vector>

class Polygon;
class Shape
    : public MorfeusObject
{
  class ShapeFactory;

public:
  MORFEUS_LIB_DECL std::string name() const;
  MORFEUS_LIB_DECL void setName(const std::string & name);

public:
  MORFEUS_LIB_DECL std::vector<MesherPolygon> getMesherPolygons() const;
  MORFEUS_LIB_DECL void print(std::ostream & output, int tabPos = 0) const;
  MORFEUS_LIB_DECL void print(int tabPos = 0) const;
  MORFEUS_LIB_DECL void readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node);
  MORFEUS_LIB_DECL void writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const;
  MORFEUS_LIB_DECL friend std::ostream & operator<<(std::ostream & output, const Shape & object);

public:
  MORFEUS_LIB_DECL static ShapeFactory & factory();

protected:
  Shape(const std::string & type);
  Shape(const std::string & type, const std::string & name);
  Shape(const std::string & type, const std::string & id, const std::string & name);

protected:
  virtual std::vector<MesherPolygon> doGetMesherPolygons() const = 0;
  virtual void doPrint(std::ostream & output, int tabPos = 0) const = 0;
  virtual void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) = 0;
  virtual void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const = 0;

private:
  class ShapeFactory
  {
  public:
    MORFEUS_LIB_DECL Shape * createShape(const std::string & type)
    {
      return mFactory.create(type);
    }

    MORFEUS_LIB_DECL bool registerType(const std::string & type,  boost::function<Shape*()> creator)
    {
      mFactory.registerFactory(type, creator);
      return true;
    }

  private:
    Factory<Shape*, std::string, boost::function<Shape*()> > mFactory;
  };

private:
  std::string mName;
};

inline std::string Shape::name() const
{
  return mName;
}

inline void Shape::setName(const std::string &name)
{
  mName = name;
}

#endif // SHAPE_H
