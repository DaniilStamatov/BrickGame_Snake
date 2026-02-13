#include "window.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QProcess>
#include <QStringList>
#include <QKeyEvent>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  setStyleSheet("background: rgb(48, 50, 54);");

  selector_widget_ = new QWidget();
  QVBoxLayout *selLayout = new QVBoxLayout(selector_widget_);
  selLayout->addStretch();

  QPushButton *snakeBtn = new QPushButton("Start");
  snakeBtn->setFixedHeight(48);
  snakeBtn->setMinimumWidth(200);
  QPushButton *quitBtn = new QPushButton("Quit");
  quitBtn->setFixedHeight(36);

  selLayout->addWidget(snakeBtn, 0, Qt::AlignHCenter);
  selLayout->addWidget(quitBtn, 0, Qt::AlignHCenter);
  selLayout->addStretch();

  setCentralWidget(selector_widget_);

  QObject::connect(snakeBtn, &QPushButton::clicked, [this]() {
    selected_game_ = "snake";
    ShowGameView(selected_game_, true);
  });


  QObject::connect(quitBtn, &QPushButton::clicked, qApp, &QApplication::quit);
}

MainWindow::~MainWindow() { delete game_window_; }

GameWindow::GameWindow() {
  setFixedSize(GAME_WIDGET_WIDTH, GAME_WIDGET_HEIGHT);
  colors_[0] = QColor(48, 50, 54);
  colors_[1] = QColor(3, 196, 115);
  colors_[2] = QColor(0, 0, 255);
  colors_[3] = QColor(255, 255, 0);
  colors_[4] = QColor(255, 165, 0);
  colors_[5] = QColor(128, 0, 128);
  colors_[6] = QColor(0, 255, 255);
  colors_[7] = QColor(255, 192, 203);
  field_size_x_ = width() / CELL_SIZE;
  field_size_y_ = height() / CELL_SIZE;
  game_info_.field = nullptr;
  game_info_.next = nullptr;
  game_info_.score = 0;
  game_info_.high_score = 0;
  game_info_.level = 0;
  game_info_.speed = 0;
  game_info_.pause = 0;
  timer_ = new QTimer();
  connect(timer_, &QTimer::timeout, this, &GameWindow::UpdateGame);
  timer_->start(16);
  elapsed_timer_.start();
  setFocusPolicy(Qt::StrongFocus);
}

GameInfo_t GameWindow::GetGameInfo() { return game_info_; }

void GameWindow::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event)
  QPainter painter(this);
  QBrush menuBrush(QColor(10, 10, 10));
  QPen pen(QColor(128, 128, 128));
  pen.setWidth(2);
  painter.setPen(pen);
  int radius = 10;
  painter.drawRoundedRect(0, 0, width(), height(), radius, radius);
  QPen pen_field(QColor(139, 144, 163));
  painter.setPen(pen_field);
  if (game_info_.field == nullptr) {
    painter.setBrush(menuBrush);
    painter.drawRect(5, height() / 2 - 16, width() - 10, MESSAGE_RECT_HEIGHT);
    painter.setFont(QFont("Roboto", 13, 700));
    painter.drawText(
        QRect(5, height() / 2 - 16, width() - 10, MESSAGE_RECT_HEIGHT),
        Qt::AlignCenter, "Press Enter to start\n new game");
  } else {
    for (int i = 0; i < HEIGHT; ++i) {
      for (int j = 0; j < WIDTH; ++j) {
        painter.setBrush(colors_[game_info_.field[i][j]]);
        painter.drawRect(radius + j * CELL_SIZE, radius + i * CELL_SIZE,
                         CELL_SIZE, CELL_SIZE);
      }
    }
  }

  if (game_info_.pause == PAUSE) {
    painter.setBrush(menuBrush);
    painter.drawRect(5, height() / 2 - 16, width() - 10, GAME_OVER_RECT_HEIGHT);
    painter.setFont(QFont("Roboto", 13, 700));
    painter.drawText(
        QRect(5, height() / 2 - 16, width() - 10, GAME_OVER_RECT_HEIGHT),
        Qt::AlignCenter, "PAUSED\n Press Enter\n to continue");
  }

  if (game_info_.pause == GAME_OVER) {
    painter.setBrush(menuBrush);
    painter.drawRect(5, height() / 2 - 16, width() - 10, GAME_OVER_RECT_HEIGHT);
    painter.setFont(QFont("Roboto", 13, 700));
    painter.drawText(
        QRect(5, height() / 2 - 16, width() - 10, GAME_OVER_RECT_HEIGHT),
        Qt::AlignCenter, "GAME OVER\n Press Enter\n to start new game");
    QString scoreText = QString("Your score: %1").arg(game_info_.score);
    painter.drawRect(5, height() / 2 + 60, width() - 10, 32);
    painter.drawText(
        QRect(5, height() / 2 + 60, width() - 10, SCORE_RECT_HEIGHT),
        Qt::AlignCenter, scoreText);
  }
}

