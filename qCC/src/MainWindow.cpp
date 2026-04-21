#include "MainWindow.h"

#include "ccAppCommon.h"
#include "ccFileIO.h"
#include "ccGLWindow.h"
#include "ccPointCloud.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), plugin_manager_(this) {
  setWindowTitle(QStringLiteral("LV CloudCompare-style Demo"));

  auto* central = new QWidget(this);
  auto* root_layout = new QHBoxLayout(central);
  auto* controls_layout = new QVBoxLayout();

  auto* summary_label = new QLabel(lv::app::BuildSummary(), this);
  summary_label->setWordWrap(true);

  auto* generate_button = new QPushButton(QStringLiteral("Generate qCC_db demo"), this);
  auto* save_button = new QPushButton(QStringLiteral("Save qCC_io demo"), this);
  auto* load_button = new QPushButton(QStringLiteral("Load qCC_io demo"), this);
  auto* plugins_button = new QPushButton(QStringLiteral("Reload plugins"), this);
  auto* run_demos_button = new QPushButton(QStringLiteral("Run plugin demos"), this);

  gl_window_ = new ccGLWindow(this);
  log_view_ = new QTextEdit(this);
  log_view_->setReadOnly(true);

  controls_layout->addWidget(summary_label);
  controls_layout->addWidget(generate_button);
  controls_layout->addWidget(save_button);
  controls_layout->addWidget(load_button);
  controls_layout->addWidget(plugins_button);
  controls_layout->addWidget(run_demos_button);
  controls_layout->addStretch();
  controls_layout->addWidget(log_view_, 1);

  root_layout->addLayout(controls_layout, 1);
  root_layout->addWidget(gl_window_, 2);
  setCentralWidget(central);
  resize(1080, 640);

  connect(generate_button, &QPushButton::clicked, this, &MainWindow::populateDemoCloud);
  connect(plugins_button, &QPushButton::clicked, this, &MainWindow::reloadPlugins);

  connect(save_button, &QPushButton::clicked, this, [this]() {
    const QString path = QFileDialog::getSaveFileName(this, QStringLiteral("Save point cloud"),
                                                      QDir::homePath(), QStringLiteral("Point Cloud (*.txt)"));
    if (path.isEmpty()) {
      return;
    }

    QString error_message;
    const auto cloud = lv::db::MakeDemoPointCloud();
    if (lv::io::SavePointCloudAsText(path, cloud, &error_message)) {
      appendLog(QStringLiteral("Saved IO demo to %1").arg(path));
    } else {
      appendLog(QStringLiteral("Save failed: %1").arg(error_message));
    }
  });

  connect(load_button, &QPushButton::clicked, this, [this]() {
    const QString path = QFileDialog::getOpenFileName(this, QStringLiteral("Load point cloud"),
                                                      QDir::homePath(), QStringLiteral("Point Cloud (*.txt)"));
    if (path.isEmpty()) {
      return;
    }

    QString error_message;
    const auto cloud = lv::io::LoadPointCloudFromText(path, &error_message);
    if (cloud.empty() && !error_message.isEmpty()) {
      appendLog(QStringLiteral("Load failed: %1").arg(error_message));
      return;
    }

    gl_window_->setPointCloud(cloud);
    appendLog(QStringLiteral("Loaded %1").arg(lv::db::DescribePointCloud(cloud)));
  });

  connect(run_demos_button, &QPushButton::clicked, this, [this]() {
    for (const QString& line : plugin_manager_.runAllDemos()) {
      appendLog(line);
    }
  });

  populateDemoCloud();
  reloadPlugins();
}

void MainWindow::populateDemoCloud() {
  const auto cloud = lv::db::MakeDemoPointCloud();
  gl_window_->setPointCloud(cloud);
  appendLog(QStringLiteral("Embedded qCC_db demo: %1").arg(lv::db::DescribePointCloud(cloud)));
}

void MainWindow::reloadPlugins() {
  const QString plugin_directory = lv::plugin::ccPluginManager::DefaultPluginDirectory(
      QCoreApplication::applicationDirPath());
  const int plugin_count = plugin_manager_.loadPlugins(plugin_directory);
  appendLog(QStringLiteral("Plugin scan: %1 plugin(s) in %2").arg(plugin_count).arg(plugin_directory));
  for (const auto& info : plugin_manager_.plugins()) {
    appendLog(QStringLiteral("  %1 [%2] - %3").arg(info.id, info.type, info.description));
  }
}

void MainWindow::appendLog(const QString& line) {
  log_view_->append(line);
}