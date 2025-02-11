#include "window.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setStyleSheet("background: rgb(48, 50, 54);");
    QWidget* centralWidget = new QWidget();
    m_layout = new QGridLayout();
    m_gameWin = new GameWindow();
    m_infoWindow = new InfoWindow(m_gameWin->GetGameInfo());
    connect(m_gameWin, &GameWindow::updateInfoText, m_infoWindow,
          &InfoWindow::Update);
    m_layout->addWidget(m_gameWin, 0, 0);
    m_layout->addWidget(m_infoWindow, 0, 1);
    centralWidget->setLayout(m_layout);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{
    delete m_gameWin;
}

GameWindow::GameWindow(QWidget *parent)
{
    setFixedSize(GAME_WIDGET_WIDTH, GAME_WIDGET_HEIGHT);
    m_colors[0] = QColor(48, 50, 54);
    m_colors[1] = QColor(3, 196, 115);   
    m_colors[2] = QColor(0, 0, 255);   
    m_colors[3] = QColor(255, 255, 0); 
    m_colors[4] = QColor(255, 165, 0); 
    m_colors[5] = QColor(128, 0, 128); 
    m_colors[6] = QColor(0, 255, 255); 
    m_colors[7] = QColor(255, 192, 203);
    m_fieldSizeX = width() / m_cellSize;
    m_fieldSizeY = height() / m_cellSize;
    m_gameInfo.field = nullptr;
    m_gameInfo.next = nullptr;
    m_gameInfo.score = 0;
    m_gameInfo.high_score = 0;
    m_gameInfo.level = 0;
    m_gameInfo.speed = 0;
    m_gameInfo.pause = 0;
    m_timer = new QTimer();
    connect(m_timer, &QTimer::timeout, this, &GameWindow::UpdateGame);
    m_timer->start(16);
    m_elapsedTimer.start();
    setFocusPolicy(Qt::StrongFocus);
}

GameInfo_t GameWindow::GetGameInfo()
{
    return m_gameInfo;
}

void GameWindow::paintEvent(QPaintEvent *event)
{
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
    if(m_gameInfo.field == nullptr) {
        painter.setBrush(menuBrush);
        painter.drawRect(5, height() / 2 - 16, width() - 10, MESSAGE_RECT_HEIGHT);
        painter.setFont(QFont("Roboto", 13, 700));
        painter.drawText(QRect(5, height() / 2 - 16, width() - 10, MESSAGE_RECT_HEIGHT),
                     Qt::AlignCenter, "Press Enter to start\n new game");
    } else {
      for(int i = 0; i < HEIGHT; ++i) {
          for(int j = 0; j < WIDTH; ++j) {
              painter.setBrush(m_colors[m_gameInfo.field[i][j]]);
              painter.drawRect(radius + j * m_cellSize, radius + i * m_cellSize, m_cellSize, m_cellSize);
          }
      }
    }
    
    if(m_gameInfo.pause == PAUSE) {
        painter.setBrush(menuBrush);
        painter.drawRect(5, height() / 2 - 16, width() - 10, GAME_OVER_RECT_HEIGHT);
        painter.setFont(QFont("Roboto", 13, 700));
        painter.drawText(QRect(5, height() / 2 - 16, width() - 10, GAME_OVER_RECT_HEIGHT),
                     Qt::AlignCenter, "PAUSED\n Press Enter\n to continue");
    }

    if(m_gameInfo.pause == GAME_OVER) {
        painter.setBrush(menuBrush);
        painter.drawRect(5, height() / 2 - 16, width() - 10, GAME_OVER_RECT_HEIGHT);
        painter.setFont(QFont("Roboto", 13, 700));
        painter.drawText(QRect(5, height() / 2 - 16, width() - 10, GAME_OVER_RECT_HEIGHT),
                     Qt::AlignCenter, "GAME OVER\n Press Enter\n to start new game");
        QString scoreText = QString("Your score: %1").arg(m_gameInfo.score);
        painter.drawRect(5, height() / 2 + 60, width() - 10, 32);
        painter.drawText(QRect(5, height() / 2 + 60, width() - 10, SCORE_RECT_HEIGHT),
                     Qt::AlignCenter, scoreText);
    }
}

void GameWindow::keyPressEvent(QKeyEvent *event)
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

void GameWindow::RenderGame()
{
  if (m_gameInfo.pause == TERMINATE) {
     QApplication::quit();
  } else { 
    repaint();
    }
}

void GameWindow::UpdateGame()
{
    qint64 deltaTime = m_elapsedTimer.restart();
    m_gameInfo = updateCurrentState(deltaTime);
    emit updateInfoText(m_gameInfo);
    update();
}


InfoWindow::InfoWindow(GameInfo_t info) : m_gameInfo(info)
{
  setFixedSize(GAME_WIDGET_WIDTH, GAME_WIDGET_HEIGHT);
  m_colors[0] = QColor(48, 50, 54);
  m_colors[1] = QColor(3, 196, 115);   
  m_colors[2] = QColor(0, 0, 255);   
  m_colors[3] = QColor(255, 255, 0); 
  m_colors[4] = QColor(255, 165, 0); 
  m_colors[5] = QColor(128, 0, 128); 
  m_colors[6] = QColor(0, 255, 255); 
  m_colors[7] = QColor(255, 192, 203);
}

void InfoWindow::Update(GameInfo_t gameInfo)
{
   if (m_gameInfo.pause == TERMINATE) {
     QApplication::quit();
  }
  else {
    m_gameInfo = gameInfo;
    repaint();
  }
}

void InfoWindow::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event)
  QPainter painter(this);
  painter.drawRect(0, 0, width() - 1, height() - 1);
  painter.drawRect(0, 250, width() - 1, height() - 1);
  painter.setPen(QColor(250, 250, 250));
  painter.setFont(QFont("Roboto", 12, 700));
  painter.drawText(QRect(10, 10, width() - 10, 20), Qt::AlignLeft,
                   "HIGH SCORE: " + QString::number(m_gameInfo.high_score));
  painter.drawText(QRect(10, 50, width() - 10, 20), Qt::AlignLeft,
                   "SCORE: " + QString::number(m_gameInfo.score));
  painter.drawText(QRect(10, 90, width() - 10, 20), Qt::AlignLeft,
                   "LEVEL: " + QString::number(m_gameInfo.level));

  if(m_gameInfo.next && m_gameInfo.pause != TERMINATE) {
    for(int i = 0; i < 4; ++i) {
      for(int j = 0; j < 4; ++j) {
          painter.setBrush(m_colors[m_gameInfo.next[i][j]]);
          painter.drawRect(60 + j * CELL_SIZE,
                         150 + i * CELL_SIZE, CELL_SIZE,
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