#include "solution.h"

#include "iterativesolver.h"
#include "excitation.h"
#include "observation.h"
#include "solver.h"

Solution::Solution(const Mesh * mesh)
  : mMesh(mesh)
  , mSolver(new IterativeSolver)
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

void Solution::runSolution(double freqGHz)
{
  mSolver->runSolver(freqGHz, mMesh, this);
}

void Solution::setSolver(std::unique_ptr<Solver> solver)
{
  mSolver = std::move(solver);
}
