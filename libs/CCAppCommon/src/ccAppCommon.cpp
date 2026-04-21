#include "ccAppCommon.h"

#include "ccGLWindow.h"

namespace lv::app {

BuildInfo MakeBuildInfo() {
  return {
      QStringLiteral("LV_app"),
      QStringLiteral("0.1.0"),
      {
          QStringLiteral("QCC_DB_LIB"),
          QStringLiteral("QCC_IO_LIB"),
          QStringLiteral("QCC_GL_LIB"),
          QStringLiteral("CCPluginAPI"),
          ccGLWindow::RendererBackendName(),
      },
  };
}

QString BuildSummary() {
  const BuildInfo info = MakeBuildInfo();
  return QStringLiteral("%1 %2 | modules: %3")
      .arg(info.application_name, info.version, info.modules.join(QStringLiteral(", ")));
}

}  // namespace lv::app