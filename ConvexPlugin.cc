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

        if ( o_it->dataType( DATA_TRIANGLE_MESH ) ) {

          emit log(LOGERR, "DataType is DATA_POLY_MESH");

        } else if ( o_it->dataType( DATA_POLY_MESH ) ) {

          emit log(LOGERR, "DataType is DATA_POLY_MESH");

        } else {

          emit log(LOGERR, "DataType not supported.");
        }

    }
  // Show script logging
  emit scriptInfo("simpleLaplace(" + QString::number(_iterations) + ")");

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

