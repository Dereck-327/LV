#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QTimer;
class QWidget;

namespace open3d {
namespace geometry {
class PointCloud;
}
namespace visualization {
class Visualizer;
}
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void InitOpen3DWidget();
    std::shared_ptr<open3d::geometry::PointCloud> CreateDemoPointCloud() const;

    Ui::MainWindow *ui;
    QWidget *open3dHostWidget_ = nullptr;
    QWidget *open3dContainerWidget_ = nullptr;
    QTimer *renderTimer_ = nullptr;
    std::unique_ptr<open3d::visualization::Visualizer> visualizer_;
};
#endif // MAINWINDOW_H
