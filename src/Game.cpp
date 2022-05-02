#include "../headers/Game.hpp"
#include "../headers/Player.hpp"
#include <memory>


Game::Game(int screenWidth, int screenHeight, const std::string& wName) : window{sf::VideoMode(screenWidth, screenHeight), wName},currentScore{0}, highScore{0}{}

std::ostream &operator<<(std::ostream &os, const Game &game)
{
    //std::cout << "Current board is: " << game.board << std::endl;
    std::cout << "Current score is: " << game.currentScore << std::endl;
    std::cout << "Current high score is: " << game.highScore << std::endl;
    return os;
}

void Game::run()
{
    this->window.setFramerateLimit(60);
    sf::Clock clock;
    sf::Int32 lastClock = clock.getElapsedTime().asMilliseconds();

    srand(static_cast<unsigned int>(time(NULL)));
    std::shared_ptr<Player> human{new Human(Board{50}, rand() % NUM_PIECES, 0, 0, rand() % (NUM_COLORS - 1) + 1, Game::slowfall)};
    std::shared_ptr<Player> robot{new Robot(Board{50 + 400}, rand() % NUM_PIECES, 0, 0, rand() % (NUM_COLORS - 1) + 1, Game::slowfall)};
    while (this->window.isOpen())
    {
        int clockDiff = tick(clock.getElapsedTime().asMilliseconds(), lastClock);
        human->move(clockDiff, this->window);
        robot->move(clockDiff, this->window);
        if (clockDiff)
            window.display();
    }
}

int Game::tick(sf::Int32 currentClock, sf::Int32& lastClock)
{
    sf::Int32 elapsed = (currentClock - lastClock);
    if (elapsed > sf::milliseconds(Game::timeDelay).asMilliseconds())
    {
        lastClock = currentClock;
        return static_cast<int>(elapsed / sf::milliseconds(Game::timeDelay).asMilliseconds());
    }

    return 0;
}

int Game::getfastfall()
{
    return Game::fastfall;
}

int Game::getslowfall()
{
    return Game::slowfall;
}
