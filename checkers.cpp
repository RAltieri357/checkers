#include "checkers.h"
Piece* pieces[8][8];
int startx;
int starty;
int endx;
int endy;
bool jump = false;
int jumpoverx = 0;
int jumpovery = 0;
bool turn = true;





Checkers::Checkers(QWidget *parent)
    : QMainWindow(parent)
{
    resize(80*8,80*8); //make window
    for ( int x = 0; x < 8; x++)
    {
        for ( int y = 0 ; y < 8 ; y++)
        {
            if (x%2 == y%2)
            {
                if(y<=2){
                    pieces[x][y] = new Piece(false);
                }
                if(y>=5){
                    pieces[x][y] = new Piece(true);
                }
            }
        }
    }
}

Checkers::~Checkers()
{

}

Piece::Piece(bool color){
    isWhite = color;
    isKing = false;
}

Piece::~Piece()
{

}

void Checkers::paintEvent(QPaintEvent*)
{
    Piece p(this); //allows access to class Piece's data
    QPainter paint(this); //grabs the ability to manipulate the QPainter
    for( int x = 0; x < 8; x++)
    {
        for( int y = 0 ; y < 8 ; y++)
        {
            if (x%2 == y%2)
            {
                paint.fillRect(QRect(80*x,80*y,80,80),QColor(160,160,100));
            }
            else
                paint.fillRect(QRect(80*x,80*y,80,80),QColor(250,250,210));

        }
    }


    for(int x = 0 ; x < 8 ; x++){
        for (int y = 0 ; y < 8 ; y++ ){
            if (pieces[x][y] != NULL){
                pieces[x][y]->draw(&paint,x,y); //draw pieces on each repaint
            }
        }
    }

}

void Piece::draw(QPainter* paint, int x, int y)
{
    if(isWhite == true){
        paint->setBrush(QBrush(Qt::white));
        paint->drawEllipse(x*80+5,y*80+5,70,70);
    }
    else{
        paint->setBrush(QBrush(QColor(60,60,60)));
        paint->drawEllipse(x*80+5,y*80+5,70,70);
    }
    if(isKing == true){ //shows the difference between king and regular piece
        paint->setBrush(QBrush(QColor(255,215,0)));
        paint->drawEllipse(x*80+25,y*80+25,30,30);
    }
}
void Checkers::mousePressEvent(QMouseEvent* event)
{
    qDebug() << "Mouse was pressed at "<<event->x() / 80<<","<<event->y() / 80;
    if ((event->x() / 80) < 8 && (event->x() / 80) > -1){
        if ((event->y() / 80) < 8 && (event->y() / 80) > -1){
            startx = event->x() / 80;
            starty = event->y() / 80;
            qDebug() << "Confirmed the presses coordinates are sane: " << startx << "," << starty << endl;
        }
    }
    else{
        qDebug() << "The pressed coordinates are not sane: " << event->x() / 80 << "," << event->y() / 80 << endl;
    }


}

void Checkers::mouseReleaseEvent(QMouseEvent* event)
{
    Piece p(this);

    qDebug() << "Mouse was released at "<<event->x() / 80<<","<<event->y() / 80;
    if(turn == true){
        if ((event->x() / 80) < 8 && (event->x() / 80) > -1){
            if ((event->y() / 80) < 8 && (event->y() / 80) > -1){
                endx = event->x() / 80;
                endy = event->y() / 80;
                if (pieces[startx][starty] != NULL && pieces[endx][endy] == NULL)
                {
                    if (pieces[startx][starty]->isWhite==true){
                        qDebug() << "Confirmed the released coordinates are sane: " << endx << "," << endy << endl;
                        bool move = p.canMove(startx,starty,endx,endy);
                        if(move == true)
                        {

                            pieces[endx][endy] = pieces[startx][starty];
                            pieces[startx][starty] = NULL;
                            if (endy == 0)
                            {
                                pieces[endx][endy]->isKing = true;
                                qDebug() << "A white piece became a king!";

                            }
                            if (abs(startx-endx) == 2)
                            {
                                pieces[jumpoverx][jumpovery] = NULL;
                            }
                            repaint();
                            turn = !turn;
                            VictoryCheck();
                            p.randomAI();
                            repaint();
                            VictoryCheck();
                        }
                    }

                }

            }
        }
        else{
            qDebug() << "The released coordinates are not sane: " << event->x() / 80 << "," << event->y() / 80 << endl;
        }
    }

}


bool Piece::canMove(int startx, int starty, int endx, int endy)
{
    if (pieces[startx][starty]->isKing != true)  //checking the isKing variable, only time the rule of going one way can apply
    {
        if (pieces[startx][starty]->isWhite != true) //black piece
        {
            if ( endy < starty )  //if black piece is going other way
                return false;
        }
        else  //white piece
        {
            if ( endy > starty )  //if white piece is going other way
                return false;
        }
    }
    if( abs(startx - endx) > 2 && abs(starty - endy) > 2 )  //if piece is going more than 2 spaces, false
        return false;
    if ( abs(startx-endx) != abs(starty-endy) )  //if the move is not diagonal, false
        return false;
    if (abs(startx-endx) == 2) //if move is 2 spaces, there must be a jump
    {
        jumpoverx = startx+(endx-startx)/2;
        jumpovery = starty+(endy-starty)/2;
        if (pieces[jumpoverx][jumpovery] == NULL)
            return false;
        if (pieces[jumpoverx][jumpovery]->isWhite == pieces[startx][starty]->isWhite)
        {
            return false;
        }
    }
    return true;
}

void Piece::randomAI()
{
    bool move = false;
    if (turn == false){
        Piece u(this);
        while (move == false){ //while loop for making sure the outcome will be a valid move for the AI
            startx = rand()%8;
            starty = rand()%8;
            endx = rand()%8;
            endy = rand()%8;
            if (pieces[startx][starty] != NULL && pieces[endx][endy] == NULL)
            {
                if (pieces[startx][starty]->isWhite==false){
                    move = u.canMove(startx,starty,endx,endy);
                }
            }
        }
        pieces[endx][endy] = pieces[startx][starty];
        pieces[startx][starty] = NULL;
        if (endy == 7) //allows black pieces to get a king
        {
            pieces[endx][endy]->isKing = true;
            qDebug() << "A black piece became a king!";
        }
        if (abs(startx-endx) == 2) //jumping over another piece
        {
            pieces[jumpoverx][jumpovery] = NULL;
        }
        turn=!turn;
    }
}
void Checkers::VictoryCheck(){
    bool blacklost = true;
    bool whitelost = true;
    Piece p(this);
    for(int x = 0 ; x < 8 ; x++){
        for (int y = 0 ; y < 8 ; y++ ){
            for (int a = 0 ; a < 8 ; a++ ){
                for (int b = 0 ; b < 8 ; b++ ){
                    if (pieces[x][y] != NULL && pieces[a][b] == NULL){
                        if (p.canMove(x,y,a,b) == true){
                            if (pieces[x][y]->isWhite == false){
                                blacklost = false;
                            }
                            else{
                                whitelost = false;
                            }
                        }
                    }
                }
            }
        }
    }
    if (blacklost == true){
        turn = !turn;
        setWindowTitle("WHITE WINS");
    }
    if (whitelost == true){
        turn = !turn;
        setWindowTitle("BLACK WINS");
    }
}
