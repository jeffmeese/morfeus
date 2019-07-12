#ifndef SHAPE_H
#define SHAPE_H

#include "morfeus.h"

#include <string>

class Shape
{
public:
  MORFEUS_LIB_DECL std::string name() const;
  MORFEUS_LIB_DECL void setName(const std::string & name);

protected:
  Shape();

private:
  std::string mName;
};

#endif // SHAPE_H
