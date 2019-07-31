#ifndef MORFEUS_EXCITATION_EXCITATION_H
#define MORFEUS_EXCITATION_EXCITATION_H

#include "morfeus.h"

#include "core/morfeusobject.h"
#include "core/factory.h"

#include "xml/rapidxml.hpp"
#include "xml/xmlutils.h"

#include <boost/numeric/ublas/vector.hpp>

namespace morfeus {

  namespace mesh {
    class Mesh;
  }

  namespace solution {
    class MeshInformation;
  }
}

namespace morfeus {
namespace excitation {

class Excitation
    : public core::MorfeusObject
{
  class ExcitationFactory;

protected:
  typedef boost::numeric::ublas::vector<dcomplex> vector;

public:
  MORFEUS_LIB_DECL bool angleDependent() const;
  MORFEUS_LIB_DECL std::string name() const;
  MORFEUS_LIB_DECL void setAngleDependent(bool value);
  MORFEUS_LIB_DECL void setName(const std::string & name);

public:
  MORFEUS_LIB_DECL void excite(double freqGHz, double thetaInc, double phiInc, const mesh::Mesh * mesh, const solution::MeshInformation * meshInfo, vector & rhs) const;
  MORFEUS_LIB_DECL void print(std::ostream & output, int tabPos = 0) const;
  MORFEUS_LIB_DECL void print(int tabPos = 0) const;
  MORFEUS_LIB_DECL void readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node);
  MORFEUS_LIB_DECL void writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const;
  MORFEUS_LIB_DECL friend std::ostream & operator<<(std::ostream & output, const Excitation & object);

public:
  MORFEUS_LIB_DECL static ExcitationFactory & factory();

protected:
  Excitation(const std::string & type);
  Excitation(const std::string & type, const std::string & name);
  Excitation(const std::string & type, const std::string & id, const std::string & name);

protected:
  virtual void doExcite(double freqGHz, double thetaInc, double phiInc, const mesh::Mesh * mesh, const solution::MeshInformation * meshInfo, vector & rhs) const = 0;
  virtual void doPrint(std::ostream & output, int tabPos = 0) const = 0;
  virtual void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) = 0;
  virtual void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const = 0;

private:
  class ExcitationFactory
  {
    friend class Excitation;

  public:
    MORFEUS_LIB_DECL Excitation * create(const std::string & type);
    MORFEUS_LIB_DECL bool registerType(const std::string & type, boost::function<Excitation*()> creator);

  private:
    core::Factory<Excitation*, std::string, boost::function<Excitation*()> > mFactory;
  };

private:
  bool mAngleDependent;
  std::string mName;
  static ExcitationFactory mFactory;
};

inline bool Excitation::angleDependent() const
{
  return mAngleDependent;
}

inline std::string Excitation::name() const
{
  return mName;
}

inline void Excitation::setAngleDependent(bool value)
{
  mAngleDependent = value;
}

inline void Excitation::setName(const std::string &name)
{
  mName = name;
}

}
}

#endif // MORFEUS_EXCITATION_EXCITATION_H
