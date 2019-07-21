#ifndef SEGMENT_H
#define SEGMENT_H

#include "morfeus.h"
#include "morfeusobject.h"

class Segment
    : public MorfeusObject
{
public:
  MORFEUS_LIB_DECL Segment();
  MORFEUS_LIB_DECL Segment(int32_t node1, int32_t node2);
  MORFEUS_LIB_DECL Segment(int32_t number, int32_t node1, int32_t node2);

public:
  MORFEUS_LIB_DECL bool boundary() const;
  MORFEUS_LIB_DECL int32_t node1() const;
  MORFEUS_LIB_DECL int32_t node2() const;
  MORFEUS_LIB_DECL void setBoundary(bool value);
  MORFEUS_LIB_DECL void setNode1(int32_t node1);
  MORFEUS_LIB_DECL void setNode2(int32_t node2);
  MORFEUS_LIB_DECL void setNodes(int32_t node1, int32_t node2);

protected:
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  bool mBoundary;
  int32_t mNode1;
  int32_t mNode2;
};

#endif // SEGMENT_H
