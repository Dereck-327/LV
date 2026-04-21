#pragma once

#include <QString>
#include <QtPlugin>

namespace lv::plugin {

enum class PluginType {
  kStandard,
  kIO,
  kGL,
};

QString ToString(PluginType type);

class ccPluginInterface {
 public:
  virtual ~ccPluginInterface() = default;

  [[nodiscard]] virtual QString pluginId() const = 0;
  [[nodiscard]] virtual PluginType pluginType() const = 0;
  [[nodiscard]] virtual QString description() const = 0;
  virtual QString performDemo() = 0;
};

}  // namespace lv::plugin

#define LV_CC_PLUGIN_INTERFACE_IID "com.lv.CCPluginInterface/1.0"

Q_DECLARE_INTERFACE(lv::plugin::ccPluginInterface, LV_CC_PLUGIN_INTERFACE_IID)