#ifndef SEGMENT_H
#define SEGMENT_H

#include "morfeus.h"
#include "morfeusobject.h"

class Segment
    : public MorfeusObject
{
public:
  MORFEUS_LIB_DECL Segment();
  MORFEUS_LIB_DECL Segment(const std::string & id);
  MORFEUS_LIB_DECL Segment(int32_t number);
  MORFEUS_LIB_DECL Segment(const std::string & id, int32_t number);
  MORFEUS_LIB_DECL Segment(int32_t node1, int32_t node2);
  MORFEUS_LIB_DECL Segment(const std::string & id, int32_t node1, int32_t node2);
  MORFEUS_LIB_DECL Segment(int32_t number, int32_t node1, int32_t node2);
  MORFEUS_LIB_DECL Segment(const std::string & id, int32_t number, int32_t node1, int32_t node2);

public:
  MORFEUS_LIB_DECL bool boundary() const;
  MORFEUS_LIB_DECL int32_t node1() const;
  MORFEUS_LIB_DECL int32_t node2() const;
  MORFEUS_LIB_DECL int32_t number() const;
  MORFEUS_LIB_DECL void setBoundary(bool value);
  MORFEUS_LIB_DECL void setNode1(int32_t value);
  MORFEUS_LIB_DECL void setNode2(int32_t value);
  MORFEUS_LIB_DECL void setNumber(int32_t value);
  MORFEUS_LIB_DECL void setNodes(int32_t node1, int32_t node2);

public:
  MORFEUS_LIB_DECL void print(std::ostream & output, int tabPos = 0) const;
  MORFEUS_LIB_DECL void print(int tabPos = 0) const;
  MORFEUS_LIB_DECL void readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node);
  MORFEUS_LIB_DECL void writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const;
  MORFEUS_LIB_DECL friend std::ostream & operator<<(std::ostream & output, const Segment & object);

private:
  bool mBoundary;
  int32_t mNode1;
  int32_t mNode2;
  int32_t mNumber;
};

inline int32_t Segment::node1() const
{
  return mNode1;
}

inline int32_t Segment::node2() const
{
  return mNode2;
}

inline int32_t Segment::number() const
{
  return mNumber;
}

inline void Segment::setBoundary(bool value)
{
  mBoundary = value;
}

inline void Segment::setNode1(int32_t value)
{
  mNode1 = value;
}

inline void Segment::setNode2(int32_t value)
{
  mNode2 = value;
}

inline void Segment::setNumber(int32_t value)
{
  mNumber = value;
}
#endif // SEGMENT_H
