#include "ui_convexToolbar.hh"
#include <QtGui>

class ConvexToolbarWidget : public QWidget, public Ui::SmootherToolbar
{
  Q_OBJECT

  public:
    ConvexToolbarWidget(QWidget *parent = 0);
};

