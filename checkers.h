#ifndef CHECKERS_H
#define CHECKERS_H

#include <QMainWindow>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <cstdlib>
#include <ctime>

class Checkers : public QMainWindow
{
    Q_OBJECT

public:
    Checkers(QWidget *parent = 0);
    ~Checkers();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void VictoryCheck();
};

class Piece
{
public:
    bool isWhite;
    bool isKing;
    void draw(QPainter* paint, int x, int y);
    Piece(bool);
    ~Piece();
    bool canMove(int startx, int starty, int endx, int endy);
    void randomAI();
};

#endif // CHECKERS_H
