#pragma once
#include "Board.hpp"

class Player
{
    public:
        virtual void move(int, sf::RenderWindow&) = 0;
        Player(Board board, int pieceNo, int incr, int col, int color, int fallingspeed);
        virtual ~Player() = default;
    protected:
        Board board;
        int pieceNo;
        int incr;
        int col;
        int rotlen;
        int rot;
        Piece p;
        int color;
        int fallingspeed;
    friend class Game;
};

class Human : public Player
{   
    public:
        void move(int, sf::RenderWindow&) override;
        Human(Board board, int pieceNo, int incr, int col, int color, int fallingspeed);
        ~Human() = default;
};

class Robot : public Player
{   
    public:
        void move(int, sf::RenderWindow&) override;
        int bestMove(int incr, int col);
        Robot(Board board, int pieceNo, int incr, int col, int color, int fallingspeed);
        ~Robot() = default;
};