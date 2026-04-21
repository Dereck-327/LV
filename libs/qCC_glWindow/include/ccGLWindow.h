#pragma once

#include "ccPointCloud.h"

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include <QPoint>
#include <QVector3D>

class ccGLWindow : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  explicit ccGLWindow(QWidget* parent = nullptr);

  void setPointCloud(const lv::db::PointCloud& cloud);
  [[nodiscard]] int pointCount() const;
  [[nodiscard]] static QString RendererBackendName();
  void resetView();

 protected:
  void initializeGL() override;
  void paintGL() override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;
  void mouseDoubleClickEvent(QMouseEvent* event) override;

 private:
  enum class InteractionMode {
    kNone,
    kRotate,
    kPan,
  };

  void updateSceneBounds();
  [[nodiscard]] float sceneRadius() const;

  lv::db::PointCloud cloud_;
  QVector3D scene_center_ = {0.0F, 0.0F, 0.0F};
  QVector3D scene_extent_ = {1.0F, 1.0F, 1.0F};
  float yaw_degrees_ = -32.0F;
  float pitch_degrees_ = 18.0F;
  float zoom_factor_ = 1.0F;
  QVector3D pan_offset_ = {0.0F, 0.0F, 0.0F};
  QPoint last_mouse_position_;
  InteractionMode interaction_mode_ = InteractionMode::kNone;
};