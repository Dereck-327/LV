#include "ccAppCommon.h"

#include <gtest/gtest.h>

TEST(CCAppCommon, BuildSummaryMentionsCoreLibraries) {
  const QString summary = lv::app::BuildSummary();
  EXPECT_TRUE(summary.contains(QStringLiteral("LV_app")));
  EXPECT_TRUE(summary.contains(QStringLiteral("QCC_DB_LIB")));
}