#include "cubedialog.h"
#include "ui_cubedialog.h"

#include <QDialogButtonBox>
#include <QPushButton>

CubeDialog::CubeDialog(QWidget *parent)
  : QDialog(parent)
  , mUi(new Ui::CubeDialog)
{
  mUi->setupUi(this);
  connect(mUi->cX, SIGNAL(textChanged(QString)), SLOT(updateData()));
  connect(mUi->cY, SIGNAL(textChanged(QString)), SLOT(updateData()));
  connect(mUi->cZ, SIGNAL(textChanged(QString)), SLOT(updateData()));
  connect(mUi->cLength, SIGNAL(textChanged(QString)), SLOT(updateData()));
  connect(mUi->cWidth, SIGNAL(textChanged(QString)), SLOT(updateData()));
  connect(mUi->cHeight, SIGNAL(textChanged(QString)), SLOT(updateData()));
  connect(mUi->cName, SIGNAL(textChanged(QString)), SLOT(updateData()));
  updateData();
}

CubeDialog::~CubeDialog()
{
  delete mUi;
}

double CubeDialog::height() const
{
  return mUi->cHeight->text().trimmed().toDouble();
}

double CubeDialog::length() const
{
  return mUi->cLength->text().trimmed().toDouble();
}

QString CubeDialog::name() const
{
  return mUi->cName->text().trimmed();
}

double CubeDialog::width() const
{
  return mUi->cWidth->text().trimmed().toDouble();
}

double CubeDialog::x() const
{
  return mUi->cX->text().trimmed().toDouble();
}

double CubeDialog::y() const
{
  return mUi->cY->text().trimmed().toDouble();
}

double CubeDialog::z() const
{
  return mUi->cZ->text().trimmed().toDouble();
}

void CubeDialog::setHeight(double value)
{
  mUi->cHeight->setText(QString::number(value));
}

void CubeDialog::setLength(double value)
{
  mUi->cLength->setText(QString::number(value));
}

void CubeDialog::setWidth(double value)
{
  mUi->cWidth->setText(QString::number(value));
}

void CubeDialog::setName(const QString & name)
{
  mUi->cName->setText(name);
}

void CubeDialog::setX(double value)
{
  mUi->cX->setText(QString::number(value));
}

void CubeDialog::setY(double value)
{
  mUi->cY->setText(QString::number(value));
}

void CubeDialog::setZ(double value)
{
  mUi->cZ->setText(QString::number(value));
}

void CubeDialog::updateData()
{
  bool enableOk = true;
  QString name = mUi->cName->text().trimmed();
  if (name.isEmpty()) {
    enableOk = false;
  }

  mUi->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(enableOk);
}
