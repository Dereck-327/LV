#include "qSampleGL.h"

#include "ccGLWindow.h"

QString qSampleGL::pluginId() const {
  return QStringLiteral("qSampleGL");
}

lv::plugin::PluginType qSampleGL::pluginType() const {
  return lv::plugin::PluginType::kGL;
}

QString qSampleGL::description() const {
  return QStringLiteral("GL plugin demo using the qCC_glWindow abstraction.");
}

QString qSampleGL::performDemo() {
  return QStringLiteral("GL plugin ready with backend: %1").arg(ccGLWindow::RendererBackendName());
}