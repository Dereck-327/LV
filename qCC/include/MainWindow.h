#pragma once

#include "ccPluginManager.h"
#include "ccPointCloud.h"

#include <QMainWindow>

#include <memory>

class QStandardItemModel;
class ccGLWindow;
class ccConsole;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;

 private:
  void setupCentralView();
  void setupTreeModels();
  void setupActions();
  void setupLogging();
  void refreshProperties();
  void populateDemoCloud();
  void reloadPlugins();
  void openPointCloud();
  void savePointCloud();

  std::unique_ptr<Ui::MainWindow> ui_;
  ccGLWindow* gl_window_ = nullptr;
  QStandardItemModel* db_model_ = nullptr;
  QStandardItemModel* properties_model_ = nullptr;
  lv::db::PointCloud current_cloud_;
  lv::plugin::ccPluginManager plugin_manager_;
};