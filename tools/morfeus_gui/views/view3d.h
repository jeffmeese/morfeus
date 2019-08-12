#ifndef VIEW3D_H
#define VIEW3D_H

#include <QVTKOpenGLNativeWidget.h>

#include <vtkAxesActor.h>
#include <vtkBoundingBox.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkInteractorStyleRubberBandZoom.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include <model/geometry/rectangle.h>
#include <model/geometry/part.h>

class View3d
    : public QVTKOpenGLNativeWidget
{
  Q_OBJECT

public:
  View3d(QWidget * parentWidget = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

public slots:
  void handleAddPart(morfeus::model::geometry::Part * part);

protected:
  void paintGL() override;

public:
  void buildAxes();
  void createParts();
  void createRectangle();

private:
  vtkSmartPointer<vtkAxesActor> mAxesActor;
  vtkSmartPointer<vtkRenderer> mRenderer;
  vtkSmartPointer<vtkInteractorStyleRubberBandZoom> mRubberBandZoom;
  vtkSmartPointer<vtkInteractorStyleTrackballCamera> mTrackballCamera;
  vtkBoundingBox mBoundingBox;
  vtkSmartPointer<vtkPoints> mPoints;
};

#endif // VIEW3D_H
