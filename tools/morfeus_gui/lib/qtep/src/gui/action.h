#ifndef ACTION_H
#define ACTION_H

#include "qtep.h"

#include <QAction>
#include <QList>
#include <QString>

class QTEP_LIB_DECL Action
    : public QAction
{
  Q_OBJECT

public:
  Action(const QString & id, QAction * qtAction = nullptr);
  Action(const QString & id, const QString & text, QAction * qtAction = nullptr);

public:
  QList<int> currentContext() const;
  QString id() const;
  QAction * qtAction() const;
  bool supportsMultipleSelection() const;
  void setCurrentContext(const QList<int> & contexts);
  void setQtAction(QAction * qtAction);
  void setSupportsMultipleSelection(bool value);

protected slots:
  void handleChanged();

private:
  virtual void doSetContext(const QList<int> & contexts);

private:
  QString mId;
  QString mText;
  QAction * mQtAction;
  bool mSupportsMultipleSelection;
};

class QTEP_LIB_DECL Separator
    : public Action
{
public:
  Separator();
};

#endif // ACTION_H
