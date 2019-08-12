#ifndef MORFEUS_CORE_MORFEUSPROJECT_H
#define MORFEUS_CORE_MORFEUSPROJECT_H

#include "morfeus.h"

#include "xml/rapidxml.hpp"
#include "xml/xmlutils.h"

#include <memory>

namespace morfeus {

  namespace model {
    class Model;
  }

  namespace media {
    class MediaLibrary;
  }

  namespace mesher {
    class Mesher;
  }

  namespace solution {
    class Solution;
  }
}

namespace morfeus {
namespace core {

class MorfeusProject
{
public:
  MORFEUS_LIB_DECL MorfeusProject();
  MORFEUS_LIB_DECL virtual ~MorfeusProject();

public:
  MORFEUS_LIB_DECL bool modified() const;
  MORFEUS_LIB_DECL std::string name() const;
  MORFEUS_LIB_DECL void setModified(bool modified = true);
  MORFEUS_LIB_DECL void setName(const std::string & name);

public:
  MORFEUS_LIB_DECL model::Model * model();
  MORFEUS_LIB_DECL const model::Model * model() const;
  MORFEUS_LIB_DECL mesher::Mesher * mesher();
  MORFEUS_LIB_DECL const mesher::Mesher * mesher() const;
  MORFEUS_LIB_DECL solution::Solution * solution();
  MORFEUS_LIB_DECL const solution::Solution * solution() const;

public:
  MORFEUS_LIB_DECL void print(std::ostream & output, int tabPos = 0) const;
  MORFEUS_LIB_DECL void print(int tabPos = 0) const;
  MORFEUS_LIB_DECL void readFromFile(const std::string & fileName);
  MORFEUS_LIB_DECL void readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node);
  MORFEUS_LIB_DECL void saveToFile(const std::string & fileName);
  MORFEUS_LIB_DECL void validate() const;
  MORFEUS_LIB_DECL void writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const;

private:
  bool mModified;
  std::string mName;
  std::unique_ptr<model::Model> mModel;
  std::unique_ptr<mesher::Mesher> mMesher;
  std::unique_ptr<solution::Solution> mSolution;
};

inline model::Model * MorfeusProject::model()
{
  return mModel.get();
}

inline const model::Model * MorfeusProject::model() const
{
  return mModel.get();
}

inline bool MorfeusProject::modified() const
{
  return mModified;
}

inline mesher::Mesher * MorfeusProject::mesher()
{
  return mMesher.get();
}

inline const mesher::Mesher * MorfeusProject::mesher() const
{
  return mMesher.get();
}

inline std::string MorfeusProject::name() const
{
  return mName;
}

inline void MorfeusProject::setModified(bool value)
{
  mModified = value;
}

inline void MorfeusProject::setName(const std::string &name)
{
  mName = name;
}

inline solution::Solution * MorfeusProject::solution()
{
  return mSolution.get();
}

inline const solution::Solution * MorfeusProject::solution() const
{
  return mSolution.get();
}

}
}

#endif // MORFEUS_CORE_MORFEUSPROJECT_H
