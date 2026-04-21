#pragma once

#include "ccPluginManager.h"

#include <QMainWindow>

class QTextEdit;
class ccGLWindow;

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr);

 private:
  void populateDemoCloud();
  void reloadPlugins();
  void appendLog(const QString& line);

  ccGLWindow* gl_window_ = nullptr;
  QTextEdit* log_view_ = nullptr;
  lv::plugin::ccPluginManager plugin_manager_;
};