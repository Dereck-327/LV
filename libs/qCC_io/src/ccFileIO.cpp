#include "ccFileIO.h"

#include <QFile>
#include <QTextStream>

namespace lv::io {

bool SavePointCloudAsText(const QString& file_path, const lv::db::PointCloud& cloud, QString* error_message) {
  QFile file(file_path);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
    if (error_message != nullptr) {
      *error_message = file.errorString();
    }
    return false;
  }

  QTextStream stream(&file);
  stream << "LV_POINT_CLOUD_V1 " << cloud.size() << '\n';
  for (const auto& point : cloud) {
    stream << point.x << ' ' << point.y << ' ' << point.z << '\n';
  }
  return true;
}

lv::db::PointCloud LoadPointCloudFromText(const QString& file_path, QString* error_message) {
  QFile file(file_path);
  lv::db::PointCloud cloud;
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    if (error_message != nullptr) {
      *error_message = file.errorString();
    }
    return cloud;
  }

  QTextStream stream(&file);
  QString header;
  qsizetype expected_points = 0;
  stream >> header >> expected_points;
  if (header != QStringLiteral("LV_POINT_CLOUD_V1")) {
    if (error_message != nullptr) {
      *error_message = QStringLiteral("Unsupported point cloud header: %1").arg(header);
    }
    return {};
  }

  cloud.reserve(static_cast<size_t>(expected_points));
  while (!stream.atEnd()) {
    lv::db::Point3D point;
    stream >> point.x >> point.y >> point.z;
    if (stream.status() != QTextStream::Ok) {
      break;
    }
    cloud.push_back(point);
  }

  return cloud;
}

}  // namespace lv::io