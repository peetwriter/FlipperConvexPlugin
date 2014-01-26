#include "convexHullPlugin.hh"


#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

#include "OpenFlipper/BasePlugin/PluginFunctions.hh"

ConvexHullPlugin::ConvexHullPlugin() :
        iterationsSpinbox_(0)
{

}

ConvexHullPlugin::~ConvexHullPlugin()
{

}

void ConvexHullPlugin::initializePlugin()
{
   // Create the Toolbox Widget
   QWidget* toolBox = new QWidget();
   QGridLayout* layout = new QGridLayout(toolBox);

   QPushButton* smoothButton = new QPushButton("&Convex",toolBox);
   smoothButton->setToolTip(tr("Smooths an Object using Laplacian Smoothing."));
   smoothButton->setWhatsThis(tr("Smooths an Object using Laplacian Smoothing. Use the Smooth Plugin for more options."));



   iterationsSpinbox_ =  new QSpinBox(toolBox) ;
   iterationsSpinbox_->setMinimum(1);
   iterationsSpinbox_->setMaximum(1000);
   iterationsSpinbox_->setSingleStep(1);
   iterationsSpinbox_->setToolTip(tr("The number of Convex operations."));
   iterationsSpinbox_->setWhatsThis(tr("Give the number, how often the Laplacian Smoothing should modify the object."));

   QLabel* label = new QLabel("Iterations:");

   layout->addWidget( label             , 0, 0);
   layout->addWidget( smoothButton      , 1, 1);
   layout->addWidget( iterationsSpinbox_, 0, 1);

   layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Expanding),2,0,1,2);

   connect( smoothButton, SIGNAL(clicked()), this, SLOT(simpleLaplace()) );

   QIcon* toolIcon = new QIcon(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"smoother1.png");
   emit addToolbox( tr("Simple Smoother") , toolBox, toolIcon );
}

void ConvexHullPlugin::pluginsInitialized() {

    // Emit slot description
    emit setSlotDescription(tr("simpleLaplace(int)"),   tr("Smooth mesh using the Laplace operator with uniform weights."),
                            QStringList(tr("iterations")), QStringList(tr("Number of iterations")));
}

/** \brief simpleLaplace
 *
 *  Smooth mesh using the Laplace operator
 *  with uniform weights.
 */
void ConvexHullPlugin::simpleLaplace() {

    int iterations = 1;

    if(!OpenFlipper::Options::nogui()) {
        iterations = iterationsSpinbox_->value();
    }

    simpleLaplace(iterations);
}

/** \brief simpleLaplace
 *
 * Smooth mesh using the Laplace operator
 * with uniform weights.
 *
 * @param _iterations Number of iterations
 */
void ConvexHullPlugin::simpleLaplace(int _iterations) {

    for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS) ; o_it != PluginFunctions::objectsEnd(); ++o_it) {

    bool selectionExists = false;

    if ( o_it->dataType( DATA_TRIANGLE_MESH ) ) {

        // Get the mesh to work on
      TriMesh* mesh = PluginFunctions::triMesh(*o_it);

      // Property for the active mesh to store original point positions
      OpenMesh::VPropHandleT< TriMesh::Point > origPositions;

      // Add a property to the mesh to store original vertex positions
      mesh->add_property( origPositions, "SmootherPlugin_Original_Positions" );

      for ( int i = 0 ; i < _iterations ; ++i ) {

          // Copy original positions to backup ( in Vertex property )
          TriMesh::VertexIter v_it, v_end=mesh->vertices_end();
          for (v_it=mesh->vertices_begin(); v_it!=v_end; ++v_it) {
            mesh->property( origPositions, v_it ) = mesh->point(v_it);
            // See if at least one vertex has been selected
            selectionExists |= mesh->status(v_it).selected();
          }

          // Do one smoothing step (For each point of the mesh ... )
          for (v_it=mesh->vertices_begin(); v_it!=v_end; ++v_it) {

            if(selectionExists && mesh->status(v_it).selected() == false) {
              continue;
            }

            TriMesh::Point point = TriMesh::Point(0.0,0.0,0.0);

            // Flag, to skip boundary vertices
            bool skip = false;

            // ( .. for each Outoing halfedge .. )
            TriMesh::VertexOHalfedgeIter voh_it(*mesh,v_it);
            for ( ; voh_it; ++voh_it ) {
                // .. add the (original) position of the Neighbour ( end of the outgoing halfedge )
                point += mesh->property( origPositions, mesh->to_vertex_handle(voh_it) );

                // Check if the current Halfedge is a boundary halfedge
                // If it is, abort and keep the current vertex position
                if ( mesh->is_boundary( voh_it.handle() ) ) {
                  skip = true;
                  break;
                }

            }

            // Devide by the valence of the current vertex
            point /= mesh->valence( v_it );

            if ( ! skip ) {
                // Set new position for the mesh if its not on the boundary
                mesh->point(v_it) = point;
            }
          }

      }// Iterations end

      // Remove the property
      mesh->remove_property( origPositions );

      mesh->update_normals();

      emit updatedObject( o_it->id(), UPDATE_GEOMETRY );

      // Create backup
      emit createBackup(o_it->id(), "Simple Smoothing", UPDATE_GEOMETRY );

   } else if ( o_it->dataType( DATA_POLY_MESH ) ) {

       // Get the mesh to work on
      PolyMesh* mesh = PluginFunctions::polyMesh(*o_it);

      // Property for the active mesh to store original point positions
      OpenMesh::VPropHandleT< TriMesh::Point > origPositions;

      // Add a property to the mesh to store original vertex positions
      mesh->add_property( origPositions, "SmootherPlugin_Original_Positions" );

      for ( int i = 0 ; i < _iterations ; ++i ) {

         // Copy original positions to backup ( in Vertex property )
         PolyMesh::VertexIter v_it, v_end=mesh->vertices_end();
         for (v_it=mesh->vertices_begin(); v_it!=v_end; ++v_it) {
            mesh->property( origPositions, v_it ) = mesh->point(v_it);
            // See if at least one vertex has been selected
            selectionExists |= mesh->status(v_it).selected();
         }

         // Do one smoothing step (For each point of the mesh ... )
         for (v_it=mesh->vertices_begin(); v_it!=v_end; ++v_it) {

            if(selectionExists && mesh->status(v_it).selected() == false) {
              continue;
            }

            PolyMesh::Point point = PolyMesh::Point(0.0,0.0,0.0);

            // Flag, to skip boundary vertices
            bool skip = false;

            // ( .. for each Outoing halfedge .. )
            PolyMesh::VertexOHalfedgeIter voh_it(*mesh,v_it);
            for ( ; voh_it; ++voh_it ) {
               // .. add the (original) position of the Neighbour ( end of the outgoing halfedge )
               point += mesh->property( origPositions, mesh->to_vertex_handle(voh_it) );

               // Check if the current Halfedge is a boundary halfedge
               // If it is, abort and keep the current vertex position
               if ( mesh->is_boundary( voh_it.handle() ) ) {
                  skip = true;
                  break;
               }

            }

            // Devide by the valence of the current vertex
            point /= mesh->valence( v_it );

            if ( ! skip ) {
               // Set new position for the mesh if its not on the boundary
               mesh->point(v_it) = point;
            }
         }

      }// Iterations end

      // Remove the property
      mesh->remove_property( origPositions );

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


Q_EXPORT_PLUGIN2( convexhullplugin , ConvexHullPlugin );

