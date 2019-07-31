#ifndef MORFEUS_CORE_MORFEUSPROJECT_H
#define MORFEUS_CORE_MORFEUSPROJECT_H

#include "morfeus.h"

#include "xml/rapidxml.hpp"
#include "xml/xmlutils.h"

#include <memory>

namespace morfeus {

  namespace geometry {
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

public:
  MORFEUS_LIB_DECL geometry::Model * model();
  MORFEUS_LIB_DECL const geometry::Model * model() const;
  MORFEUS_LIB_DECL media::MediaLibrary * mediaLibrary();
  MORFEUS_LIB_DECL const media::MediaLibrary * mediaLibrary() const;
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
  std::unique_ptr<geometry::Model> mModel;
  std::unique_ptr<media::MediaLibrary> mMediaLibrary;
  std::unique_ptr<mesher::Mesher> mMesher;
  std::unique_ptr<solution::Solution> mSolution;
};

inline geometry::Model * MorfeusProject::model()
{
  return mModel.get();
}

inline const geometry::Model * MorfeusProject::model() const
{
  return mModel.get();
}

inline media::MediaLibrary * MorfeusProject::mediaLibrary()
{
  return mMediaLibrary.get();
}

inline const media::MediaLibrary * MorfeusProject::mediaLibrary() const
{
  return mMediaLibrary.get();
}

inline mesher::Mesher * MorfeusProject::mesher()
{
  return mMesher.get();
}

inline const mesher::Mesher * MorfeusProject::mesher() const
{
  return mMesher.get();
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
