#include "Scene_plane_item.h"
#include <CGAL/Three/Triangle_container.h>
#include <CGAL/Three/Edge_container.h>
#include <QApplication>
using namespace CGAL::Three;
typedef Triangle_container Tc;
typedef Edge_container Ec;
typedef Viewer_interface VI;


Scene_plane_item::Scene_plane_item(const CGAL::Three::Scene_interface* scene_interface)
  :scene(scene_interface),
    manipulable(false),
    can_clone(true),
    frame(new ManipulatedFrame())
  {
    setNormal(0., 0., 1.);
    setTriangleContainer(0, new Tc(VI::PROGRAM_NO_SELECTION,
                                                 false));
    setEdgeContainer(0, new Ec(VI::PROGRAM_NO_SELECTION,
                                                 false));
    //Generates an integer which will be used as ID for each buffer
    invalidateOpenGLBuffers();
  }
Scene_plane_item::~Scene_plane_item() {
  delete frame;
}

void Scene_plane_item::initializeBuffers(Viewer_interface *viewer) const
{
  getTriangleContainer(0)->initializeBuffers(viewer);
  getTriangleContainer(0)->setFlatDataSize(nb_quads);
  positions_quad.resize(0);
  positions_quad.shrink_to_fit();
  getEdgeContainer(0)->initializeBuffers(viewer);
  getEdgeContainer(0)->setFlatDataSize(nb_lines);
  positions_lines.resize(0);
  positions_lines.shrink_to_fit();
}

void Scene_plane_item::computeElements() const
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    positions_quad.resize(0);
    positions_lines.resize(0);

    const double diag = scene_diag();
    //The quad
    {

    positions_quad.push_back(-diag);
    positions_quad.push_back(-diag);
    positions_quad.push_back(0.0  );
    positions_quad.push_back(-diag);
    positions_quad.push_back(diag );
    positions_quad.push_back(0.0  );
    positions_quad.push_back(diag );
    positions_quad.push_back(-diag);
    positions_quad.push_back(0.0  );

    positions_quad.push_back(diag );
    positions_quad.push_back(-diag);
    positions_quad.push_back(0.0  );
    positions_quad.push_back(-diag);
    positions_quad.push_back(diag );
    positions_quad.push_back(0.0  );
    positions_quad.push_back(diag );
    positions_quad.push_back(diag );
    positions_quad.push_back(0.0  );

    getTriangleContainer(0)->allocate(Tc::Flat_vertices, positions_quad.data(),
                                      static_cast<int>(positions_quad.size() 
                                                       * sizeof(float)));
    }
    //The grid
    float x = (2*diag)/10.0;
    float y = (2*diag)/10.0;
    {
        for(float u = 0; u < 11; u += 1.f)
        {

            positions_lines.push_back(-diag + x* u);
            positions_lines.push_back(-diag       );
            positions_lines.push_back(0.0         );

            positions_lines.push_back(-diag + x* u);
            positions_lines.push_back(diag        );
            positions_lines.push_back(0.0         );
        }
        for(float v=0; v<11; v += 1.f)
        {

            positions_lines.push_back(-diag        );
            positions_lines.push_back(-diag + v * y);
            positions_lines.push_back(0.0          );

            positions_lines.push_back(diag         );
            positions_lines.push_back(-diag + v * y);
            positions_lines.push_back(0.0          );
        }
        getEdgeContainer(0)->allocate(Ec::Vertices, positions_lines.data(),
                                          static_cast<int>(positions_lines.size() 
                                                           * sizeof(float)));
    }
    nb_quads = positions_quad.size();
    nb_lines = positions_lines.size();
    setBuffersFilled(true);
    QApplication::restoreOverrideCursor();
}

void Scene_plane_item::draw(Viewer_interface* viewer)const
{
  if(!isInit(viewer))
    initGL(viewer);
  if ( getBuffersFilled() &&
       ! getBuffersInit(viewer))
  {
    initializeBuffers(viewer);
    setBuffersInit(viewer, true);
  }
  if(!getBuffersFilled())
  {
    computeElements();
    initializeBuffers(viewer);
  }
  
  QMatrix4x4 f_matrix;
  for(int i=0; i<16; i++)
    f_matrix.data()[i] = (float)frame->matrix()[i];
  
  getTriangleContainer(0)->setFrameMatrix(f_matrix);
  getTriangleContainer(0)->setColor(this->color());
  getTriangleContainer(0)->draw(viewer, true);
}

