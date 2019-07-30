#ifndef MORFEUSPROJECT_H
#define MORFEUSPROJECT_H

#include "morfeus.h"
#include "rapidxml.hpp"
#include "xmlutils.h"

#include <memory>

namespace Morfeus {
  namespace Geometry {
    class Model;
  }
  class MaterialDatabase;
  class Mesher;
  class Solution;
}

namespace Morfeus {

class MorfeusProject
{
public:
  MORFEUS_LIB_DECL MorfeusProject();

public:
  MORFEUS_LIB_DECL Geometry::Model * model();
  MORFEUS_LIB_DECL const Geometry::Model * model() const;
  MORFEUS_LIB_DECL MaterialDatabase * materialDatabase();
  MORFEUS_LIB_DECL const MaterialDatabase * materialDatabase() const;
  MORFEUS_LIB_DECL Mesher * mesher();
  MORFEUS_LIB_DECL const Mesher * mesher() const;
  MORFEUS_LIB_DECL Solution * solution();
  MORFEUS_LIB_DECL const Solution * solution() const;

public:
  MORFEUS_LIB_DECL void print(std::ostream & output, int tabPos = 0) const;
  MORFEUS_LIB_DECL void print(int tabPos = 0) const;
  MORFEUS_LIB_DECL void readFromFile(const std::string & fileName);
  MORFEUS_LIB_DECL void readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node);
  MORFEUS_LIB_DECL void saveToFile(const std::string & fileName);
  MORFEUS_LIB_DECL void validate() const;
  MORFEUS_LIB_DECL void writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const;


private:
  std::unique_ptr<Geometry::Model> mModel;
  std::unique_ptr<MaterialDatabase> mMaterialDatabase;
  std::unique_ptr<Mesher> mMesher;
  std::unique_ptr<Solution> mSolution;
};

inline Geometry::Model * MorfeusProject::model()
{
  return mModel.get();
}

inline const Geometry::Model * MorfeusProject::model() const
{
  return mModel.get();
}

inline MaterialDatabase * MorfeusProject::materialDatabase()
{
  return mMaterialDatabase.get();
}

inline const MaterialDatabase * MorfeusProject::materialDatabase() const
{
  return mMaterialDatabase.get();
}

inline Mesher * MorfeusProject::mesher()
{
  return mMesher.get();
}

inline const Mesher * MorfeusProject::mesher() const
{
  return mMesher.get();
}

inline Solution * MorfeusProject::solution()
{
  return mSolution.get();
}

inline const Solution * MorfeusProject::solution() const
{
  return mSolution.get();
}

}

#endif // MORFEUSPROJECT_H
