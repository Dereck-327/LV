#pragma once

#include <QString>

#include <vector>

namespace lv::db {

struct Point3D {
  float x = 0.0F;
  float y = 0.0F;
  float z = 0.0F;
};

using PointCloud = std::vector<Point3D>;

PointCloud MakeDemoPointCloud(int rings = 6, int samples_per_ring = 24);
QString DescribePointCloud(const PointCloud& cloud);

}  // namespace lv::db