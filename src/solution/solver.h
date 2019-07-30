#ifndef SOLVER_H
#define SOLVER_H

#include "morfeus.h"
#include "morfeusobject.h"

#include "factory.h"
#include "rapidxml.hpp"
#include "xmlutils.h"

#include <boost/noncopyable.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/vector.hpp>

namespace Morfeus {

  class Excitation;
  namespace mesh {
  class Mesh;
  }
  class MeshInformation;

  namespace Observation {
    class Observation;
  }

  class Solution;
}

namespace Morfeus {

class Solver
    : private boost::noncopyable
    , public MorfeusObject
{
  class SolverFactory;

public:
  typedef boost::numeric::ublas::vector<dcomplex> vector;

public:
  MORFEUS_LIB_DECL bool allocated() const;
  MORFEUS_LIB_DECL void print(std::ostream & output, int tabPos = 0) const;
  MORFEUS_LIB_DECL void print(int tabPos = 0) const;
  MORFEUS_LIB_DECL void readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node);
  MORFEUS_LIB_DECL void writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const;
  MORFEUS_LIB_DECL friend std::ostream & operator<<(std::ostream & output, const Solver & object);
  MORFEUS_LIB_DECL vector runSolver(double freqGhz, const mesh::Mesh * mesh, const MeshInformation * meshInfo, const vector & rhs);
  MORFEUS_LIB_DECL void setAllocated(bool value);

public:
  MORFEUS_LIB_DECL static SolverFactory & factory();

protected:
  Solver(const std::string & type);
  Solver(const std::string & type, const std::string & id);

protected:
  typedef boost::numeric::ublas::mapped_matrix<dcomplex> sparse_matrix;
  typedef boost::numeric::ublas::triangular_matrix<dcomplex> triangular_matrix;

protected:
  virtual void allocateMatrices(const MeshInformation * meshInfo) = 0;
  virtual void clearMatrices(const mesh::Mesh * mesh, const MeshInformation * meshInfo) = 0;
  virtual void doPrint(std::ostream & output, int tabPos = 0) const = 0;
  virtual void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) = 0;
  virtual void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const = 0;
  virtual vector solveSystem(const vector & rhs) = 0;
  virtual void updateBoundaryIntegralMatrix(std::size_t row, std::size_t col, const dcomplex & i1, const dcomplex & i2) = 0;
  virtual void updateFiniteElementMatrix(std::size_t row, std::size_t col, const dcomplex & i1, const dcomplex & i2) = 0;

private:
  void buildBiMatrix(double freqGHz, const mesh::Mesh * mesh, const MeshInformation * meshInfo);
  void buildFeMatrix(double freqGHz, const mesh::Mesh * mesh, const MeshInformation * meshInfo);

private:
  class SolverFactory
  {
  public:
    MORFEUS_LIB_DECL Solver * create(const std::string & type);
    MORFEUS_LIB_DECL bool registerType(const std::string & type, boost::function<Solver*()> creator);

  private:
    Factory<Solver*, std::string, boost::function<Solver*()> > mFactory;
  };

private:
  bool mAllocated;
};

inline bool Solver::allocated() const
{
  return mAllocated;
}

inline void Solver::setAllocated(bool value)
{
  mAllocated = value;
}

}

#endif // SOLVER_H
