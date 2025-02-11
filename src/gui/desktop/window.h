#pragma once

#include <QGridLayout>
#include <QKeyEvent>
#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include <QElapsedTimer>
#include <QWidget>
#include <QApplication>

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
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    private:
    GameWindow* m_gameWin;
    QGridLayout* m_layout;
    InfoWindow* m_infoWindow;
};

class GameWindow : public QWidget {
    Q_OBJECT
public:
    const static int m_cellSize = 20;
    GameWindow(QWidget *parent = nullptr);
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
    QColor m_colors[8];

    GameInfo_t m_gameInfo;
    int m_fieldSizeX;
    int m_fieldSizeY;
    QTimer *m_timer;
    QElapsedTimer m_elapsedTimer;
};

class InfoWindow : public QWidget {
    Q_OBJECT
public:
    InfoWindow(GameInfo_t info);
    void Update(GameInfo_t gameInfo);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    GameInfo_t m_gameInfo;
    QColor m_colors[8];
};
