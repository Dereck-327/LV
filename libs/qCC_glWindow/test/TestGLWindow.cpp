#include "ccGLWindow.h"

#include <gtest/gtest.h>

TEST(QCC_GL, RendererBackendNameIsExposed) {
  EXPECT_FALSE(ccGLWindow::RendererBackendName().isEmpty());
}