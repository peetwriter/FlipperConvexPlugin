#ifndef CONVEXHULLPLUGIN_HH
#define CONVEXHULLPLUGIN_HH

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/BasePlugin/BackupInterface.hh>
#include <OpenFlipper/common/Types.hh>

class ConvexHullPlugin : public QObject, BaseInterface, ToolboxInterface, LoggingInterface, ScriptInterface, BackupInterface
{
  Q_OBJECT
  Q_INTERFACES(BaseInterface)
  Q_INTERFACES(ToolboxInterface)
  Q_INTERFACES(LoggingInterface)
  Q_INTERFACES(ScriptInterface)
  Q_INTERFACES(BackupInterface)

  signals:
    //BaseInterface
    void updateView();
    void updatedObject(int _id, const UpdateType& _type);
    void setSlotDescription(QString     _slotName,   QString     _slotDescription,
                          QStringList _parameters, QStringList _descriptions);

    //LoggingInterface
    void log(Logtype _type, QString _message);
    void log(QString _message);

    // ToolboxInterface
    void addToolbox( QString _name  , QWidget* _widget, QIcon* _icon);

    // ScriptInterface
    void scriptInfo(QString _functionName);

    // BackupInterface
    void createBackup( int _id , QString _name, UpdateType _type = UPDATE_ALL );

  public:

    ConvexHullPlugin();
    ~ConvexHullPlugin();

    // BaseInterface
    QString name() { return (QString("Convex Hull")); };
    QString description( ) { return (QString("Building Convex Hull")); };

   private:

    /// SpinBox for Number of iterations
    QSpinBox* iterationsSpinbox_;

   private slots:
    void simpleLaplace();

    void initializePlugin(); // BaseInterface

    void pluginsInitialized(); // BaseInterface

    // Scriptable functions
   public slots:

    void simpleLaplace(int _iterations);

    QString version() { return QString("1.0"); };
};

#endif //CONVEXHULLPLUGIN_HH
