

#ifndef CONVEXPLUGIN_HH
#define CONVEXPLUGIN_HH

//== INCLUDES =================================================================

#include <QObject>
#include <QMenuBar>
#include <QTimer>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/BackupInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>

#include <OpenFlipper/common/Types.hh>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

#include <OpenMesh/Tools/Smoother/JacobiLaplaceSmootherT.hh>

#include "ConvexToolbarWidget.hh"


//== CLASS DEFINITION =========================================================


/** Plugin for Smoother Support
 */
class ConvexPlugin : public QObject, BaseInterface, ToolboxInterface, LoggingInterface, BackupInterface, ScriptInterface
{
  Q_OBJECT
  Q_INTERFACES(BaseInterface)
  Q_INTERFACES(ToolboxInterface)
  Q_INTERFACES(BackupInterface)
  Q_INTERFACES(LoggingInterface)
  Q_INTERFACES(ScriptInterface)

signals:

  // BaseInterface
  void updateView();

  void updatedObject(int, const UpdateType&);

  void setSlotDescription(QString     _slotName,   QString     _slotDescription,
                          QStringList _parameters, QStringList _descriptions);

  // LoggingInterface
  void log(Logtype _type, QString _message);
  void log(QString _message);
  
  // ToolboxInterface
  void addToolbox( QString _name  , QWidget* _widget, QIcon* _icon );
  
  // BackupInterface
  void createBackup( int _id , QString _name, UpdateType _type = UPDATE_ALL );

  // ScriptInterface
  void scriptInfo(QString _functionName);
  
public :

  /// default constructor
  ConvexPlugin();

  /// default destructor
  ~ConvexPlugin() {};

  /// Name of the Plugin
  QString name(){ return (QString("Convex Hull")); };

  /// Description of the Plugin
  QString description() { return (QString("Building Convex Hull")); };

private slots:

  void initializePlugin(); // BaseInterface
  
  /// Second initialization stage
  void pluginsInitialized();

  /// Tell system that this plugin runs without ui
  void noguiSupported( ) {} ;

private :

  /// Widget for Toolbox
  ConvexToolbarWidget* tool_;
  QIcon* toolIcon_;

private slots:

  /// Slot connected to the smooth button in the toolbox
  void slot_smooth();

//===========================================================================
/** @name Scripting Functions
  * @{ */
//===========================================================================

public slots:
   QString version() { return QString("1.0"); };



private:
   typedef OpenMesh::Smoother::JacobiLaplaceSmootherT< TriMesh > SmootherType;
};

#endif
