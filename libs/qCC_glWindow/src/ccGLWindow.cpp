#include "ccGLWindow.h"

#include <QMatrix4x4>
#include <QMouseEvent>
#include <QPainter>
#include <QVector4D>

#include <algorithm>
#include <cmath>
#include <optional>

namespace {

struct ScreenPoint {
  QPointF position;
  float depth = 0.0F;
  float size = 2.5F;
};

struct ProjectedPoint {
  QPointF position;
  float depth = 0.0F;
};

std::optional<ProjectedPoint> ProjectPoint(const QVector3D& point,
                                           const QMatrix4x4& projection,
                                           const QMatrix4x4& view,
                                           int width,
                                           int height) {
  const QVector4D clip = projection * view * QVector4D(point, 1.0F);
  if (clip.w() <= 0.0F) {
    return std::nullopt;
  }

  const QVector3D ndc(clip.x() / clip.w(), clip.y() / clip.w(), clip.z() / clip.w());
  if (std::abs(ndc.x()) > 1.3F || std::abs(ndc.y()) > 1.3F || ndc.z() < -1.3F || ndc.z() > 1.3F) {
    return std::nullopt;
  }

  const qreal screen_x = (ndc.x() * 0.5 + 0.5) * static_cast<qreal>(width);
  const qreal screen_y = (1.0 - (ndc.y() * 0.5 + 0.5)) * static_cast<qreal>(height);
  return ProjectedPoint{QPointF(screen_x, screen_y), ndc.z()};
}

void DrawProjectedLine(QPainter& painter,
                       const QVector3D& start,
                       const QVector3D& end,
                       const QMatrix4x4& projection,
                       const QMatrix4x4& view,
                       int width,
                       int height,
                       const QPen& pen) {
  const auto from = ProjectPoint(start, projection, view, width, height);
  const auto to = ProjectPoint(end, projection, view, width, height);
  if (!from.has_value() || !to.has_value()) {
    return;
  }

  painter.setPen(pen);
  painter.drawLine(from->position, to->position);
}

void DrawAxisLabel(QPainter& painter,
                   const QVector3D& anchor,
                   const QString& label,
                   const QColor& color,
                   const QMatrix4x4& projection,
                   const QMatrix4x4& view,
                   int width,
                   int height) {
  const auto projected = ProjectPoint(anchor, projection, view, width, height);
  if (!projected.has_value()) {
    return;
  }

  painter.setPen(color);
  painter.drawText(projected->position + QPointF(6.0, -6.0), label);
}

void DrawOrientationGizmo(QPainter& painter, int width, float yaw_degrees, float pitch_degrees) {
  const QPointF center(static_cast<qreal>(width - 70), 70.0);
  const float radius = 28.0F;

  QMatrix4x4 rotation;
  rotation.rotate(pitch_degrees, 1.0F, 0.0F, 0.0F);
  rotation.rotate(yaw_degrees, 0.0F, 1.0F, 0.0F);

  struct AxisVisual {
    QVector3D direction;
    QColor color;
    QString label;
  };

  const std::array<AxisVisual, 3> axes = {{{QVector3D(1.0F, 0.0F, 0.0F), QColor(234, 92, 92), QStringLiteral("X")},
                                           {QVector3D(0.0F, 1.0F, 0.0F), QColor(82, 214, 116), QStringLiteral("Y")},
                                           {QVector3D(0.0F, 0.0F, 1.0F), QColor(87, 163, 255), QStringLiteral("Z")}}};

  painter.setPen(QPen(QColor(170, 180, 195, 120), 1.0));
  painter.setBrush(QColor(20, 26, 36, 120));
  painter.drawEllipse(center, radius + 10.0F, radius + 10.0F);

  for (const auto& axis : axes) {
    const QVector3D dir = rotation.mapVector(axis.direction).normalized();
    const QPointF end(center.x() + dir.x() * radius, center.y() - dir.y() * radius);
    painter.setPen(QPen(axis.color, 2.0));
    painter.drawLine(center, end);
    painter.setBrush(axis.color);
    painter.drawEllipse(end, 3.5, 3.5);
    painter.setPen(axis.color);
    painter.drawText(end + QPointF(5.0, -4.0), axis.label);
  }
}

}  // namespace

ccGLWindow::ccGLWindow(QWidget* parent) : QOpenGLWidget(parent) {
  setMinimumSize(420, 320);
  setMouseTracking(true);
}

