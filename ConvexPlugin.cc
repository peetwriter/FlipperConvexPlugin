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


//-----------------------------------------------------------------------------

void
ConvexPlugin::
slot_smooth()
{
    emit log(LOGWARN , "Unable to get object ( Only Triangle Meshes supported)");
}
//-----------------------------------------------------------------------------

Q_EXPORT_PLUGIN2( ConvexPlugin , ConvexPlugin );

