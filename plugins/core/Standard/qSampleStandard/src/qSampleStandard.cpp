#include "qSampleStandard.h"

#include "ccAppCommon.h"

QString qSampleStandard::pluginId() const {
  return QStringLiteral("qSampleStandard");
}

lv::plugin::PluginType qSampleStandard::pluginType() const {
  return lv::plugin::PluginType::kStandard;
}

QString qSampleStandard::description() const {
  return QStringLiteral("Standard plugin demo mirroring CloudCompare's plugin folder layout.");
}

QString qSampleStandard::performDemo() {
  return QStringLiteral("Standard plugin executed: %1").arg(lv::app::BuildSummary());
}