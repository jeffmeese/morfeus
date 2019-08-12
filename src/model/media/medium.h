#ifndef MEDIUM_H
#define MEDIUM_H

#include "core/morfeusobject.h"

#include "math/types.h"

namespace morfeus {
namespace model {
namespace media {

class Medium
    : public core::MorfeusObject
{
public:
  MORFEUS_LIB_DECL int32_t attribute() const;
  MORFEUS_LIB_DECL void setAttribute(int32_t value);

public:
  MORFEUS_LIB_DECL math::dcomplex value(double freqGHz) const;

protected:
  Medium(const std::string & type);
  Medium(const std::string & type, const std::string & name);
  Medium(const std::string & type, const std::string & id, const std::string & name);

protected:
  virtual math::dcomplex doValue(double freqGHz) const = 0;

private:
  int32_t mAttribute;
};

inline int32_t Medium::attribute() const
{
  return mAttribute;
}

inline void Medium::setAttribute(int32_t value)
{
  mAttribute = value;
}

}
}
}

#endif // MEDIUM_H
