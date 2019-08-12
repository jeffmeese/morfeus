#ifndef MORFEUS_SOLUTION_SOLVER_H
#define MORFEUS_SOLUTION_SOLVER_H

#include "morfeus.h"

#include "core/morfeusobject.h"
#include "core/factory.h"

#include "math/types.h"

#include "xml/rapidxml.hpp"
#include "xml/xmlutils.h"

#include <boost/noncopyable.hpp>

namespace morfeus {

  class Excitation;
    namespace mesh {
      class Mesh;
  }

  namespace media {
    class MediaLibrary;
  }

  namespace Observation {
    class Observation;
  }

  namespace solution {
    class MeshInformation;
    class Solution;
  }
}

namespace morfeus {
namespace solution {

class Solver
    : private boost::noncopyable
    , public core::MorfeusObject
{
  class SolverFactory;

public:
  MORFEUS_LIB_DECL const media::MediaLibrary * mediaLibrary() const;
  MORFEUS_LIB_DECL void setMediaLibrary(const media::MediaLibrary * library);
  MORFEUS_LIB_DECL bool allocated() const;
  MORFEUS_LIB_DECL math::vector runSolver(double freqGhz, const mesh::Mesh * mesh,
                                          const solution::MeshInformation * meshInfo, const math::vector & rhs);
  MORFEUS_LIB_DECL void setAllocated(bool value);

public:
  MORFEUS_LIB_DECL static SolverFactory & factory();

protected:
  Solver(const std::string & type);
  Solver(const std::string & type, const std::string & id);

protected:


protected:
  virtual void allocateMatrices(const solution::MeshInformation * meshInfo) = 0;
  virtual void clearMatrices(const mesh::Mesh * mesh, const solution::MeshInformation * meshInfo) = 0;
  virtual math::vector solveSystem(const math::vector & rhs) = 0;
  virtual void updateMatrix(std::size_t row, std::size_t col, const math::dcomplex & i1, const math::dcomplex & i2) = 0;

private:
  void buildBiMatrix(double freqGHz, const mesh::Mesh * mesh, const solution::MeshInformation * meshInfo);
  void buildFeMatrix(double freqGHz, const mesh::Mesh * mesh, const solution::MeshInformation * meshInfo);

private:
  class SolverFactory
  {
  public:
    MORFEUS_LIB_DECL Solver * create(const std::string & type);
    MORFEUS_LIB_DECL bool registerType(const std::string & type, boost::function<Solver*()> creator);

  private:
    core::Factory<Solver*, std::string, boost::function<Solver*()> > mFactory;
  };

private:
  bool mAllocated;
  const media::MediaLibrary * mMediaLibrary;
};

inline bool Solver::allocated() const
{
  return mAllocated;
}

inline void Solver::setAllocated(bool value)
{
  mAllocated = value;
}

inline const media::MediaLibrary * Solver::mediaLibrary() const
{
return mMediaLibrary;
}

inline void Solver::setMediaLibrary(const media::MediaLibrary *library)
{
mMediaLibrary = library;
}

}
}

#endif // MORFEUS_SOLUTION_SOLVER_H
