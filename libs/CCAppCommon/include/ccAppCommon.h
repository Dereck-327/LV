#pragma once

#include <QString>
#include <QStringList>

namespace lv::app {

struct BuildInfo {
  QString application_name;
  QString version;
  QStringList modules;
};

BuildInfo MakeBuildInfo();
QString BuildSummary();

}  // namespace lv::app