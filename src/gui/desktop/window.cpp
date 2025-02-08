#include "window.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setFixedSize(800, 800);
    setStyleSheet("background: rgb(48, 50, 54);");
    QWidget* centralWidget = new QWidget();
    m_layout = new QGridLayout();
    m_game_win = new GameWidget();
    m_layout->addWidget(m_game_win, 0, 0);
    centralWidget->setLayout(m_layout);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{
    delete m_game_win;
}

GameWidget::GameWidget(QWidget *parent)
{
    setFixedSize(200, 400);
    m_colors[0] = QColor(0, 0, 0);
    m_colors[1] = QColor(0, 255, 0);   // Зеленый
    m_colors[2] = QColor(0, 0, 255);   // Синий
    m_colors[3] = QColor(255, 255, 0); // Желтый
    m_colors[4] = QColor(255, 165, 0); // Оранжевый
    m_colors[5] = QColor(128, 0, 128); // Пурпурный
    m_colors[6] = QColor(0, 255, 255); // Голубой
    m_colors[7] = QColor(255, 192, 203); // Розовый
    m_fieldSizeX = width() / m_cellSize;
    m_fieldSizeY = height() / m_cellSize;
    m_timer = new QTimer();
    connect(m_timer, &QTimer::timeout, this, &GameWidget::updateGame);
    m_timer->start(16);
    m_elapsedTimer.start();
    setFocusPolicy(Qt::StrongFocus);
}

void GameWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    QBrush menuBrush(QColor(255, 255, 255));
    painter.setPen(QColor(128, 128, 128));
    painter.drawRect(0, 0, width(), height());
    if(m_gameInfo.field == nullptr) {
        painter.setBrush(menuBrush);
        painter.drawRect(5, height() / 2 - 16, width() - 10, 40);
        painter.setFont(QFont("Roboto", 13, 700));
        painter.drawText(QRect(5, height() / 2 - 16, width() - 10, 40),
                     Qt::AlignCenter, "Press Enter to start\n new game");
    }
    if(m_gameInfo.field != nullptr) {
        for(int i = 0; i < HEIGHT; ++i) {
          for(int j = 0; j < WIDTH; ++j) {
              painter.setBrush(m_colors[m_gameInfo.field[i][j]]);
              painter.drawRect(j * m_cellSize, i * m_cellSize, m_cellSize, m_cellSize);
          }
      }
    }
    
    if(m_gameInfo.pause == PAUSE) {
        painter.setBrush(menuBrush);
        painter.drawRect(5, height() / 2 - 16, width() - 10, 32);
        painter.setFont(QFont("Roboto", 13, 700));
        painter.drawText(QRect(5, height() / 2 - 16, width() - 10, 32),
                     Qt::AlignCenter, "PAUSED Press Enter to unpause");
    }

    if(m_gameInfo.pause == GAME_OVER) {
        painter.setBrush(menuBrush);
        painter.drawRect(5, height() / 2 - 16, width() - 10, 60);
        painter.setFont(QFont("Roboto", 13, 700));
        painter.drawText(QRect(5, height() / 2 - 16, width() - 10, 70),
                     Qt::AlignCenter, "GAME_OVER\n Press Enter\n to start new game");
        QString scoreText = QString("Your score: %1").arg(m_gameInfo.score);
        painter.drawRect(5, height() / 2 + 40, width() - 10, 32);
        painter.drawText(QRect(5, height() / 2 + 40, width() - 10, 32),
                     Qt::AlignCenter, scoreText);
    }
}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
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

void GameWidget::renderGame()
{
  if (m_gameInfo.pause == TERMINATE) {
    exit(0);
  }
  repaint();
}

void GameWidget::updateGame()
{
    qint64 deltaTime = m_elapsedTimer.restart();
    m_gameInfo = updateCurrentState(deltaTime);
    
    update();
}
