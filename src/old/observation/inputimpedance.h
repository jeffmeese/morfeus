#ifndef INPUTIMPEDANCE_H
#define INPUTIMPEDANCE_H

#include "morfeus.h"

#include "observation/observation.h"

class InputImpedance
    : public Observation
{
public:
  MORFEUS_LIB_DECL InputImpedance(std::size_t id);
  MORFEUS_LIB_DECL InputImpedance(std::size_t id, const std::string & name);
  MORFEUS_LIB_DECL InputImpedance(std::size_t id, const std::string & name, std::size_t feedId);

public:
  MORFEUS_LIB_DECL std::size_t feedId() const;
  MORFEUS_LIB_DECL void setFeedId(std::size_t id);

public:
  MORFEUS_LIB_DECL virtual void calculate(const Solution * solution, const Mesh & mesh, double freq, const dcomplex_array & efield);

private:
  std::size_t mFeedId;
};

#endif // INPUTIMPEDANCE_H
