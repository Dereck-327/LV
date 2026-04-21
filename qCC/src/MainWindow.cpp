#include "MainWindow.h"

#include "ccAppCommon.h"
#include "ccConsole.h"
#include "ccFileIO.h"
#include "ccGLWindow.h"

#include "ui_mainwindow.h"

#include <spdlog/spdlog.h>

#include <QCoreApplication>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui_(std::make_unique<Ui::MainWindow>()), plugin_manager_(this) {
  ui_->setupUi(this);
  setWindowTitle(QStringLiteral("CloudCompare UI on LV"));

  setupCentralView();
  setupTreeModels();
  setupLogging();
  setupActions();

  populateDemoCloud();
  reloadPlugins();
  refreshProperties();

  statusBar()->showMessage(QStringLiteral("CloudCompare-style UI loaded"));
  spdlog::info("Main window initialized with legacy CloudCompare UI layout");
}

MainWindow::~MainWindow() {
  spdlog::info("Shutting down LV UI");
  ccConsole::Shutdown();
}

void MainWindow::setupCentralView() {
  auto* layout = new QVBoxLayout(ui_->centralwidget);
  layout->setContentsMargins(0, 0, 0, 0);

  gl_window_ = new ccGLWindow(ui_->centralwidget);
  layout->addWidget(gl_window_);
}

void MainWindow::setupTreeModels() {
  db_model_ = new QStandardItemModel(this);
  db_model_->setHorizontalHeaderLabels({QStringLiteral("DB Tree")});
  ui_->dbTreeView->setModel(db_model_);
  ui_->dbTreeView->setHeaderHidden(false);

  properties_model_ = new QStandardItemModel(this);
  properties_model_->setHorizontalHeaderLabels({QStringLiteral("Property"), QStringLiteral("Value")});
  ui_->propertiesTreeView->setModel(properties_model_);
}

void MainWindow::setupActions() {
  connect(ui_->actionOpen, &QAction::triggered, this, &MainWindow::openPointCloud);
  connect(ui_->actionSave, &QAction::triggered, this, &MainWindow::savePointCloud);
  connect(ui_->actionRefresh, &QAction::triggered, this, [this]() {
    populateDemoCloud();
    reloadPlugins();
    refreshProperties();
    spdlog::info("UI refreshed");
  });
  connect(ui_->actionQuit, &QAction::triggered, this, &QWidget::close);
  connect(ui_->actionAbout, &QAction::triggered, this, [this]() {
    QMessageBox::about(this, QStringLiteral("About LV"), lv::app::BuildSummary());
  });
  connect(ui_->actionAboutPlugins, &QAction::triggered, this, [this]() {
    QMessageBox::information(this,
                             QStringLiteral("Plugins"),
                             QStringLiteral("Loaded %1 plugin(s)").arg(plugin_manager_.plugins().size()));
  });
}

void MainWindow::setupLogging() {
  ccConsole::Init(ui_->consoleWidget, this);
  ccConsole::RegisterQtMessageHandler();
  spdlog::info("spdlog console bridge initialized");
}

void MainWindow::refreshProperties() {
  properties_model_->removeRows(0, properties_model_->rowCount());

  const auto add_property = [this](const QString& name, const QString& value) {
    properties_model_->appendRow({new QStandardItem(name), new QStandardItem(value)});
  };

  add_property(QStringLiteral("Application"), QStringLiteral("LV_app"));
  add_property(QStringLiteral("Renderer"), ccGLWindow::RendererBackendName());
  add_property(QStringLiteral("Current cloud"), lv::db::DescribePointCloud(current_cloud_));
  add_property(QStringLiteral("Plugins"), QString::number(plugin_manager_.plugins().size()));
  add_property(QStringLiteral("Interaction"), QStringLiteral("Left drag rotate / Middle drag 3D pan / Wheel zoom / Double click reset"));
  add_property(QStringLiteral("Scene guides"), QStringLiteral("Floor grid and orientation gizmo"));
  add_property(QStringLiteral("Build summary"), lv::app::BuildSummary());
}

void MainWindow::populateDemoCloud() {
  current_cloud_ = lv::db::MakeDemoPointCloud();
  gl_window_->setPointCloud(current_cloud_);

  db_model_->removeRows(0, db_model_->rowCount());

  auto* scene_item = new QStandardItem(QStringLiteral("Scene"));
  scene_item->appendRow(new QStandardItem(lv::db::DescribePointCloud(current_cloud_)));
  scene_item->appendRow(new QStandardItem(ccGLWindow::RendererBackendName()));

  db_model_->appendRow(scene_item);
  ui_->dbTreeView->expandAll();

  refreshProperties();
  spdlog::info("Embedded interactive 3D demo prepared: {} points", current_cloud_.size());
}

void MainWindow::reloadPlugins() {
  const QString plugin_directory = lv::plugin::ccPluginManager::DefaultPluginDirectory(
      QCoreApplication::applicationDirPath());
  const int plugin_count = plugin_manager_.loadPlugins(plugin_directory);

  auto* plugin_root = new QStandardItem(QStringLiteral("Plugins"));
  for (const auto& info : plugin_manager_.plugins()) {
    plugin_root->appendRow(new QStandardItem(QStringLiteral("%1 [%2]").arg(info.id, info.type)));
    spdlog::info("Loaded plugin {} ({})", info.id.toStdString(), info.type.toStdString());
  }

  db_model_->appendRow(plugin_root);
  ui_->dbTreeView->expandAll();
  spdlog::info("Plugin scan completed in {} with {} plugin(s)", plugin_directory.toStdString(), plugin_count);
}

void MainWindow::openPointCloud() {
  const QString path = QFileDialog::getOpenFileName(this,
                                                    QStringLiteral("Load point cloud"),
                                                    QDir::homePath(),
                                                    QStringLiteral("Point Cloud (*.txt)"));
  if (path.isEmpty()) {
    return;
  }

  QString error_message;
  const auto cloud = lv::io::LoadPointCloudFromText(path, &error_message);
  if (!error_message.isEmpty()) {
    spdlog::error("Failed to load {}: {}", path.toStdString(), error_message.toStdString());
    QMessageBox::warning(this, QStringLiteral("Load failed"), error_message);
    return;
  }

  current_cloud_ = cloud;
  gl_window_->setPointCloud(current_cloud_);
  refreshProperties();
  spdlog::info("Loaded point cloud from {}", path.toStdString());
}

void MainWindow::savePointCloud() {
  const QString path = QFileDialog::getSaveFileName(this,
                                                    QStringLiteral("Save point cloud"),
                                                    QDir::homePath(),
                                                    QStringLiteral("Point Cloud (*.txt)"));
  if (path.isEmpty()) {
    return;
  }

  QString error_message;
  if (!lv::io::SavePointCloudAsText(path, current_cloud_, &error_message)) {
    spdlog::error("Failed to save {}: {}", path.toStdString(), error_message.toStdString());
    QMessageBox::warning(this, QStringLiteral("Save failed"), error_message);
    return;
  }

  spdlog::info("Saved point cloud to {}", path.toStdString());
}