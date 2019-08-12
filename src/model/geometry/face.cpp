#include "face.h"

#include "facet.h"
#include "polygon.h"
#include "vertex.h"

#include "model/model.h"

#include <boost/geometry/arithmetic/cross_product.hpp>
#include <boost/qvm/vec_operations.hpp>
#include <boost/qvm/vec.hpp>

namespace morfeus {
namespace model {
namespace geometry {

int32_t Face::mTotal = 0;
static const std::string OBJECT_ID("Face");

Face::Face()
  : MorfeusObject(OBJECT_ID)
  , mMedium(nullptr)
{
  init();
  setName(createName(mNumber));
}

Face::Face(const std::string & name)
  : MorfeusObject(OBJECT_ID, name)
  , mMedium(nullptr)
{
  init();
}

Face::Face(const std::string & id, const std::string & name)
  : MorfeusObject(OBJECT_ID, id, name)
  , mMedium(nullptr)
{
  init();
}

void Face::addFacet(std::unique_ptr<Facet> facet)
{
  mFacets.push_back(std::move(facet));
}

void Face::doPrint(std::ostream & output, int tabPos) const
{

}

void Face::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{

}

void Face::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{

}

void Face::init()
{
  mNumber = mTotal++;
  mLocalMeshSize = -1.0;
  mMedium = nullptr;
}

}
}
}
