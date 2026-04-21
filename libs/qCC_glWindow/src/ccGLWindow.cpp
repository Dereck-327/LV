#include "ccGLWindow.h"

#include <QPainter>

ccGLWindow::ccGLWindow(QWidget* parent) : QOpenGLWidget(parent) {
  setMinimumSize(420, 320);
}

void ccGLWindow::setPointCloud(const lv::db::PointCloud& cloud) {
  cloud_ = cloud;
  update();
}

int ccGLWindow::pointCount() const {
  return static_cast<int>(cloud_.size());
}

QString ccGLWindow::RendererBackendName() {
  return QStringLiteral("QOpenGLWidget + OpenGL::GL");
}

void ccGLWindow::initializeGL() {
  initializeOpenGLFunctions();
}

void ccGLWindow::paintGL() {
  glViewport(0, 0, width(), height());
  glClearColor(0.08F, 0.09F, 0.12F, 1.0F);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setPen(Qt::NoPen);
  painter.setBrush(QColor(74, 184, 255));

  const QRectF drawing_rect = rect().adjusted(24.0, 24.0, -24.0, -24.0);
  for (const auto& point : cloud_) {
    const qreal x = drawing_rect.center().x() + static_cast<qreal>(point.x) * drawing_rect.width() * 0.45;
    const qreal y = drawing_rect.center().y() - static_cast<qreal>(point.y) * drawing_rect.height() * 0.45;
    painter.drawEllipse(QPointF(x, y), 3.0, 3.0);
  }

  painter.setPen(QColor(230, 236, 242));
  painter.drawText(rect().adjusted(12, 12, -12, -12), Qt::AlignTop | Qt::AlignLeft,
                   QStringLiteral("%1\n%2 points").arg(RendererBackendName()).arg(pointCount()));
}