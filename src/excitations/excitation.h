#ifndef EXCITATION_H
#define EXCITATION_H

#include "morfeus.h"

#include <boost/numeric/ublas/vector.hpp>

class Mesh;
class MeshInformation;

class Excitation
{
protected:
  typedef boost::numeric::ublas::vector<dcomplex> vector;

public:
  MORFEUS_LIB_DECL virtual ~Excitation();

public:
  MORFEUS_LIB_DECL void excite(double freqGHz, const Mesh * mesh, const MeshInformation * meshInfo, vector & rhs) const;

protected:
  Excitation(int32_t id);

protected:
  virtual void doExcite(double freqGHz, const Mesh * mesh, const MeshInformation * meshInfo, vector & rhs) const = 0;

private:
  int32_t mId;
};

#endif // EXCITATION_H
