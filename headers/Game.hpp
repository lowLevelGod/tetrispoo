#pragma once
#include "Board.hpp"
#include <string>
#include <SFML/Graphics.hpp>

class Game
{
private:
       sf::RenderWindow window;
       Board board;
       int currentScore;
       int highScore;
       const float fastfall = 0.1f;
       const float slowfall = 0.3f;
       float fallingspeed;

public:
       explicit Game(int screenWidth = 800, int screenHeight = 800, const std::string& wName = "Tetris");
       const Board &getBoard() const { return board; }
       void run();
       friend std::ostream &operator<<(std::ostream &os, const Game &game);
};