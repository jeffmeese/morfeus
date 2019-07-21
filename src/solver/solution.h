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
  MORFEUS_LIB_DECL virtual ~Solution() override;

public:
  MORFEUS_LIB_DECL void addExcitation(std::unique_ptr<Excitation> excitation);
  MORFEUS_LIB_DECL void addObservation(std::unique_ptr<Observation> observation);
  MORFEUS_LIB_DECL Excitation * getExcitation(std::size_t index);
  MORFEUS_LIB_DECL const Excitation * getExcitation(std::size_t index) const;
  MORFEUS_LIB_DECL Observation * getObservation(std::size_t index);
  MORFEUS_LIB_DECL const Observation * getObservation(std::size_t index) const;
  MORFEUS_LIB_DECL void runSolution(double freqGHz, const Mesh * mesh);
  MORFEUS_LIB_DECL void setSolver(std::unique_ptr<Solver> solver);
  MORFEUS_LIB_DECL std::size_t totalExcitations() const;
  MORFEUS_LIB_DECL std::size_t totalObservations() const;

protected:
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  typedef std::unique_ptr<Excitation> ExcitationPtr;
  typedef std::unique_ptr<Observation> ObservationPtr;

private:
  const Mesh * mMesh;
  std::vector<ExcitationPtr> mExcitations;
  std::vector<ObservationPtr> mObservations;
  std::unique_ptr<Solver> mSolver;
};

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

inline std::size_t Solution::totalExcitations() const
{
  return mExcitations.size();
}

inline std::size_t Solution::totalObservations() const
{
  return mObservations.size();
}

#endif // SOLUTION_H
