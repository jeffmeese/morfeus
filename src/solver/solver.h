#ifndef SOLVER_H
#define SOLVER_H

#include "morfeus.h"

#include <boost/noncopyable.hpp>

class Solver : private boost::noncopyable
{
public:
  MORFEUS_LIB_DECL void runSolver();
};

#endif // SOLVER_H
