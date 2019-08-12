#include "view3d.h"

#include <vtkCamera.h>
#include <vtkCaptionActor2D.h>
#include <vtkCellData.h>
#include <vtkMatrix4x4.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolygon.h>
#include <vtkProperty.h>
#include <vtkProperty2D.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>

#include <model/geometry/rectangle.h>
#include <model/geometry/face.h>
#include <model/geometry/facet.h>
#include <model/geometry/vertex.h>

#include <QDebug>

View3d::View3d(QWidget * parentWidget, Qt::WindowFlags flags)
  : QVTKOpenGLNativeWidget (parentWidget, flags)
  , mAxesActor(vtkAxesActor::New())
  , mPoints(vtkPoints::New())
  , mRenderer(vtkRenderer::New())
  , mRubberBandZoom(vtkInteractorStyleRubberBandZoom::New())
  , mTrackballCamera(vtkInteractorStyleTrackballCamera::New())
{
  mRenderer->SetBackground(1.0, 1.0, 1.0);
  mTrackballCamera->SetDefaultRenderer(mRenderer);
  mRenderer->ResetCamera();
  GetRenderWindow()->AddRenderer(mRenderer);
  GetRenderWindow()->GetInteractor()->SetInteractorStyle(mTrackballCamera);
  buildAxes();
}

void View3d::buildAxes()
{
  mAxesActor->AxisLabelsOn();
  mAxesActor->SetShaftTypeToLine();
  mAxesActor->SetTipTypeToCone();
  mAxesActor->SetNormalizedTipLength(0.0, 0.0, 0.0);

  mAxesActor->SetXAxisLabelText("X/U");
  mAxesActor->GetXAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToViewport();
  mAxesActor->GetXAxisCaptionActor2D()->GetTextActor()->GetTextProperty()->SetColor(1.0,0.0,0.0);
  mAxesActor->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
  mAxesActor->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->ShadowOff();

  mAxesActor->SetYAxisLabelText("Y/V");
  mAxesActor->GetYAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToViewport();
  mAxesActor->GetYAxisCaptionActor2D()->GetTextActor()->GetTextProperty()->SetColor(0.0,1.0,0.0);
  mAxesActor->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
  mAxesActor->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->ShadowOff();

  mAxesActor->SetZAxisLabelText("Z/N");
  mAxesActor->GetZAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToViewport();
  mAxesActor->GetZAxisCaptionActor2D()->GetTextActor()->GetTextProperty()->SetColor(0.0,0.0,1.0);
  mAxesActor->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
  mAxesActor->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->ShadowOff();

  mRenderer->AddActor(mAxesActor);
}

void View3d::createParts()
{
}

void View3d::handleAddPart(morfeus::model::geometry::Part * part)
{
  using namespace morfeus::model::geometry;

  auto points = vtkSmartPointer<vtkPoints>::New();
  auto vertexList = part->getVertexList();
  for (std::size_t i = 0; i < vertexList.size(); i++) {
    const Vertex * vertex = vertexList.at(i);
    points->InsertNextPoint(vertex->x(), vertex->y(), vertex->z());
  }

  auto faceList = part->getFaceList();
  if (faceList.size() > 0) {
    for (std::size_t i = 0; i < faceList.size(); i++) {
      auto face = faceList.at(i);

      // Add all the face polygons
      auto polygons = vtkSmartPointer<vtkCellArray>::New();
      for (std::size_t j = 0; j < face->totalFacets(); j++) {
        auto facet = face->facet(j);
        auto polygon = vtkSmartPointer<vtkPolygon>::New();

        std::size_t totalVertices = facet->totalVertices();
        polygon->GetPointIds()->SetNumberOfIds(totalVertices);
        for (std::size_t k = 0; k < totalVertices; k++) {
          const Vertex * vertex = facet->vertex(k);
          polygon->GetPointIds()->SetId(k, vertex->number());
        }
        polygons->InsertNextCell(polygon);
      }

      // Create a PolyData
      auto polygonPolyData = vtkSmartPointer<vtkPolyData>::New();
      polygonPolyData->SetPoints(points);
      polygonPolyData->SetPolys(polygons);

      // Create a mapper and actor
      auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
      mapper->SetInputData(polygonPolyData);

      auto actor = vtkSmartPointer<vtkActor>::New();
      actor->SetMapper(mapper);

      actor->GetProperty()->SetColor(30.0/255, 144.0/255, 255.0/255);
      mRenderer->AddActor(actor);
    }
  }
  this->update();
}

void View3d::paintGL()
{
  QVTKOpenGLNativeWidget::paintGL();
  mRenderer->GetRenderWindow()->Render();
}