void GameWindow::keyPressEvent(QKeyEvent *event) {
  UserAction_t action = UserAction_t::Up;

  if (event->key() == Qt::Key_Return) {
    action = UserAction_t::Start;
  }
  if (event->key() == Qt::Key_Space) {
    action = UserAction_t::Pause;
  }
  if (event->key() == Qt::Key_Escape) {
    action = UserAction_t::Terminate;
  }
  if (event->key() == Qt::Key_Left) {
    action = UserAction_t::Left;
  }
  if (event->key() == Qt::Key_Right) {
    action = UserAction_t::Right;
  }
  if (event->key() == Qt::Key_Up) {
    action = UserAction_t::Up;
  }
  if (event->key() == Qt::Key_Down) {
    action = UserAction_t::Down;
  }
  if (event->key() == Qt::Key_R) {
    action = UserAction_t::Action;
  }
  userInput(action, false);
}

void GameWindow::RenderGame() {
  if (game_info_.pause == TERMINATE) {
    QApplication::quit();
  } else {
    repaint();
  }
}

void GameWindow::UpdateGame() {
  qint64 deltaTime = elapsed_timer_.restart();
  game_info_ = updateCurrentState(deltaTime);
  emit updateInfoText(game_info_);
  update();
}

InfoWindow::InfoWindow(GameInfo_t info) : game_info_(info) {
  setFixedSize(GAME_WIDGET_WIDTH, GAME_WIDGET_HEIGHT);
  colors_[0] = QColor(48, 50, 54);
  colors_[1] = QColor(3, 196, 115);
  colors_[2] = QColor(0, 0, 255);
  colors_[3] = QColor(255, 255, 0);
  colors_[4] = QColor(255, 165, 0);
  colors_[5] = QColor(128, 0, 128);
  colors_[6] = QColor(0, 255, 255);
  colors_[7] = QColor(255, 192, 203);
}

void InfoWindow::Update(GameInfo_t gameInfo) {
  if (game_info_.pause == TERMINATE) {
    QApplication::quit();
  } else {
    game_info_ = gameInfo;
    repaint();
  }
}

void InfoWindow::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event)
  QPainter painter(this);
  painter.drawRect(0, 0, width() - 1, height() - 1);
  painter.drawRect(0, 250, width() - 1, height() - 1);
  painter.setPen(QColor(250, 250, 250));
  painter.setFont(QFont("Roboto", 12, 700));
  painter.drawText(QRect(10, 10, width() - 10, 20), Qt::AlignLeft,
                   "HIGH SCORE: " + QString::number(game_info_.high_score));
  painter.drawText(QRect(10, 50, width() - 10, 20), Qt::AlignLeft,
                   "SCORE: " + QString::number(game_info_.score));
  painter.drawText(QRect(10, 90, width() - 10, 20), Qt::AlignLeft,
                   "LEVEL: " + QString::number(game_info_.level));
  if (game_info_.next && game_info_.pause != TERMINATE) {
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        painter.setBrush(colors_[game_info_.next[i][j]]);
        painter.drawRect(60 + j * CELL_SIZE, 150 + i * CELL_SIZE, CELL_SIZE,
                         CELL_SIZE);
      }
    }
  }

  painter.drawText(QRect(10, 260, width() - 10, 20), Qt::AlignLeft,
                   "Controls:");
  painter.drawText(QRect(10, 280, width() - 10, 20), Qt::AlignLeft,
                   "Enter - Start Game");
  painter.drawText(QRect(10, 300, width() - 10, 20), Qt::AlignLeft,
                   "Space - Pause");
  painter.drawText(QRect(10, 320, width() - 10, 20), Qt::AlignLeft,
                   "Escape - Exit");
  painter.drawText(QRect(10, 360, width() - 10, 20), Qt::AlignLeft,
                   "Arrow Keys - Move");
}

void MainWindow::ShowGameView(const QString &which, bool auto_start) {
  selected_game_ = which;
  
  QWidget *newCentral = new QWidget();
  layout_ = new QGridLayout();
  game_window_ = new GameWindow();
  info_window_ = new InfoWindow(game_window_->GetGameInfo());
  
  connect(game_window_, &GameWindow::updateInfoText, info_window_,
          &InfoWindow::Update);
          
  layout_->addWidget(game_window_, 0, 0);
  layout_->addWidget(info_window_, 0, 1);
  newCentral->setLayout(layout_);
  
  QWidget *old = centralWidget();
  setCentralWidget(newCentral);
  if (old) {
    old->deleteLater();
    selector_widget_ = nullptr;
  }
  
  if (game_window_) {
    game_window_->setFocus(Qt::ActiveWindowFocusReason);
    game_window_->grabKeyboard();
  }

  if (auto_start) {
    QKeyEvent *ev = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
    QCoreApplication::postEvent(game_window_, ev);
  }
}