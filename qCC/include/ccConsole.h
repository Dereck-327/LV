#pragma once

#include <QListWidget>
#include <QObject>

class QKeyEvent;
class QWidget;

class ccCustomQListWidget : public QListWidget {
  Q_OBJECT

 public:
  explicit ccCustomQListWidget(QWidget* parent = nullptr);

 protected:
  void keyPressEvent(QKeyEvent* event) override;
};

class ccConsole : public QObject {
  Q_OBJECT

 public:
  static ccConsole* Instance();
  static void Init(ccCustomQListWidget* text_display, QWidget* parent_widget = nullptr);
  static void Shutdown();
  static void RegisterQtMessageHandler();

  void appendMessage(int level, const QString& message);

 private:
  explicit ccConsole(QObject* parent = nullptr);

  ccCustomQListWidget* text_display_ = nullptr;
  QWidget* parent_widget_ = nullptr;
};