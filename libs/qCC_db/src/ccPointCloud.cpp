#include "ccPointCloud.h"

#include <QtGlobal>

#include <cmath>

namespace lv::db {

PointCloud MakeDemoPointCloud(int rings, int samples_per_ring) {
  PointCloud cloud;
  cloud.reserve(static_cast<size_t>(qMax(rings, 0) * qMax(samples_per_ring, 0)));

  for (int ring = 0; ring < rings; ++ring) {
    const float layer_ratio = static_cast<float>(ring) / static_cast<float>(qMax(rings - 1, 1));
    const float major_radius = 0.45F + layer_ratio * 0.45F;
    const float minor_radius = 0.12F + 0.08F * std::sin(layer_ratio * 6.28318530718F);
    for (int sample = 0; sample < samples_per_ring; ++sample) {
      const float angle = static_cast<float>(sample) * 6.28318530718F /
                          static_cast<float>(qMax(samples_per_ring, 1));
      const float twist = angle * 2.0F + layer_ratio * 5.5F;
      const float radius = major_radius + minor_radius * std::cos(twist);
      const float x = radius * std::cos(angle);
      const float y = radius * std::sin(angle);
      const float z = minor_radius * std::sin(twist) + (layer_ratio - 0.5F) * 1.6F;
      cloud.push_back({x, y, z});
    }
  }

  return cloud;
}

QString DescribePointCloud(const PointCloud& cloud) {
  return QStringLiteral("Interactive 3D demo cloud with %1 points").arg(cloud.size());
}

}  // namespace lv::db