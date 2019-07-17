#ifndef ITERATIVESOLUTION_H
#define ITERATIVESOLUTION_H

#include "solution.h"

class IterativeSolution
    : public Solution
{
public:
  IterativeSolution(const Mesh & mesh);

protected:
  void doRunSolution(double freqGHz) override;
};

#endif // ITERATIVESOLUTION_H
