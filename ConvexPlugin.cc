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
    connect(tool_->pB_smooth,SIGNAL(clicked() ),this,SLOT(slot_smooth()));

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


void ConvexPlugin::slot_smooth() {

    int iterations = 1;

    if(!OpenFlipper::Options::nogui()) {
        iterations = tool_->iterationSpinBox->value();
    }

    slot_smooth(iterations);
}


void
ConvexPlugin::
slot_smooth(int _iterations)
{
    for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS) ; o_it != PluginFunctions::objectsEnd(); ++o_it) {

    bool selectionExists = false;

    if ( o_it->dataType( DATA_TRIANGLE_MESH ) ) {

        // Get the mesh to work on
      TriMesh* mesh = PluginFunctions::triMesh(*o_it);

      for ( int i = 0 ; i < _iterations ; ++i ) {
          PolyMesh::VertexIter v_it, v_end=mesh->vertices_end();
          // Do one smoothing step (For each point of the mesh ... )
          for (v_it=mesh->vertices_begin(); v_it!=v_end; ++v_it) {

            TriMesh::Point point = TriMesh::Point(0.0 + i,0.0 + i,0.0 + i);
            mesh->point(v_it) = point;

          }

      }// Iterations end

      // Remove the property

      mesh->update_normals();

      emit updatedObject( o_it->id(), UPDATE_GEOMETRY );

      // Create backup
      emit createBackup(o_it->id(), "Simple Smoothing", UPDATE_GEOMETRY );

   } else if ( o_it->dataType( DATA_POLY_MESH ) ) {

       // Get the mesh to work on
      PolyMesh* mesh = PluginFunctions::polyMesh(*o_it);


      for ( int i = 0 ; i < _iterations ; ++i ) {


         PolyMesh::VertexIter v_it, v_end=mesh->vertices_end();

         PolyMesh::Point point = PolyMesh::Point(0.0+ i,0.0 + i,0.0 + i);
          mesh->point(v_it) = point;


      }// Iterations end

      mesh->update_normals();

      emit updatedObject( o_it->id() , UPDATE_GEOMETRY);

      // Create backup
      emit createBackup(o_it->id(), "Simple Smoothing", UPDATE_GEOMETRY);

    } else {

      emit log(LOGERR, "DataType not supported.");
    }
  }

  // Show script logging
  emit scriptInfo("simpleLaplace(" + QString::number(_iterations) + ")");

  emit updateView();
}
//-----------------------------------------------------------------------------

Q_EXPORT_PLUGIN2( ConvexPlugin , ConvexPlugin );

