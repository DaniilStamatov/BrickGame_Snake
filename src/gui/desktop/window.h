#pragma once

#include <QGridLayout>
#include <QKeyEvent>
#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include <QElapsedTimer>
#include <QWidget>

#ifdef TETRIS
#include "../../brick_game/tetris/tetris.h"
#elif defined(SNAKE)
#include <ctime>
#include "../../brick_game/snake/controller/controller.h"
#endif
class GameWidget;
class MainWindow : public QMainWindow {
    Q_OBJECT
    public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    private:
    GameWidget* m_game_win;
    QGridLayout* m_layout;
};

class GameWidget : public QWidget {
    Q_OBJECT
public:
    GameWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
private slots:
    void updateGame();
    void renderGame();
private:
    QColor m_colors[8];

    GameInfo_t m_gameInfo;
    int m_cellSize = 20;
    int m_fieldSizeX;
    int m_fieldSizeY;
    QTimer *m_timer;
    QElapsedTimer m_elapsedTimer;
};