#pragma once
#include "Board.hpp"
#include "Player.hpp"
#include <string>
#include <SFML/Graphics.hpp>

class Game
{
private:
       sf::RenderWindow window;
       static const int fastfall = 2;
       static const int slowfall = 1;
       static const int timeDelay = 200;
       static const int maxpwrupcount = 5;
       inline static bool isReset;
       inline static bool isQuit;
       std::vector<sf::Text> text;
       int tick(sf::Int32, sf::Int32&);
       void drawToWindow(const std::shared_ptr<Piece>&);
       void initRender(const sf::Font&);
public:
       explicit Game(int screenWidth = 800, int screenHeight = 800, const std::string& wName = "Tetris");
       void run();
       static int getfastfall();
       static int getslowfall();
       static void setResetMode();
       static void setQuitMode();
       static int getMaxpwrupcount();
       // static void initRender(const sf::Font&);
       friend std::ostream &operator<<(std::ostream &os, const Game &game);
};