#pragma once

#include "ccPointCloud.h"

#include <QString>

namespace lv::io {

bool SavePointCloudAsText(const QString& file_path, const lv::db::PointCloud& cloud, QString* error_message = nullptr);
lv::db::PointCloud LoadPointCloudFromText(const QString& file_path, QString* error_message = nullptr);

}  // namespace lv::io