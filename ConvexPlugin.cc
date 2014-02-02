//== INCLUDES =================================================================

#include <QtGui>

#include "ConvexPlugin.hh"

#include <iostream>
#include <ACG/GL/GLState.hh>
#include <QStringList>
#include <ACG/Scenegraph/ManipulatorNode.hh>


#include <OpenFlipper/BasePlugin/PluginFunctions.hh>


#define SMOOTHER "SmootherData"

//== IMPLEMENTATION ==========================================================

ConvexPlugin::ConvexPlugin() :
        tool_(0),
        toolIcon_(0)
{
}

//-----------------------------------------------------------------------------

void
ConvexPlugin::
initializePlugin()
{
  if ( OpenFlipper::Options::gui() ) {
    tool_ = new ConvexToolbarWidget();
    QSize size(100, 100);
    tool_->resize(size);

    // connect signals->slots
    connect(tool_->pB_smooth,SIGNAL(clicked() ),this,SLOT(logger_func()));
    connect(tool_->pb_Create,SIGNAL(clicked() ),this,SLOT(create_object()));

    toolIcon_ = new QIcon(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"smoother2.png");
    emit addToolbox( tr("Convex") , tool_, toolIcon_ );
  }
}

//-----------------------------------------------------------------------------

/** \brief Set the scripting slot descriptions
 *
 */
void
ConvexPlugin::pluginsInitialized(){}


void ConvexPlugin::logger_func() {

    int n = 0;
    std::stringstream sstm;

    for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS) ; o_it != PluginFunctions::objectsEnd(); ++o_it) {

        if ( o_it->dataType( DATA_TRIANGLE_MESH ) ) {

         TriMesh* mesh = PluginFunctions::triMesh(*o_it);
         TriMesh::VertexIter v_it, v_end = mesh->vertices_end();
         for (v_it = mesh->vertices_begin(); v_it != v_end; ++v_it) {
             n++;
         }

          emit log(LOGERR, "DataType is Triangle mesh, number of vertices:" + n);

        } else if ( o_it->dataType( DATA_POLY_MESH ) ) {

          PolyMesh* mesh = PluginFunctions::polyMesh(*o_it);
          PolyMesh::VertexIter v_it, v_end = mesh->vertices_end();
          PolyMesh::Point point;


          for (v_it = mesh->vertices_begin(); v_it != v_end; ++v_it) {
              point = mesh->point(v_it);
              n = point[1];

          }

          sstm << "DataType is PolyMesh, number of vertices:" << n;
          QString result = QString::fromStdString(sstm.str());

          emit log(LOGERR, result);

        } else {

          emit log(LOGERR, "DataType not supported.");
        }

    }

  emit updateView();
}


void ConvexPlugin::create_object()
{
    int newObjectId = -1;
    emit addEmptyObject(DATA_PLANE, newObjectId);

    PlaneObject* object = 0;
    PluginFunctions::getObject(newObjectId, object);

    if (object){

        // Now you can use the object as usual, e.g. Get the node
        PlaneNode* planeNode = object->planeNode();
        // change it
        //planeNode->setPosition(origin,normal);
        //planeNode->setSize(kinectinfo_->getMaxDepth() / 2, kinectinfo_->getMaxDepth() / 2);
    } else {
        emit log(LOGERR, "something went wrong when creating an object  ");
    }
}

//-----------------------------------------------------------------------------

Q_EXPORT_PLUGIN2( ConvexPlugin , ConvexPlugin );

