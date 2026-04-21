#include "ccFileIO.h"
#include "ccPointCloud.h"
#include "support/PointCloudTestData.h"

#include <QDir>
#include <QString>

#include <gtest/gtest.h>

namespace {

class PointCloudWorkflowTest : public ::testing::Test {
 protected:
  void SetUp() override {
    input_cloud_ = lv::test::MakeReferenceCloud();
    output_file_path_ = QDir::current().filePath(lv::test::ReferenceCloudFileName());
  }

  lv::db::PointCloud input_cloud_;
  QString output_file_path_;
};

TEST_F(PointCloudWorkflowTest, DemoCloudDescriptionMatchesPointCount) {
  const QString description = lv::db::DescribePointCloud(input_cloud_);
  EXPECT_TRUE(description.contains(QString::number(input_cloud_.size())));
}

TEST_F(PointCloudWorkflowTest, SavedCloudCanBeLoadedBackWithoutLosingPointCount) {
  QString error_message;
  ASSERT_TRUE(lv::io::SavePointCloudAsText(output_file_path_, input_cloud_, &error_message))
      << error_message.toStdString();

  const lv::db::PointCloud loaded_cloud = lv::io::LoadPointCloudFromText(output_file_path_, &error_message);
  ASSERT_TRUE(error_message.isEmpty()) << error_message.toStdString();
  EXPECT_EQ(loaded_cloud.size(), input_cloud_.size());
}

}  // namespace