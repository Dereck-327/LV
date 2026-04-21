#include "ccGLWindow.h"

#include <QApplication>

#include <gtest/gtest.h>

TEST(QCC_GL, RendererBackendNameIsExposed) {
  EXPECT_FALSE(ccGLWindow::RendererBackendName().isEmpty());
}

TEST(QCC_GL, ResetViewKeepsViewerConstructible) {
  int argc = 0;
  char** argv = nullptr;
  QApplication app(argc, argv);
  ccGLWindow window;
  window.resetView();
  EXPECT_EQ(window.pointCount(), 0);
}