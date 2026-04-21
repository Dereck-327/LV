#include "ccPluginManager.h"

#include <gtest/gtest.h>

TEST(CCPluginAPI, DefaultPluginDirectoryEndsWithPlugins) {
  const QString path = lv::plugin::ccPluginManager::DefaultPluginDirectory(QStringLiteral("C:/demo/bin"));
  EXPECT_TRUE(path.endsWith(QStringLiteral("plugins")));
}