void Scene_plane_item::drawEdges(CGAL::Three::Viewer_interface* viewer)const
{
  if(!isInit(viewer))
    initGL(viewer);
  if ( getBuffersFilled() &&
       ! getBuffersInit(viewer))
  {
    initializeBuffers(viewer);
    setBuffersInit(viewer, true);
  }
  if(!getBuffersFilled())
  {
    computeElements();
    initializeBuffers(viewer);
  }
    QMatrix4x4 f_matrix;
    for(int i=0; i<16; i++)
        f_matrix.data()[i] = (float)frame->matrix()[i];
    getEdgeContainer(0)->setFrameMatrix(f_matrix);
    getEdgeContainer(0)->setColor(QColor(0,0,0));
    getEdgeContainer(0)->draw(viewer, true);
}

void Scene_plane_item::flipPlane()
{
  CGAL::qglviewer::Quaternion q;
  CGAL::qglviewer::Vec axis(0,1,0);
  if(frame->orientation().axis() == axis)
    q.setAxisAngle(CGAL::qglviewer::Vec(1,0,0), CGAL_PI);
  else
    q.setAxisAngle(axis, CGAL_PI);
  frame->rotate(q.normalized());
  invalidateOpenGLBuffers();
  Q_EMIT itemChanged();

}

bool Scene_plane_item::manipulatable() const {
  return manipulable;
}
Scene_item::ManipulatedFrame* Scene_plane_item::manipulatedFrame() {
  return frame;
}

Scene_plane_item* Scene_plane_item::clone() const {
  if(can_clone)
  {
    Scene_plane_item* item = new Scene_plane_item(scene);
    item->manipulable = manipulable;
    item->can_clone = true;
    item->frame = new ManipulatedFrame;
    item->frame->setPosition(frame->position());
    item->frame->setOrientation(frame->orientation());
    return item;
  }
  else
    return 0;
}

QString Scene_plane_item::toolTip() const {
  const CGAL::qglviewer::Vec& pos = frame->position();
  const CGAL::qglviewer::Vec& n = frame->inverseTransformOf(CGAL::qglviewer::Vec(0.f, 0.f, 1.f));
  return
    tr("<p><b>%1</b> (mode: %2, color: %3)<br />")
    .arg(this->name())
    .arg(this->renderingModeName())
    .arg(this->color().name())

    +
    tr("<i>Plane</i></p>"
       "<p>Equation: %1*x + %2*y + %3*z + %4 = 0<br />"
       "Normal vector: (%1, %2, %3)<br />"
       "Point: (%5, %6, %7)</p>")
    .arg(n[0]).arg(n[1]).arg(n[2])
    .arg( - pos * n)
    .arg(pos[0]).arg(pos[1]).arg(pos[2])
    +
    tr("<p>Can clone: %1<br />"
       "Manipulatable: %2</p>")
    .arg(can_clone?tr("true"):tr("false"))
    .arg(manipulable?tr("true"):tr("false"));
}

Plane_3 Scene_plane_item::plane(CGAL::qglviewer::Vec offset) const {
  const CGAL::qglviewer::Vec& pos = frame->position() - offset;
  const CGAL::qglviewer::Vec& n =
    frame->inverseTransformOf(CGAL::qglviewer::Vec(0.f, 0.f, 1.f));
  return Plane_3(n[0], n[1],  n[2], - n * pos);
}

void Scene_plane_item::invalidateOpenGLBuffers()
{
    setBuffersFilled(false);
    getTriangleContainer(0)->reset_vbos(ALL);
    getEdgeContainer(0)->reset_vbos(ALL);
}

void Scene_plane_item::setPosition(float x, float y, float z) {
  frame->setPosition(x, y, z);
}

void Scene_plane_item::setPosition(double x, double y, double z) {
  const CGAL::qglviewer::Vec offset = static_cast<CGAL::Three::Viewer_interface*>(CGAL::QGLViewer::QGLViewerPool().first())->offset();
  frame->setPosition((float)x+offset.x, (float)y+offset.y, (float)z+offset.z);
}

void Scene_plane_item::setNormal(float x, float y, float z) {
  QVector3D normal(x,y,z);
  if(normal == QVector3D(0,0,0))
    return;
  QVector3D origin(0,0,1);
  CGAL::qglviewer::Quaternion q;
  if(origin == normal)
  {
    return;
  }
   if(origin == -normal)
  {
    q.setAxisAngle(CGAL::qglviewer::Vec(0,1,0),CGAL_PI);
    frame->setOrientation(q);
    return;
  }

  QVector3D cp = QVector3D::crossProduct(origin, normal);
  cp.normalize();
  q.setAxisAngle(CGAL::qglviewer::Vec(cp.x(),cp.y(), cp.z()),acos(QVector3D::dotProduct(origin, normal)/(normal.length()*origin.length())));

  frame->setOrientation(q.normalized());
}

void Scene_plane_item::setNormal(double x, double y, double z) {
  setNormal((float)x, (float)y, (float)z);
}

void Scene_plane_item::setClonable(bool b) {
  can_clone = b;
}

void Scene_plane_item::setManipulatable(bool b) {
  manipulable = b;
}
