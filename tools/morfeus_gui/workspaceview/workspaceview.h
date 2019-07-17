#ifndef WORKSPACEVIEW_H
#define WORKSPACEVIEW_H

#include <QTreeView>

class ActionManager;
class GuiProject;

class WorkspaceView
    : public QTreeView
{
  Q_OBJECT

public:
  WorkspaceView(QWidget * parentWidget = nullptr);

public:
  void setProject(GuiProject * project);

public:
  static ActionManager & actionManager();

protected slots:
  void handleAddRectangle();
  void handleContextMenu(const QPoint & menuPos);
  void handleRename();

private:
  void initActions();

private:
  GuiProject * mProject;
};

#endif // WORKSPACEVIEW_H
