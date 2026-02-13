#pragma once

#include <QApplication>
#include <QElapsedTimer>
#include <QGridLayout>
#include <QKeyEvent>
#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include <QWidget>
#include <QString>

#include "../../brick_game/common.h"

#ifdef TETRIS
#include "../../brick_game/tetris/tetris.h"
#elif defined(SNAKE)
#include <ctime>

#include "../../brick_game/snake/controller/controller.h"
#endif
#define WIDGET_HEIGHT 32
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define GAME_WIDGET_WIDTH 220
#define GAME_WIDGET_HEIGHT 420
#define CELL_SIZE 20
#define PAUSE_RECT_HEIGHT 32
#define GAME_OVER_RECT_HEIGHT 70
#define SCORE_RECT_HEIGHT 32
#define MESSAGE_RECT_HEIGHT 50

class GameWindow;
class InfoWindow;
class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

 private:
  GameWindow *game_window_;
  QGridLayout *layout_;
  InfoWindow *info_window_;
  QWidget *selector_widget_;

  // Initialize and show the game view (replace selector).
  // `which` - name of the game requested by the user ("snake" or "tetris").
  // If empty, default built-in game will be used. If auto_start is true,
  // simulate Enter to begin the game immediately.
  void ShowGameView(const QString &which = QString(), bool auto_start = false);

  // Remember which game user selected (for UI/diagnostics).
  QString selected_game_;
};

class GameWindow : public QWidget {
  Q_OBJECT
 public:
  GameWindow();
  GameInfo_t GetGameInfo();

 protected:
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
 private slots:
  void UpdateGame();
  void RenderGame();
 signals:
  void updateInfoText(GameInfo_t gameInfo);

 private:
  QColor colors_[8];

  GameInfo_t game_info_;
  int field_size_x_;
  int field_size_y_;
  QTimer *timer_;
  QElapsedTimer elapsed_timer_;
};

class InfoWindow : public QWidget {
  Q_OBJECT
 public:
  InfoWindow(GameInfo_t info);
  void Update(GameInfo_t gameInfo);

 protected:
  void paintEvent(QPaintEvent *event) override;

 private:
  GameInfo_t game_info_;
  QColor colors_[8];
};
