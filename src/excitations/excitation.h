#ifndef EXCITATION_H
#define EXCITATION_H

#include "morfeus.h"
#include "morfeusobject.h"

#include "factory.h"

#include <boost/numeric/ublas/vector.hpp>

class Mesh;
class MeshInformation;

class Excitation
    : public MorfeusObject
{
  class ExcitationFactory;

protected:
  typedef boost::numeric::ublas::vector<dcomplex> vector;

public:
  MORFEUS_LIB_DECL std::string name() const;
  MORFEUS_LIB_DECL void setName(const std::string & name);

public:
  MORFEUS_LIB_DECL void excite(double freqGHz, const Mesh * mesh, const MeshInformation * meshInfo, vector & rhs) const;
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
  virtual void doExcite(double freqGHz, const Mesh * mesh, const MeshInformation * meshInfo, vector & rhs) const = 0;
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
    Factory<Excitation*, std::string, boost::function<Excitation*()> > mFactory;
  };

private:
  std::string mName;
  static ExcitationFactory mFactory;
};

inline std::string Excitation::name() const
{
  return mName;
}

inline void Excitation::setName(const std::string &name)
{
  mName = name;
}

#endif // EXCITATION_H
