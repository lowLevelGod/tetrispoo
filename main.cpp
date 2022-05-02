#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "Game.hpp"
#include "Board.hpp"
#include "Piece.hpp"
#include "Player.hpp"

int main()
{
       Game game(1200, 1200);
       game.run();
       return 0;
}