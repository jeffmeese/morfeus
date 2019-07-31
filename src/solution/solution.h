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

  namespace excitation {
    class Excitation;
  }

  namespace material {
    class MaterialDatabase;
  }

  namespace mesh {
    class Mesh;
  }

  namespace observation {
    class Observation;
  }

  namespace solution {
    class Solver;
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
  MORFEUS_LIB_DECL std::size_t totalExcitations() const;
  MORFEUS_LIB_DECL std::size_t totalObservations() const;

public:
  MORFEUS_LIB_DECL void addExcitation(std::unique_ptr<excitation::Excitation> excitation);
  MORFEUS_LIB_DECL void addObservation(std::unique_ptr<observation::Observation> observation);
  MORFEUS_LIB_DECL excitation::Excitation * getExcitation(std::size_t index);
  MORFEUS_LIB_DECL const excitation::Excitation * getExcitation(std::size_t index) const;
  MORFEUS_LIB_DECL observation::Observation * getObservation(std::size_t index);
  MORFEUS_LIB_DECL const observation::Observation * getObservation(std::size_t index) const;

protected:
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  typedef std::unique_ptr<excitation::Excitation> ExcitationPtr;
  typedef std::unique_ptr<observation::Observation> ObservationPtr;

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
  std::vector<ExcitationPtr> mExcitations;
  std::vector<ObservationPtr> mObservations;
  std::unique_ptr<Solver> mSolver;
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

inline Solver * Solution::solver()
{
  return mSolver.get();
}

inline const Solver * Solution::solver() const
{
  return mSolver.get();
}

inline excitation::Excitation * Solution::getExcitation(std::size_t index)
{
  return mExcitations.at(index).get();
}

inline const excitation::Excitation * Solution::getExcitation(std::size_t index) const
{
  return mExcitations.at(index).get();
}

inline observation::Observation * Solution::getObservation(std::size_t index)
{
  return mObservations.at(index).get();
}

inline const observation::Observation * Solution::getObservation(std::size_t index) const
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

inline std::size_t Solution::totalExcitations() const
{
  return mExcitations.size();
}

inline std::size_t Solution::totalObservations() const
{
  return mObservations.size();
}

}
}

#endif // MORFEUS_SOLUTION_SOLUTION_H
