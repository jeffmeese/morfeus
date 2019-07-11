#ifndef SOLUTION_H
#define SOLUTION_H

#include "morfeus.h"

#include <memory>
#include <vector>

class Excitation;
class Mesh;
class Observation;

class Solution
{
public:
  MORFEUS_LIB_DECL Solution(const Mesh & mesh);
  MORFEUS_LIB_DECL virtual ~Solution();

public:
  MORFEUS_LIB_DECL void addExcitation(std::unique_ptr<Excitation> excitation);
  MORFEUS_LIB_DECL void addObservation(std::unique_ptr<Observation> observation);

private:
  typedef std::unique_ptr<Excitation> ExcitationPtr;
  typedef std::unique_ptr<Observation> ObservationPtr;

private:
  const Mesh & mMesh;
  std::vector<ExcitationPtr> mExcitations;
  std::vector<ObservationPtr> mObservations;
};

#endif // SOLUTION_H
