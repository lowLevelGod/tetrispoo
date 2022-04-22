#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <iostream>
#include "Piece.hpp"
#include "Board.hpp"

using namespace std::string_literals;

class Game
{
private:
       sf::RenderWindow window;
       Board board;
       int currentScore;
       int highScore;

public:
       explicit Game(int screenWidth = 800, int screenHeight = 800) : window{sf::VideoMode(screenWidth, screenHeight), "My window"s}, board{Board()}, currentScore{0}, highScore{0} {}
       const Board &getBoard() const { return board; }
       void run();
       friend std::ostream &operator<<(std::ostream &os, const Game &game);
};

void Game::run()
{
       Piece dog(this->board.getRotations()[THET][0]);
       sf::Clock clock;
       this->window.setFramerateLimit(60);
       int i = 0;
       int col = BOARD_START;
       int rot = 0;
       while (window.isOpen())
       {
              // check all the window's events that were triggered since the last iteration of the loop
              sf::Event event;
              while (window.pollEvent(event))
              {
                     // "close requested" event: we close the window
                     switch (event.type)
                     {
                     case sf::Event::Closed:
                            window.close();
                            break;
                     case sf::Event::KeyPressed:
                            switch (event.key.code)
                            {
                            case sf::Keyboard::Left:
                                   if (col > 1)
                                          col -= 1;
                                   break;
                            case sf::Keyboard::Right:
                                   if (col + dog.getWidth() <= GRID_WIDTH + 1)
                                          col += 1;
                                   break;
                            case sf::Keyboard::Up:
                                   rot = (rot + 1) % 4;
                                   dog = this->board.getRotations()[THET][rot];
                            default:
                                   break;
                            }
                     default:
                            break;
                     }
              }

              this->board.place(dog, BOARD_START + i * dog.getHeight(), col);
              this->board.drawGrid(this->window);
              while (clock.getElapsedTime().asSeconds() < 1.0f)
                     ;
              clock.restart();
              this->window.display();
              this->board.undo();
              std::cout << this->board.dropHeight(dog, BOARD_START) << std::endl;
              std::cout << BOARD_START + i * dog.getHeight() << std::endl;
              if (this->board.dropHeight(dog, BOARD_START) > BOARD_START + (i + 2) * dog.getHeight())
                     ++i;
              if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && col > BOARD_START)
                     col -= 1;
              else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && col + dog.getWidth() < GRID_WIDTH - 1)
                     col += 1;
       }
}

std::ostream &operator<<(std::ostream &os, const Game &game)
{
       std::cout << "Current board is: " << game.board << std::endl;
       std::cout << "Current score is: " << game.currentScore << std::endl;
       std::cout << "Current high score is: " << game.highScore << std::endl;
       return os;
}

int main()
{
       Game game(1200, 1200);
       game.run();
       // std::cout << game;
       //  run the program as long as the window is open

       return 0;
}