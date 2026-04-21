#include "ccPluginManager.h"

#include <QDir>
#include <QPluginLoader>

namespace lv::plugin {

ccPluginManager::ccPluginManager(QObject* parent) : QObject(parent) {}

int ccPluginManager::loadPlugins(const QString& plugin_directory) {
  plugins_.clear();

  QDir dir(plugin_directory);
  if (!dir.exists()) {
    return 0;
  }

#if defined(Q_OS_WIN)
  const QStringList name_filters = {QStringLiteral("*.dll")};
#elif defined(Q_OS_MACOS)
  const QStringList name_filters = {QStringLiteral("*.dylib")};
#else
  const QStringList name_filters = {QStringLiteral("*.so")};
#endif

  const QFileInfoList candidates = dir.entryInfoList(name_filters, QDir::Files | QDir::Readable);
  for (const QFileInfo& candidate : candidates) {
    auto loader = std::make_unique<QPluginLoader>(candidate.absoluteFilePath());
    QObject* instance = loader->instance();
    if (instance == nullptr) {
      continue;
    }

    auto* plugin = qobject_cast<ccPluginInterface*>(instance);
    if (plugin == nullptr) {
      continue;
    }

    PluginInfo info{
        plugin->pluginId(),
        ToString(plugin->pluginType()),
        plugin->description(),
        candidate.absoluteFilePath(),
    };
    plugins_.push_back({info, plugin, std::move(loader)});
  }

  return static_cast<int>(plugins_.size());
}

std::vector<PluginInfo> ccPluginManager::plugins() const {
  std::vector<PluginInfo> result;
  result.reserve(plugins_.size());
  for (const auto& plugin : plugins_) {
    result.push_back(plugin.info);
  }
  return result;
}

QStringList ccPluginManager::runAllDemos() const {
  QStringList messages;
  for (const auto& plugin : plugins_) {
    messages.push_back(QStringLiteral("[%1] %2").arg(plugin.info.id, plugin.plugin->performDemo()));
  }
  return messages;
}

QString ccPluginManager::DefaultPluginDirectory(const QString& app_directory) {
  return QDir(app_directory).filePath(QStringLiteral("plugins"));
}

}  // namespace lv::plugin