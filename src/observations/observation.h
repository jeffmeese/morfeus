#ifndef OBSERVATION_H
#define OBSERVATION_H

#include "morfeus.h"
#include "types.h"

class Observation
{
public:
  MORFEUS_LIB_DECL virtual ~Observation();

protected:
  Observation(int32_t id);

private:
  int32_t mId;
};

#endif // OBSERVATION_H
