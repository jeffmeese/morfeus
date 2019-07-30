#ifndef MESHITEM_H
#define MESHITEM_H

#include "projectitem.h"
#include "mesh/mesh.h"

class MeshItem
    : public ProjectItem
{
public:
  MeshItem(std::unique_ptr<morfeus::mesh::Mesh> mesh);

protected:
  bool loadAttributes(QXmlStreamReader & reader) override;
  bool saveAttributes(QXmlStreamWriter & writer) const override;

private:
  std::unique_ptr<morfeus::mesh::Mesh> mMesh;
};

#endif // MESHITEM_H
