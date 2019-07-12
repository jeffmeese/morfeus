#ifndef MESHITEM_H
#define MESHITEM_H

#include "projectitem.h"

class Mesh;

class MeshItem
    : public ProjectItem
{
public:
  MeshItem();
  MeshItem(Mesh * mesh);

public:
  void setMesh(Mesh * mesh);

protected:
  bool loadAttributes(QXmlStreamReader & reader) override;
  bool saveAttributes(QXmlStreamWriter & writer) const override;

private:
  Mesh * mMesh;
};

#endif // MESHITEM_H
