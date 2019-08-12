#ifndef MORFEUS_SOLUTION_SOLUTION_H
#define MORFEUS_SOLUTION_SOLUTION_H

#include "morfeus.h"

#include "core/morfeusobject.h"

#include "xml/rapidxml.hpp"
#include "xml/xmlutils.h"

#include <boost/noncopyable.hpp>

#include <memory>
#include <vector>

namespace morfeus {

  namespace mesh {
    class Mesh;
  }

  namespace observation {
    class Observation;
  }

  namespace solution {
    class Solver;
  }

  namespace sources {
    class Source;
  }
}

namespace morfeus {
namespace solution {

class Solution
    : private boost::noncopyable
    , public core::MorfeusObject
{
public:
  MORFEUS_LIB_DECL Solution();
  MORFEUS_LIB_DECL Solution(const std::string & id);

public:
  MORFEUS_LIB_DECL double frequencyIncrement() const;
  MORFEUS_LIB_DECL double frequencyStart() const;
  MORFEUS_LIB_DECL double frequencyStop() const;
  MORFEUS_LIB_DECL double phiIncrement() const;
  MORFEUS_LIB_DECL double phiStart() const;
  MORFEUS_LIB_DECL double phiStop() const;
  MORFEUS_LIB_DECL double thetaIncrement() const;
  MORFEUS_LIB_DECL double thetaStart() const;
  MORFEUS_LIB_DECL double thetaStop() const;
  MORFEUS_LIB_DECL Solver * solver();
  MORFEUS_LIB_DECL const Solver * solver() const;
  MORFEUS_LIB_DECL void setFrequencyIncrement(double value);
  MORFEUS_LIB_DECL void setFrequencyStart(double value);
  MORFEUS_LIB_DECL void setFrequencyStop(double value);
  MORFEUS_LIB_DECL void setPhiIncrement(double value);
  MORFEUS_LIB_DECL void setPhiStart(double value);
  MORFEUS_LIB_DECL void setPhiStop(double value);
  MORFEUS_LIB_DECL void setThetaIncrement(double value);
  MORFEUS_LIB_DECL void setThetaStart(double value);
  MORFEUS_LIB_DECL void setThetaStop(double value);
  MORFEUS_LIB_DECL void setSolver(std::unique_ptr<Solver> solver);
  MORFEUS_LIB_DECL std::size_t totalObservations() const;
  MORFEUS_LIB_DECL std::size_t totalSources() const;

public:
  MORFEUS_LIB_DECL void addObservation(std::unique_ptr<observation::Observation> observation);
  MORFEUS_LIB_DECL void addSource(std::unique_ptr<sources::Source> source);
  MORFEUS_LIB_DECL observation::Observation * observation(std::size_t index);
  MORFEUS_LIB_DECL const observation::Observation * observation(std::size_t index) const;
  MORFEUS_LIB_DECL sources::Source * source(std::size_t index);
  MORFEUS_LIB_DECL const sources::Source * source(std::size_t index) const;

protected:
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  typedef std::unique_ptr<observation::Observation> ObservationPtr;
  typedef std::unique_ptr<solution::Solver> SolverPtr;
  typedef std::unique_ptr<sources::Source> SourcePtr;
  typedef std::vector<ObservationPtr> ObservationVector;
  typedef std::vector<SourcePtr> SourceVector;

private:
  const mesh::Mesh * mMesh;
  double mFreqIncr;
  double mFreqStart;
  double mFreqStop;
  double mPhiIncr;
  double mPhiStart;
  double mPhiStop;
  double mThetaIncr;
  double mThetaStart;
  double mThetaStop;
  ObservationVector mObservations;
  SourceVector mSources;
  SolverPtr mSolver;
};

inline double Solution::frequencyIncrement() const
{
  return mFreqIncr;
}

inline double Solution::frequencyStart() const
{
  return mFreqStart;
}

inline double Solution::frequencyStop() const
{
  return mFreqStop;
}

inline double Solution::phiIncrement() const
{
  return mPhiIncr;
}

inline double Solution::phiStart() const
{
  return mPhiStart;
}

inline double Solution::phiStop() const
{
  return mPhiStop;
}

inline observation::Observation * Solution::observation(std::size_t index)
{
  return mObservations.at(index).get();
}

inline const observation::Observation * Solution::observation(std::size_t index) const
{
  return mObservations.at(index).get();
}

inline void Solution::setFrequencyIncrement(double value)
{
  mFreqIncr = value;
}

inline void Solution::setFrequencyStart(double value)
{
  mFreqStart = value;
}

inline void Solution::setFrequencyStop(double value)
{
  mFreqStop = value;
}

inline void Solution::setPhiIncrement(double value)
{
  mPhiIncr = value;
}

inline void Solution::setPhiStart(double value)
{
  mPhiStart = value;
}

inline void Solution::setPhiStop(double value)
{
  mPhiStop = value;
}

inline void Solution::setThetaIncrement(double value)
{
  mThetaIncr = value;
}

inline void Solution::setThetaStart(double value)
{
  mThetaStart = value;
}

inline void Solution::setThetaStop(double value)
{
  mThetaStop = value;
}

inline Solver * Solution::solver()
{
  return mSolver.get();
}

inline const Solver * Solution::solver() const
{
  return mSolver.get();
}

inline sources::Source * Solution::source(std::size_t index)
{
  return mSources.at(index).get();
}

inline const sources::Source * Solution::source(std::size_t index) const
{
  return mSources.at(index).get();
}

inline double Solution::thetaIncrement() const
{
  return mThetaIncr;
}

inline double Solution::thetaStart() const
{
  return mThetaStart;
}

inline double Solution::thetaStop() const
{
  return mThetaStop;
}

inline std::size_t Solution::totalSources() const
{
  return mSources.size();
}

inline std::size_t Solution::totalObservations() const
{
  return mObservations.size();
}

}
}

#endif // MORFEUS_SOLUTION_SOLUTION_H
