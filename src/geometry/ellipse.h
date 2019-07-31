#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "surface.h"

namespace morfeus {
namespace geometry {

class Ellipse
    : public Surface
{
public:
  Ellipse();
  Ellipse(const std::string & name);
  Ellipse(const std::string & id, const std::string & name);

protected:
  std::vector<Face*> doGetFaceList() const override;
  std::vector<Segment*> doGetSegmentList() const override;
  void doPrint(std::ostream & output, int tabPos = 0) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;
};

}
}

#endif // ELLIPSE_H
