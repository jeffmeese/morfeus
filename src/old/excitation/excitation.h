#ifndef EXCITATION_H
#define EXCITATION_H

#include "morfeus.h"
#include "types.h"

#include <cstddef>

class Mesh;

class Excitation
{
public:
  MORFEUS_LIB_DECL Excitation(std::size_t id);

public:
  MORFEUS_LIB_DECL std::size_t id() const;

public:
  MORFEUS_LIB_DECL virtual void excite(const Mesh & mesh, double freq, dcomplex_array & rhs) const = 0;

private:
  std::size_t mId;
};

#endif // EXCITATION_H
