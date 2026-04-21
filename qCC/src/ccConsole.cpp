#include "ccConsole.h"

#include <spdlog/async.h>
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <QApplication>
#include <QClipboard>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QKeyEvent>
#include <QListWidgetItem>
#include <QMetaObject>
#include <QPointer>

#include <memory>
#include <mutex>
#include <vector>

namespace {

ccConsole* g_console = nullptr;
QtMessageHandler g_previous_qt_handler = nullptr;

QColor LevelColor(spdlog::level::level_enum level) {
  switch (level) {
    case spdlog::level::warn:
      return QColor(232, 170, 60);
    case spdlog::level::err:
    case spdlog::level::critical:
      return QColor(225, 89, 89);
    case spdlog::level::debug:
      return QColor(92, 173, 255);
    default:
      return QColor(230, 236, 242);
  }
}

class QtListWidgetSink final : public spdlog::sinks::sink {
 public:
  explicit QtListWidgetSink(QPointer<ccConsole> console) : console_(console) {}

  void log(const spdlog::details::log_msg& msg) override {
    spdlog::memory_buf_t formatted;
    formatter_->format(msg, formatted);
    const QString text = QString::fromUtf8(formatted.data(), static_cast<int>(formatted.size())).trimmed();
    const int level = static_cast<int>(msg.level);

    QMetaObject::invokeMethod(
        qApp,
        [console = console_, level, text]() {
          if (console) {
            console->appendMessage(level, text);
          }
        },
        Qt::QueuedConnection);
  }

  void flush() override {}

  void set_pattern(const std::string& pattern) override {
    set_formatter(std::make_unique<spdlog::pattern_formatter>(pattern));
  }

  void set_formatter(std::unique_ptr<spdlog::formatter> sink_formatter) override {
    formatter_ = std::move(sink_formatter);
  }

 private:
  QPointer<ccConsole> console_;
  std::unique_ptr<spdlog::formatter> formatter_ = std::make_unique<spdlog::pattern_formatter>();
};

void QtMessageBridge(QtMsgType type, const QMessageLogContext& context, const QString& message) {
  const std::string payload = QStringLiteral("[Qt] %1 (%2:%3)").arg(message, context.file ? context.file : "?", QString::number(context.line)).toStdString();
  switch (type) {
    case QtDebugMsg:
      spdlog::debug("{}", payload);
      break;
    case QtInfoMsg:
      spdlog::info("{}", payload);
      break;
    case QtWarningMsg:
      spdlog::warn("{}", payload);
      break;
    case QtCriticalMsg:
    case QtFatalMsg:
      spdlog::error("{}", payload);
      break;
  }
}

void EnsureDefaultLogger(QPointer<ccConsole> console) {
  auto logger = spdlog::default_logger();
  if (logger != nullptr && logger->name() == "lv") {
    return;
  }

  const QString log_dir = QDir(QCoreApplication::applicationDirPath()).filePath(QStringLiteral("logs"));
  QDir().mkpath(log_dir);
  const QString log_file = QDir(log_dir).filePath(
      QStringLiteral("lv_%1.log").arg(QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd_hhmmss"))));

  std::vector<spdlog::sink_ptr> sinks;
  sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file.toStdString(), true));
  sinks.push_back(std::make_shared<QtListWidgetSink>(console));

  logger = std::make_shared<spdlog::logger>("lv", sinks.begin(), sinks.end());
  logger->set_level(spdlog::level::debug);
  logger->flush_on(spdlog::level::info);
  logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");
  spdlog::set_default_logger(logger);
}

}  // namespace

ccCustomQListWidget::ccCustomQListWidget(QWidget* parent) : QListWidget(parent) {}

void ccCustomQListWidget::keyPressEvent(QKeyEvent* event) {
  if (event->matches(QKeySequence::Copy)) {
    QStringList selected_lines;
    for (const auto* item : selectedItems()) {
      selected_lines.push_back(item->text());
    }
    QApplication::clipboard()->setText(selected_lines.join('\n'));
    return;
  }

  QListWidget::keyPressEvent(event);
}

ccConsole* ccConsole::Instance() {
  return g_console;
}

void ccConsole::Init(ccCustomQListWidget* text_display, QWidget* parent_widget) {
  if (g_console == nullptr) {
    g_console = new ccConsole(parent_widget);
  }

  g_console->text_display_ = text_display;
  g_console->parent_widget_ = parent_widget;
  EnsureDefaultLogger(g_console);
}

void ccConsole::Shutdown() {
  if (g_previous_qt_handler != nullptr) {
    qInstallMessageHandler(g_previous_qt_handler);
    g_previous_qt_handler = nullptr;
  }

  spdlog::shutdown();

  delete g_console;
  g_console = nullptr;
}

void ccConsole::RegisterQtMessageHandler() {
  if (g_previous_qt_handler == nullptr) {
    g_previous_qt_handler = qInstallMessageHandler(QtMessageBridge);
  }
}

void ccConsole::appendMessage(int level, const QString& message) {
  if (text_display_ == nullptr) {
    return;
  }

  auto* item = new QListWidgetItem(message, text_display_);
  item->setForeground(LevelColor(static_cast<spdlog::level::level_enum>(level)));
  text_display_->addItem(item);
  text_display_->scrollToBottom();
}

ccConsole::ccConsole(QObject* parent) : QObject(parent) {}