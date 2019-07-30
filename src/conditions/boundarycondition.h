#ifndef BOUNDARYCONDITION_H
#define BOUNDARYCONDITION_H

#include "morfeusobject.h"

namespace morfeus {

class BoundaryCondition
    : public MorfeusObject
{
public:
  MORFEUS_LIB_DECL int attribute() const;
  MORFEUS_LIB_DECL void setAttribute(int attribute);

protected:
  BoundaryCondition(const std::string & type);

private:
  int mAttribute;
};

inline int BoundaryCondition::attribute() const
{
  return mAttribute;
}

inline void BoundaryCondition::setAttribute(int attribute)
{
  mAttribute = attribute;
}

}

#endif // BOUNDARYCONDITION_H
