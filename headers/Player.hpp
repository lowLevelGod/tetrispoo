#pragma once
#include "Board.hpp"

class Player
{
    public:
        virtual void move(int, sf::RenderWindow&) = 0;
        Player(const Board& board, int pieceNo, int incr, int col, int color, int fallingspeed);
        virtual std::shared_ptr<Player> clone() = 0;
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
        std::shared_ptr<Player> clone() override;
        ~Human() = default;
};

class Robot : public Player
{   
    public:
        void move(int, sf::RenderWindow&) override;
        int bestMove();
        Robot(const Board& board, int pieceNo, int incr, int col, int color, int fallingspeed);
        std::shared_ptr<Player> clone() override;
        ~Robot() = default;
};