#ifndef SIMPLEPLUGIN_HH_INCLUDED
#define SIMPLEPLUGIN_HH_INCLUDED
#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/common/Types.hh>
class ConvexHullPlugin : public QObject, BaseInterface
{
Q_OBJECT
Q_INTERFACES(BaseInterface)
public :
  
  ~ConvexHullPlugin() {};
  
  QString name() { return QString("ConvexHullPlugin"); };
  
  QString description() { return QString("Does actually nothing but works!"); };
  
};
#endif