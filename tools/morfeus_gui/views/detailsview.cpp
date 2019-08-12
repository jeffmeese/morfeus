#include "detailsview.h"

#include <model/geometry/face.h>
#include <model/geometry/region.h>
#include <model/geometry/segment.h>
#include <model/geometry/wire.h>

#include <QStandardItemModel>

DetailsView::DetailsView(QWidget * parentWidget)
  : QTreeView(parentWidget)
{
}

void DetailsView::displayPartProperties(morfeus::model::geometry::Part *part)
{
  QStandardItemModel * model = new QStandardItemModel;

  std::vector<morfeus::model::geometry::Segment*> segmentList = part->getSegmentList();
  std::vector<morfeus::model::geometry::Face*> faceList = part->getFaceList();
  std::vector<morfeus::model::geometry::Region*> regionList = part->getRegionList();
  std::vector<morfeus::model::geometry::Wire*> wireList = part->getWireList();

  if (segmentList.size() > 0) {
    QStandardItem * headerItem = new QStandardItem("Segments");
    for (std::size_t i = 0; i < segmentList.size(); i++) {
      const morfeus::model::geometry::Segment * segment = segmentList.at(i);
      QStandardItem * item = new QStandardItem(QString::fromStdString(segment->name()));
      headerItem->appendRow(item);
    }
    model->appendRow(headerItem);
  }

  if (faceList.size() > 0) {
    QStandardItem * headerItem = new QStandardItem("Faces");
    for (std::size_t i = 0; i < faceList.size(); i++) {
      const morfeus::model::geometry::Face * face = faceList.at(i);
      QStandardItem * item = new QStandardItem(QString::fromStdString(face->name()));
      headerItem->appendRow(item);
    }
    model->appendRow(headerItem);
  }

  if (regionList.size() > 0) {
    QStandardItem * headerItem = new QStandardItem("Regions");
    for (std::size_t i = 0; i < regionList.size(); i++) {
      const morfeus::model::geometry::Region * region = regionList.at(i);
      QStandardItem * item = new QStandardItem(QString::fromStdString(region->name()));
      headerItem->appendRow(item);
    }
    model->appendRow(headerItem);
  }

  if (wireList.size() > 0) {
    QStandardItem * headerItem = new QStandardItem("Wires");
    for (std::size_t i = 0; i < wireList.size(); i++) {
      const morfeus::model::geometry::Wire * wire = wireList.at(i);
      QStandardItem * item = new QStandardItem(QString::fromStdString(wire->name()));
      headerItem->appendRow(item);
    }
    model->appendRow(headerItem);
  }

  this->setModel(model);
}
