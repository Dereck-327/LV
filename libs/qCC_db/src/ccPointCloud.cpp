#include "ccPointCloud.h"

#include <QtGlobal>

#include <cmath>

namespace lv::db {

PointCloud MakeDemoPointCloud(int rings, int samples_per_ring) {
  PointCloud cloud;
  cloud.reserve(static_cast<size_t>(qMax(rings, 0) * qMax(samples_per_ring, 0)));

  for (int ring = 1; ring <= rings; ++ring) {
    const float radius = static_cast<float>(ring) / static_cast<float>(qMax(rings, 1));
    for (int sample = 0; sample < samples_per_ring; ++sample) {
      const float angle = static_cast<float>(sample) * 6.28318530718F /
                          static_cast<float>(qMax(samples_per_ring, 1));
      cloud.push_back({radius * std::cos(angle), radius * std::sin(angle), radius - 0.5F});
    }
  }

  return cloud;
}

QString DescribePointCloud(const PointCloud& cloud) {
  return QStringLiteral("Point cloud demo with %1 points").arg(cloud.size());
}

}  // namespace lv::db