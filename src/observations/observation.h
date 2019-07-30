#ifndef OBSERVATION_H
#define OBSERVATION_H

#include "morfeus.h"
#include "morfeusobject.h"

#include "factory.h"
#include "rapidxml.hpp"
#include "xmlutils.h"

#include <boost/numeric/ublas/vector.hpp>

namespace morfeus {

class MeshInformation;
namespace mesh {
  class Mesh;
}

namespace observation {

class Observation
    : public MorfeusObject
{
  class ObservationFactory;

public:
  typedef boost::numeric::ublas::vector<dcomplex> vector;

public:
  MORFEUS_LIB_DECL std::string name() const;
  MORFEUS_LIB_DECL void setName(const std::string & name);

public:
  MORFEUS_LIB_DECL void calculate(double freqGHz, double thetaInc, double phiInc, const mesh::Mesh * mesh, const MeshInformation * meshInfo, const vector & efield);
  MORFEUS_LIB_DECL void print(std::ostream & output, int tabPos = 0) const;
  MORFEUS_LIB_DECL void print(int tabPos = 0) const;
  MORFEUS_LIB_DECL void report(std::ostream & output) const;
  MORFEUS_LIB_DECL void readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node);
  MORFEUS_LIB_DECL void writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const;
  MORFEUS_LIB_DECL friend std::ostream & operator<<(std::ostream & output, const Observation & object);

public:
  MORFEUS_LIB_DECL static ObservationFactory & factory();

protected:
  Observation(const std::string & type);
  Observation(const std::string & type, const std::string & name);
  Observation(const std::string & type, const std::string & id, const std::string & name);

protected:
  virtual void doCalculate(double freqGHz, double thetaInc, double phiInc, const mesh::Mesh * mesh, const MeshInformation * meshInfo, const vector & efield) = 0;
  virtual void doPrint(std::ostream & output, int tabPos = 0) const = 0;
  virtual void doReport(std::ostream & output) const = 0;
  virtual void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) = 0;
  virtual void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const = 0;

private:
  class ObservationFactory
  {
  public:
    MORFEUS_LIB_DECL Observation * create(const std::string & type);
    MORFEUS_LIB_DECL bool registerType(const std::string & type, boost::function<Observation*()> creator);

  private:
    Factory<Observation*, std::string, boost::function<Observation*()> > mFactory;
  };

private:
  std::string mName;
};

inline std::string Observation::name() const
{
  return mName;
}

inline void Observation::setName(const std::string & name)
{
  mName = name;
}

}
}

#endif // OBSERVATION_H
