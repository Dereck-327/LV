#pragma once

#include "ccPluginInterface.h"

#include <QObject>

class qSampleGL final : public QObject, public lv::plugin::ccPluginInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID LV_CC_PLUGIN_INTERFACE_IID)
  Q_INTERFACES(lv::plugin::ccPluginInterface)

 public:
  [[nodiscard]] QString pluginId() const override;
  [[nodiscard]] lv::plugin::PluginType pluginType() const override;
  [[nodiscard]] QString description() const override;
  QString performDemo() override;
};