void ccGLWindow::setPointCloud(const lv::db::PointCloud& cloud) {
  cloud_ = cloud;
  updateSceneBounds();
  resetView();
  update();
}

int ccGLWindow::pointCount() const {
  return static_cast<int>(cloud_.size());
}

QString ccGLWindow::RendererBackendName() {
  return QStringLiteral("Interactive QOpenGLWidget point cloud viewer");
}

void ccGLWindow::resetView() {
  yaw_degrees_ = -32.0F;
  pitch_degrees_ = 18.0F;
  zoom_factor_ = 1.0F;
  pan_offset_ = {0.0F, 0.0F, 0.0F};
  update();
}

void ccGLWindow::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ccGLWindow::paintGL() {
  glViewport(0, 0, width(), height());
  glClearColor(0.08F, 0.09F, 0.12F, 1.0F);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  const float aspect_ratio = static_cast<float>(std::max(width(), 1)) / static_cast<float>(std::max(height(), 1));
  const float radius = sceneRadius();
  const float camera_distance = std::max(2.2F * radius / zoom_factor_, 0.25F);

  QMatrix4x4 projection;
  projection.perspective(45.0F, aspect_ratio, 0.01F, 100.0F);

  QMatrix4x4 view;
  view.translate(0.0F, 0.0F, -camera_distance);
  view.rotate(pitch_degrees_, 1.0F, 0.0F, 0.0F);
  view.rotate(yaw_degrees_, 0.0F, 1.0F, 0.0F);
  view.translate(-(scene_center_ + pan_offset_));

  std::vector<ScreenPoint> screen_points;
  screen_points.reserve(cloud_.size());
  for (const auto& point : cloud_) {
    const QVector4D world_point(point.x, point.y, point.z, 1.0F);
    const QVector4D clip = projection * view * world_point;
    if (clip.w() <= 0.0F) {
      continue;
    }

    const QVector3D ndc(clip.x() / clip.w(), clip.y() / clip.w(), clip.z() / clip.w());
    if (std::abs(ndc.x()) > 1.2F || std::abs(ndc.y()) > 1.2F || ndc.z() < -1.2F || ndc.z() > 1.2F) {
      continue;
    }

    const qreal screen_x = (ndc.x() * 0.5 + 0.5) * static_cast<qreal>(width());
    const qreal screen_y = (1.0 - (ndc.y() * 0.5 + 0.5)) * static_cast<qreal>(height());
    const float point_size = 1.8F + (1.0F - std::clamp(ndc.z(), -1.0F, 1.0F)) * 2.2F;
    screen_points.push_back({QPointF(screen_x, screen_y), ndc.z(), point_size});
  }

  std::sort(screen_points.begin(), screen_points.end(), [](const ScreenPoint& lhs, const ScreenPoint& rhs) {
    return lhs.depth > rhs.depth;
  });

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.fillRect(rect(), QColor(12, 16, 24, 24));

  const float radius_for_guides = std::max(sceneRadius(), 1.0F);
  const float grid_half_extent = radius_for_guides * 1.6F;
  const float grid_step = std::max(radius_for_guides / 6.0F, 0.16F);
  int line_index = 0;

  for (float value = -grid_half_extent; value <= grid_half_extent + 0.001F; value += grid_step) {
    const bool is_origin = std::abs(value) < 0.0001F;
    const bool is_major = (line_index % 3) == 0;
    const QColor grid_color = is_origin
                                  ? QColor(124, 136, 160, 210)
                                  : (is_major ? QColor(88, 101, 126, 165) : QColor(56, 64, 82, 110));
    const qreal line_width = is_origin ? 1.8 : (is_major ? 1.2 : 0.8);
    DrawProjectedLine(painter,
                      QVector3D(-grid_half_extent, value, 0.0F),
                      QVector3D(grid_half_extent, value, 0.0F),
                      projection,
                      view,
                      width(),
                      height(),
                      QPen(grid_color, line_width));
    DrawProjectedLine(painter,
                      QVector3D(value, -grid_half_extent, 0.0F),
                      QVector3D(value, grid_half_extent, 0.0F),
                      projection,
                      view,
                      width(),
                      height(),
                      QPen(grid_color, line_width));
              ++line_index;
  }

  painter.setPen(Qt::NoPen);
  for (const auto& point : screen_points) {
    const int blue = static_cast<int>(180 + (1.0F - point.depth) * 30.0F);
    painter.setBrush(QColor(88, 196, std::clamp(blue, 0, 255), 220));
    painter.drawEllipse(point.position, point.size, point.size);
  }

  painter.setPen(QColor(230, 236, 242));
  painter.drawText(rect().adjusted(12, 12, -12, -12), Qt::AlignTop | Qt::AlignLeft,
                   QStringLiteral("%1\n%2 points\nGrid + orientation gizmo enabled\nLMB rotate | MMB 3D drag | Wheel zoom | Double click reset")
                       .arg(RendererBackendName())
                       .arg(pointCount()));

    DrawOrientationGizmo(painter, width(), yaw_degrees_, pitch_degrees_);
}

