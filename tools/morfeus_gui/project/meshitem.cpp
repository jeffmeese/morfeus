#include "meshitem.h"

#include <mesh.h>

MeshItem::MeshItem()
  : ProjectItem("Mesh")
  , mMesh(nullptr)
{
}

MeshItem::MeshItem(Mesh * mesh)
  : ProjectItem ("Mesh")
  , mMesh(nullptr)
{
  setMesh(mesh);
}

bool MeshItem::loadAttributes(QXmlStreamReader & reader)
{
  return !reader.hasError();
}

bool MeshItem::saveAttributes(QXmlStreamWriter & writer) const
{
  return !writer.hasError();
}

void MeshItem::setMesh(Mesh *mesh)
{
  mMesh = mesh;
}
