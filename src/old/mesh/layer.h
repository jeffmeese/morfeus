#ifndef LAYER_H
#define LAYER_H

#include "morfeus.h"

#include <string>

class Layer
{
public:
  MORFEUS_LIB_DECL Layer(std::size_t id, double thickness);
  MORFEUS_LIB_DECL Layer(std::size_t id, double thickness, const std::string & name);

public:
  MORFEUS_LIB_DECL std::size_t id() const;
  MORFEUS_LIB_DECL std::string name() const;
  MORFEUS_LIB_DECL double thickness() const;
  MORFEUS_LIB_DECL void setName(const std::string & name);
  MORFEUS_LIB_DECL void setThickness(double thickness);

private:
  std::size_t mId;
  std::string mName;
  double mThickness;
};

#endif // LAYER_H
