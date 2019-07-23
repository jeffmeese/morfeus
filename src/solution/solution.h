#ifndef SOLUTION_H
#define SOLUTION_H

#include "morfeus.h"
#include "morfeusobject.h"

#include <boost/noncopyable.hpp>

#include <memory>
#include <vector>

class Excitation;
class MaterialDatabase;
class Mesh;
class Observation;
class Solver;

class Solution
    : private boost::noncopyable
    , public MorfeusObject
{
public:
  MORFEUS_LIB_DECL Solution();
  MORFEUS_LIB_DECL Solution(const std::string & id);

public:
  MORFEUS_LIB_DECL double frequencyIncrement() const;
  MORFEUS_LIB_DECL double frequencyStart() const;
  MORFEUS_LIB_DECL double frequencyStop() const;
  MORFEUS_LIB_DECL void setFrequencyIncrement(double value);
  MORFEUS_LIB_DECL void setFrequencyStart(double value);
  MORFEUS_LIB_DECL void setFrequencyStop(double value);

public:
  MORFEUS_LIB_DECL void addExcitation(std::unique_ptr<Excitation> excitation);
  MORFEUS_LIB_DECL void addObservation(std::unique_ptr<Observation> observation);
  MORFEUS_LIB_DECL Excitation * getExcitation(std::size_t index);
  MORFEUS_LIB_DECL const Excitation * getExcitation(std::size_t index) const;
  MORFEUS_LIB_DECL Observation * getObservation(std::size_t index);
  MORFEUS_LIB_DECL const Observation * getObservation(std::size_t index) const;
  MORFEUS_LIB_DECL void print(std::ostream & output, int tabPos = 0) const;
  MORFEUS_LIB_DECL void print(int tabPos = 0) const;
  MORFEUS_LIB_DECL void readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node);
  MORFEUS_LIB_DECL void writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const;
  MORFEUS_LIB_DECL friend std::ostream & operator<<(std::ostream & output, const Excitation & object);
  MORFEUS_LIB_DECL void runSolution(const Mesh * mesh);
  MORFEUS_LIB_DECL void setSolver(std::unique_ptr<Solver> solver);
  MORFEUS_LIB_DECL std::size_t totalExcitations() const;
  MORFEUS_LIB_DECL std::size_t totalObservations() const;

private:
  typedef std::unique_ptr<Excitation> ExcitationPtr;
  typedef std::unique_ptr<Observation> ObservationPtr;

private:
  const Mesh * mMesh;
  double mFreqIncr;
  double mFreqStart;
  double mFreqStop;
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

inline Excitation * Solution::getExcitation(std::size_t index)
{
  return mExcitations.at(index).get();
}

inline const Excitation * Solution::getExcitation(std::size_t index) const
{
  return mExcitations.at(index).get();
}

inline Observation * Solution::getObservation(std::size_t index)
{
  return mObservations.at(index).get();
}

inline const Observation * Solution::getObservation(std::size_t index) const
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

inline std::size_t Solution::totalExcitations() const
{
  return mExcitations.size();
}

inline std::size_t Solution::totalObservations() const
{
  return mObservations.size();
}

#endif // SOLUTION_H
