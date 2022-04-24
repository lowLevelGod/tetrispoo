#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "Piece.hpp"
#include "Board.hpp"
#include "Game.hpp"

int main()
{
       Game game(1200, 1200);
       game.run();
       return 0;
}