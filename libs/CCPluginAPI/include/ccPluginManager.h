#pragma once

#include "ccPluginInterface.h"

#include <QObject>
#include <QPluginLoader>
#include <QStringList>

#include <memory>
#include <vector>

namespace lv::plugin {

struct PluginInfo {
  QString id;
  QString type;
  QString description;
  QString file_path;
};

class ccPluginManager : public QObject {
  Q_OBJECT

 public:
  explicit ccPluginManager(QObject* parent = nullptr);

  int loadPlugins(const QString& plugin_directory);
  [[nodiscard]] std::vector<PluginInfo> plugins() const;
  [[nodiscard]] QStringList runAllDemos() const;
  [[nodiscard]] static QString DefaultPluginDirectory(const QString& app_directory);

 private:
  struct LoadedPlugin {
    PluginInfo info;
    ccPluginInterface* plugin = nullptr;
    std::unique_ptr<QPluginLoader> loader;
  };

  std::vector<LoadedPlugin> plugins_;
};

}  // namespace lv::plugin