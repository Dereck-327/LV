#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <memory>

#include "ui_mainwindow.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
