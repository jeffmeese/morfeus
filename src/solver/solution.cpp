#include "solution.h"

#include "excitation.h"
#include "observation.h"

Solution::Solution(const Mesh & mesh)
  : mMesh(mesh)
{
}

Solution::~Solution()
{

}

void Solution::addExcitation(std::unique_ptr<Excitation> excitation)
{
  mExcitations.push_back(std::move(excitation));
}

void Solution::addObservation(std::unique_ptr<Observation> observation)
{
  mObservations.push_back(std::move(observation));
}
