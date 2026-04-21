#pragma once

#include "ccPointCloud.h"

#include <QString>

namespace lv::test {

lv::db::PointCloud MakeReferenceCloud();
QString ReferenceCloudFileName();

}  // namespace lv::test