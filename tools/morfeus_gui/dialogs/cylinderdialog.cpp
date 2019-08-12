#include "cylinderdialog.h"
#include "ui_cylinderdialog.h"

CylinderDialog::CylinderDialog(QWidget *parent)
  : QDialog(parent)
  , mUi(new Ui::CylinderDialog)
{
  mUi->setupUi(this);
  connect(mUi->cX, SIGNAL(textChanged(QString)), SLOT(updateData()));
  connect(mUi->cY, SIGNAL(textChanged(QString)), SLOT(updateData()));
  connect(mUi->cZ, SIGNAL(textChanged(QString)), SLOT(updateData()));
  connect(mUi->cRadius, SIGNAL(textChanged(QString)), SLOT(updateData()));
  connect(mUi->cHeight, SIGNAL(textChanged(QString)), SLOT(updateData()));
  connect(mUi->cName, SIGNAL(textChanged(QString)), SLOT(updateData()));
}

CylinderDialog::~CylinderDialog()
{
  delete mUi;
}

double CylinderDialog::height() const
{
  return mUi->cHeight->text().trimmed().toDouble();
}

QString CylinderDialog::name() const
{
  return mUi->cName->text().trimmed();
}

double CylinderDialog::radius() const
{
  return mUi->cRadius->text().trimmed().toDouble();
}

double CylinderDialog::x() const
{
  return mUi->cX->text().trimmed().toDouble();
}

double CylinderDialog::y() const
{
  return mUi->cY->text().trimmed().toDouble();
}

double CylinderDialog::z() const
{
  return mUi->cZ->text().trimmed().toDouble();
}

void CylinderDialog::setHeight(double value)
{
  mUi->cHeight->setText(QString::number(value));
}

void CylinderDialog::setName(const QString & name)
{
  mUi->cName->setText(name);
}

void CylinderDialog::setRadius(double value)
{
  mUi->cRadius->setText(QString::number(value));
}

void CylinderDialog::setX(double value)
{
  mUi->cX->setText(QString::number(value));
}

void CylinderDialog::setY(double value)
{
  mUi->cY->setText(QString::number(value));
}

void CylinderDialog::setZ(double value)
{
  mUi->cZ->setText(QString::number(value));
}

void CylinderDialog::updateData()
{

}