void ccGLWindow::mousePressEvent(QMouseEvent* event) {
  last_mouse_position_ = event->pos();
  if (event->button() == Qt::LeftButton) {
    interaction_mode_ = InteractionMode::kRotate;
  } else if (event->button() == Qt::MiddleButton) {
    interaction_mode_ = InteractionMode::kPan;
  }
  event->accept();
}

void ccGLWindow::mouseMoveEvent(QMouseEvent* event) {
  const QPoint delta = event->pos() - last_mouse_position_;
  last_mouse_position_ = event->pos();

  if (interaction_mode_ == InteractionMode::kRotate) {
    yaw_degrees_ += delta.x() * 0.55F;
    pitch_degrees_ = std::clamp(pitch_degrees_ + delta.y() * 0.45F, -89.0F, 89.0F);
    update();
  } else if (interaction_mode_ == InteractionMode::kPan) {
    const float radius = sceneRadius();
    const float pan_scale = radius * 0.0032F / std::max(zoom_factor_, 0.2F);

    QMatrix4x4 camera_rotation;
    camera_rotation.rotate(yaw_degrees_, 0.0F, 1.0F, 0.0F);
    camera_rotation.rotate(pitch_degrees_, 1.0F, 0.0F, 0.0F);

    const QVector3D camera_right = camera_rotation.mapVector(QVector3D(1.0F, 0.0F, 0.0F)).normalized();
    const QVector3D camera_up = camera_rotation.mapVector(QVector3D(0.0F, 1.0F, 0.0F)).normalized();
    const QVector3D drag_translation = (-static_cast<float>(delta.x()) * camera_right +
                                        static_cast<float>(delta.y()) * camera_up) * pan_scale;

    pan_offset_ += drag_translation;
    update();
  }

  event->accept();
}

void ccGLWindow::mouseReleaseEvent(QMouseEvent* event) {
  interaction_mode_ = InteractionMode::kNone;
  event->accept();
}

void ccGLWindow::wheelEvent(QWheelEvent* event) {
  const qreal steps = static_cast<qreal>(event->angleDelta().y()) / 120.0;
  zoom_factor_ = std::clamp(zoom_factor_ * std::pow(1.18F, static_cast<float>(steps)), 0.2F, 25.0F);
  update();
  event->accept();
}

void ccGLWindow::mouseDoubleClickEvent(QMouseEvent* event) {
  resetView();
  event->accept();
}

void ccGLWindow::updateSceneBounds() {
  if (cloud_.empty()) {
    scene_center_ = {0.0F, 0.0F, 0.0F};
    scene_extent_ = {1.0F, 1.0F, 1.0F};
    return;
  }

  QVector3D min_bounds(cloud_.front().x, cloud_.front().y, cloud_.front().z);
  QVector3D max_bounds = min_bounds;
  for (const auto& point : cloud_) {
    min_bounds.setX(std::min(min_bounds.x(), point.x));
    min_bounds.setY(std::min(min_bounds.y(), point.y));
    min_bounds.setZ(std::min(min_bounds.z(), point.z));
    max_bounds.setX(std::max(max_bounds.x(), point.x));
    max_bounds.setY(std::max(max_bounds.y(), point.y));
    max_bounds.setZ(std::max(max_bounds.z(), point.z));
  }

  scene_center_ = 0.5F * (min_bounds + max_bounds);
  scene_extent_ = max_bounds - min_bounds;
}

float ccGLWindow::sceneRadius() const {
  return std::max({scene_extent_.x(), scene_extent_.y(), scene_extent_.z(), 1.0F}) * 0.75F;
}