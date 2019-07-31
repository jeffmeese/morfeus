#ifndef MEDIUM_H
#define MEDIUM_H

#include "core/morfeusobject.h"

namespace morfeus {
namespace media {

class Medium
    : public core::MorfeusObject
{
public:
  MORFEUS_LIB_DECL std::string name() const;
  MORFEUS_LIB_DECL void setName(const std::string & name);

public:
  MORFEUS_LIB_DECL dcomplex value(double freqGHz) const;

protected:
  Medium(const std::string & type);
  Medium(const std::string & type, const std::string & name);
  Medium(const std::string & type, const std::string & id, const std::string & name);

protected:
  virtual dcomplex doValue(double freqGHz) const = 0;

private:
  std::string mName;
};

inline std::string Medium::name() const
{
  return mName;
}

inline void Medium::setName(const std::string & name)
{
  mName = name;
}

}
}

#endif // MEDIUM_H
