#include "qSampleIO.h"

#include "ccFileIO.h"
#include "ccPointCloud.h"

#include <QDir>
#include <QTemporaryDir>

QString qSampleIO::pluginId() const {
  return QStringLiteral("qSampleIO");
}

lv::plugin::PluginType qSampleIO::pluginType() const {
  return lv::plugin::PluginType::kIO;
}

QString qSampleIO::description() const {
  return QStringLiteral("IO plugin demo for text point cloud import/export.");
}

QString qSampleIO::performDemo() {
  QTemporaryDir temp_dir;
  const QString path = QDir(temp_dir.path()).filePath(QStringLiteral("plugin_io_demo.txt"));
  const auto cloud = lv::db::MakeDemoPointCloud(2, 8);
  QString error_message;
  if (!lv::io::SavePointCloudAsText(path, cloud, &error_message)) {
    return QStringLiteral("IO plugin failed to save demo: %1").arg(error_message);
  }

  const auto loaded_cloud = lv::io::LoadPointCloudFromText(path, &error_message);
  if (!error_message.isEmpty()) {
    return QStringLiteral("IO plugin failed to load demo: %1").arg(error_message);
  }

  return QStringLiteral("IO plugin round-tripped %1 points").arg(loaded_cloud.size());
}