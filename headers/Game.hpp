#pragma once
#include "Board.hpp"
#include "Player.hpp"
#include <string>
#include <SFML/Graphics.hpp>

class Game
{
private:
       sf::RenderWindow window;
       int currentScore;
       int highScore;
       static const int fastfall = 2;
       static const int slowfall = 1;
       static const int timeDelay = 200;
       int tick(sf::Int32, sf::Int32&);
public:
       explicit Game(int screenWidth = 800, int screenHeight = 800, const std::string& wName = "Tetris");
       void run();
       static int getfastfall();
       static int getslowfall();
       friend std::ostream &operator<<(std::ostream &os, const Game &game);
};