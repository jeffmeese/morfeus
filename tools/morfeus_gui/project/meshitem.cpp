#include "meshitem.h"

#include <mesh.h>

MeshItem::MeshItem(std::unique_ptr<Morfeus::mesh::Mesh> mesh)
  : ProjectItem ("Mesh")
  , mMesh(std::move(mesh))
{
}

bool MeshItem::loadAttributes(QXmlStreamReader & reader)
{
  return !reader.hasError();
}

bool MeshItem::saveAttributes(QXmlStreamWriter & writer) const
{
  return !writer.hasError();
}

