#pragma once
#include "Board.hpp"

class Player
{
    public:
        virtual void move(int, sf::RenderWindow&) = 0;
        Player(const Board& board, int pieceNo, int incr, int col, int color, int fallingspeed);
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
        Human(const Board& board, int pieceNo, int incr, int col, int color, int fallingspeed);
        ~Human() = default;
};

class Robot : public Player
{   
    public:
        void move(int, sf::RenderWindow&) override;
        int bestMove();
        Robot(const Board& board, int pieceNo, int incr, int col, int color, int fallingspeed);
        ~Robot() = default;
};