#include "model.h"

#include "geometry/part.h"
#include "model/ports/port.h"
#include "media/medium.h"

namespace morfeus {
namespace model {

static const std::string OBJECT_ID("Model");

Model::Model()
  : core::MorfeusObject (OBJECT_ID)
{
}

Model::~Model()
{
}

void Model::addMedium(std::unique_ptr<media::Medium> medium)
{
  mMedia.push_back(std::move(medium));
}

void Model::addPart(std::unique_ptr<geometry::Part> part)
{
  mParts.push_back(std::move(part));
}

void Model::addPort(std::unique_ptr<ports::Port> port)
{
  mPorts.push_back(std::move(port));
}

void Model::doPrint(std::ostream &output, int tabPos) const
{
}

void Model::doXmlRead(rapidxml::xml_document<> &document, rapidxml::xml_node<> *node)
{
}

void Model::doXmlWrite(rapidxml::xml_document<> &document, rapidxml::xml_node<> *node) const
{
}

}
}


