#include "ccFileIO.h"

#include <QDir>
#include <QTemporaryDir>

#include <gtest/gtest.h>

TEST(QCC_IO, PointCloudRoundTrip) {
  QTemporaryDir temp_dir;
  const QString file_path = QDir(temp_dir.path()).filePath(QStringLiteral("cloud.txt"));

  QString error_message;
  const auto source_cloud = lv::db::MakeDemoPointCloud(2, 6);
  ASSERT_TRUE(lv::io::SavePointCloudAsText(file_path, source_cloud, &error_message)) << error_message.toStdString();

  const auto loaded_cloud = lv::io::LoadPointCloudFromText(file_path, &error_message);
  ASSERT_TRUE(error_message.isEmpty()) << error_message.toStdString();
  EXPECT_EQ(loaded_cloud.size(), source_cloud.size());
}