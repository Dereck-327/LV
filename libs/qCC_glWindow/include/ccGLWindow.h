#pragma once

#include "ccPointCloud.h"

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

class ccGLWindow : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  explicit ccGLWindow(QWidget* parent = nullptr);

  void setPointCloud(const lv::db::PointCloud& cloud);
  [[nodiscard]] int pointCount() const;
  [[nodiscard]] static QString RendererBackendName();

 protected:
  void initializeGL() override;
  void paintGL() override;

 private:
  lv::db::PointCloud cloud_;
};