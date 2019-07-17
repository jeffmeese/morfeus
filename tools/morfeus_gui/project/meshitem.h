#ifndef MESHITEM_H
#define MESHITEM_H

#include "projectitem.h"

class Mesh;

class MeshItem
    : public ProjectItem
{
public:
  MeshItem(std::unique_ptr<Mesh> mesh);

protected:
  bool loadAttributes(QXmlStreamReader & reader) override;
  bool saveAttributes(QXmlStreamWriter & writer) const override;

private:
  std::unique_ptr<Mesh> mMesh;
};

#endif // MESHITEM_H
