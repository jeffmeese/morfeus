#ifndef CYLINDERDIALOG_H
#define CYLINDERDIALOG_H

#include <QDialog>

namespace Ui {
  class CylinderDialog;
}

class CylinderDialog
    : public QDialog
{
  Q_OBJECT

public:
  explicit CylinderDialog(QWidget *parent = nullptr);
  ~CylinderDialog();

public:
  double height() const;
  QString name() const;
  double radius() const;
  double x() const;
  double y() const;
  double z() const;
  void setHeight(double value);
  void setName(const QString & name);
  void setRadius(double value);
  void setX(double value);
  void setY(double value);
  void setZ(double value);

private slots:
  void updateData();

private:
  Ui::CylinderDialog *mUi;
};

#endif // CYLINDERDIALOG_H
