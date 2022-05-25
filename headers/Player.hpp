#pragma once
#include "Board.hpp"
#include "Score.hpp"

class Player
{
    public:
        virtual void move(int, sf::RenderWindow&) = 0;
        Player(const Board& board, int pieceNo, int incr, int col, int color, int fallingspeed, int currentScore, int pwrupcount = 0);
        virtual std::shared_ptr<Player> clone() = 0;
        virtual void reset() = 0;
        virtual ~Player() = default;
        Player& operator=(const Player&);
        Player(const Player&);
    protected:
        Board board;
        int pieceNo;
        int incr;
        int col;
        int color;
        int fallingspeed;
        int currentScore;
        int rotlen;
        int rot;
        std::shared_ptr<Piece> p;
        int pwrupcount;
    friend class Game;
};

class Human : public Player
{   
    public:
        void move(int, sf::RenderWindow&) override;
        void reset() override;
        Human(const Board& board, int pieceNo, int incr, int col, int color, int fallingspeed, int currentScore, int pwrupcount = 0);
        std::shared_ptr<Player> clone() override;
        Human(const Human&);
        Human& operator=(const Human&);
        ~Human() = default;
};

class Robot : public Player
{   
    private:
        int bestCol;
    public:
        void move(int, sf::RenderWindow&) override;
        void reset() override;
        int bestMove();
        void setBestCol(int bestCol) { this->bestCol = bestCol;}
        Robot(const Board& board, int pieceNo, int incr, int col, int color, int fallingspeed, int currentScore, int pwrupcount = 0, int bestCol = 0);
        std::shared_ptr<Player> clone() override;
        Robot(const Robot&);
        Robot& operator=(const Robot&);
        ~Robot() = default;
};