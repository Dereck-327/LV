#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <open3d/Open3D.h>

#include <algorithm>
#include <cmath>

#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <QWindow>

#ifdef _WIN32
#include <windows.h>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitOpen3DWidget();
}

MainWindow::~MainWindow()
{
    if (renderTimer_ != nullptr) {
        renderTimer_->stop();
    }
    if (visualizer_ != nullptr) {
        visualizer_->DestroyVisualizerWindow();
    }
    delete ui;
}

void MainWindow::InitOpen3DWidget()
{
    auto *rootLayout = new QVBoxLayout(ui->centralwidget);
    rootLayout->setContentsMargins(8, 8, 8, 8);
    rootLayout->setSpacing(8);

    auto *toolBarLayout = new QHBoxLayout();
    auto *hintLabel = new QLabel(tr("Open3D交互: 左键旋转, 中键平移, 滚轮缩放"), ui->centralwidget);
    auto *resetViewButton = new QPushButton(tr("重置视角"), ui->centralwidget);
    toolBarLayout->addWidget(hintLabel);
    toolBarLayout->addStretch();
    toolBarLayout->addWidget(resetViewButton);
    rootLayout->addLayout(toolBarLayout);

    open3dHostWidget_ = new QWidget(ui->centralwidget);
    open3dHostWidget_->setMinimumSize(640, 420);
    rootLayout->addWidget(open3dHostWidget_, 1);

    visualizer_ = std::make_unique<open3d::visualization::Visualizer>();
    const bool created = visualizer_->CreateVisualizerWindow("LV Open3D Widget", 960, 720, 100, 100, true);
    if (!created) {
        QMessageBox::warning(this, tr("Open3D"), tr("无法创建Open3D可视化窗口。"));
        return;
    }

    auto cloud = CreateDemoPointCloud();
    visualizer_->AddGeometry(cloud);
    visualizer_->GetRenderOption().point_size_ = 3.0;
    visualizer_->GetViewControl().SetZoom(0.8);

#ifdef _WIN32
    const HWND hwnd = FindWindowW(nullptr, L"LV Open3D Widget");
    if (hwnd == nullptr) {
        QMessageBox::warning(this, tr("Open3D"), tr("未找到Open3D窗口句柄，无法嵌入Qt。"));
        return;
    }
    auto *nativeWindow = QWindow::fromWinId(reinterpret_cast<WId>(hwnd));
    open3dContainerWidget_ = QWidget::createWindowContainer(nativeWindow, open3dHostWidget_);
    auto *hostLayout = new QVBoxLayout(open3dHostWidget_);
    hostLayout->setContentsMargins(0, 0, 0, 0);
    hostLayout->addWidget(open3dContainerWidget_);
#else
    QMessageBox::warning(this, tr("Open3D"), tr("当前示例仅在Windows下支持嵌入。"));
    return;
#endif

    renderTimer_ = new QTimer(this);
    connect(renderTimer_, &QTimer::timeout, this, [this]() {
        if (visualizer_ != nullptr) {
            visualizer_->PollEvents();
            visualizer_->UpdateRender();
        }
    });
    renderTimer_->start(16);

    connect(resetViewButton, &QPushButton::clicked, this, [this]() {
        if (visualizer_ == nullptr) {
            return;
        }
        visualizer_->ResetViewPoint(true);
        visualizer_->UpdateRender();
    });
}

std::shared_ptr<open3d::geometry::PointCloud> MainWindow::CreateDemoPointCloud() const
{
    auto cloud = std::make_shared<open3d::geometry::PointCloud>();
    cloud->points_.reserve(2400);
    cloud->colors_.reserve(2400);

    for (int i = -30; i <= 30; ++i) {
        for (int j = -30; j <= 30; ++j) {
            const double x = static_cast<double>(i) * 0.05;
            const double y = static_cast<double>(j) * 0.05;
            const double z = 0.2 * std::sin(x * 3.0) * std::cos(y * 3.0);
            cloud->points_.emplace_back(x, y, z);

            const double r = (x + 1.5) / 3.0;
            const double g = (y + 1.5) / 3.0;
            const double b = (z + 0.2) / 0.4;
            cloud->colors_.emplace_back(
                std::clamp(r, 0.0, 1.0),
                std::clamp(g, 0.0, 1.0),
                std::clamp(b, 0.0, 1.0));
        }
    }

    return cloud;
}
