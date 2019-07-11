#ifndef EXCITATION_H
#define EXCITATION_H

#include "morfeus.h"

class Mesh;

class Excitation
{
public:
  MORFEUS_LIB_DECL virtual ~Excitation();

public:
  MORFEUS_LIB_DECL void excite(const Mesh & mesh);

protected:
  Excitation(int32_t id);

protected:
  virtual void doExcite(const Mesh & mesh) = 0;

private:
  int32_t mId;
};

#endif // EXCITATION_H
