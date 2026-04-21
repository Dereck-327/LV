#include "ccPluginInterface.h"

namespace lv::plugin {

QString ToString(PluginType type) {
  switch (type) {
    case PluginType::kStandard:
      return QStringLiteral("standard");
    case PluginType::kIO:
      return QStringLiteral("io");
    case PluginType::kGL:
      return QStringLiteral("gl");
  }

  return QStringLiteral("unknown");
}

}  // namespace lv::plugin