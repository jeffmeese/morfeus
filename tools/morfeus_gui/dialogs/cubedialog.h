#ifndef CUBEDIALOG_H
#define CUBEDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
  class CubeDialog;
}

class CubeDialog
    : public QDialog
{
  Q_OBJECT

public:
  explicit CubeDialog(QWidget *parent = nullptr);
  ~CubeDialog();

public:
  double height() const;
  double length() const;
  QString name() const;
  double width() const;
  double x() const;
  double y() const;
  double z() const;
  void setHeight(double value);
  void setLength(double value);
  void setWidth(double value);
  void setName(const QString & name);
  void setX(double value);
  void setY(double value);
  void setZ(double value);

private slots:
  void updateData();

private:
  Ui::CubeDialog * mUi;
};

#endif // CUBEDIALOG_H
