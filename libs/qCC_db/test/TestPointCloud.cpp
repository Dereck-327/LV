#include "ccPointCloud.h"

#include <gtest/gtest.h>

TEST(QCC_DB, DemoCloudHasRequestedSize) {
  const auto cloud = lv::db::MakeDemoPointCloud(3, 10);
  EXPECT_EQ(cloud.size(), 30U);
  EXPECT_TRUE(lv::db::DescribePointCloud(cloud).contains(QStringLiteral("30")));
}