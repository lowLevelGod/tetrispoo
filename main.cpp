#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "headers/Game.hpp"
#include "headers/Board.hpp"
#include "headers/Piece.hpp"
#include "headers/Player.hpp"
#include "headers/Exception.hpp"

int main()
{
       auto& game = Game::getGame();
       try
       {
              game.run();
       }
       catch (const IOfailed &exc)
       {
              std::cerr << exc.what() << std::endl;
       }
       catch (const TooManyPowerups &exc)
       {
              std::cerr << exc.what() << std::endl;
       }
       return 0;
}