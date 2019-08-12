#ifndef DETAILSVIEW_H
#define DETAILSVIEW_H

#include <QTreeView>

#include <model/geometry/part.h>

class DetailsView
    : public QTreeView
{
public:
  DetailsView(QWidget * parentWidget = nullptr);

public:
  void displayPartProperties(morfeus::model::geometry::Part * part);
};

#endif // DETAILSVIEW_H
