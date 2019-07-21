#ifndef EXCITATION_H
#define EXCITATION_H

#include "morfeus.h"
#include "morfeusobject.h"

#include <boost/numeric/ublas/vector.hpp>

#include <Factory.h>
#include <Singleton.h>

class Mesh;
class MeshInformation;

class Excitation
    : public MorfeusObject
{
protected:
  typedef boost::numeric::ublas::vector<dcomplex> vector;

public:
  MORFEUS_LIB_DECL void excite(double freqGHz, const Mesh * mesh, const MeshInformation * meshInfo, vector & rhs) const;

public:
  typedef Loki::SingletonHolder<Loki::Factory<Excitation, std::string> > Factory;

protected:
  Excitation();
  Excitation(int32_t number);
  Excitation(const std::string & name, int32_t number);

protected:
  virtual void doExcite(double freqGHz, const Mesh * mesh, const MeshInformation * meshInfo, vector & rhs) const = 0;

private:
};

#endif // EXCITATION_H
