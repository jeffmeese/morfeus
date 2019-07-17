#ifndef SOLUTION_H
#define SOLUTION_H

#include "morfeus.h"

#include <boost/noncopyable.hpp>

#include <memory>
#include <vector>

class Excitation;
class Mesh;
class Observation;
class Solver;

class Solution
    : private boost::noncopyable
{
public:
  MORFEUS_LIB_DECL void addExcitation(std::unique_ptr<Excitation> excitation);
  MORFEUS_LIB_DECL void addObservation(std::unique_ptr<Observation> observation);
  MORFEUS_LIB_DECL void setSolver(std::unique_ptr<Solver> solver);

public:
  MORFEUS_LIB_DECL void runSolution(double freqGHz);

protected:
  Solution(const Mesh & mesh);

protected:
  virtual void doRunSolution(double freqGHz);

private:
  typedef std::unique_ptr<Excitation> ExcitationPtr;
  typedef std::unique_ptr<Observation> ObservationPtr;

private:
  const Mesh & mMesh;
  std::vector<ExcitationPtr> mExcitations;
  std::vector<ObservationPtr> mObservations;
  std::unique_ptr<Solver> mSolver;
};

#endif // SOLUTION_H
