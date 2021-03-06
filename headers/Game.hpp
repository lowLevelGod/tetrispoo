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
       bool isReset;
       bool isQuit;
       std::vector<sf::Text> text;
       int tick(sf::Int32, sf::Int32&);
       void drawToWindow(const std::shared_ptr<Piece>&);
       void initRender(const sf::Font&);
       explicit Game(int screenWidth = 800, int screenHeight = 800, const std::string& wName = "Tetris");
public:
       Game(const Game&) = delete;
       Game& operator=(const Game&) = delete;
       static Game& getGame();
       void run();
       static int getfastfall();
       static int getslowfall();
       void setResetMode();
       void setQuitMode();
       static int getMaxpwrupcount();
       // static void initRender(const sf::Font&);
       friend std::ostream &operator<<(std::ostream &os, const Game &game);
};