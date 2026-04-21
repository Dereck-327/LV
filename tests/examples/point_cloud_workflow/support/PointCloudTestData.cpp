#include "support/PointCloudTestData.h"

namespace lv::test {

lv::db::PointCloud MakeReferenceCloud() {
  return lv::db::MakeDemoPointCloud(4, 12);
}

QString ReferenceCloudFileName() {
  return QStringLiteral("point_cloud_workflow_demo.txt");
}

}  // namespace lv::test