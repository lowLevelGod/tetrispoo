#include "../headers/Game.hpp"
#include "../headers/Player.hpp"
#include <memory>


Game::Game(int screenWidth, int screenHeight, const std::string& wName) : window{sf::VideoMode(screenWidth, screenHeight), wName},currentScore{0}, highScore{0}{
    Game::isReset = false;
    Game::isQuit = false;
}

std::ostream &operator<<(std::ostream &os, const Game &game)
{
    //std::cout << "Current board is: " << game.board << std::endl;
    std::cout << "Current score is: " << game.currentScore << std::endl;
    std::cout << "Current high score is: " << game.highScore << std::endl;
    return os;
}

void Game::run()
{
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        throw _EXCEPTION_PTR_H;
    }
    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::Green);
    text.setPosition(sf::Vector2f(static_cast<float>(200), static_cast<float>(20)));
    text.setString("YOU");

    sf::Text text2;
    text2.setFont(font);
    text2.setFillColor(sf::Color::Red);
    text2.setPosition(sf::Vector2f(static_cast<float>(900), static_cast<float>(20)));
    text2.setString("BOT");

    this->window.setFramerateLimit(60);
    sf::Clock clock;
    sf::Int32 lastClock = clock.getElapsedTime().asMilliseconds();

    srand(static_cast<unsigned int>(time(NULL)));
    std::shared_ptr<Player> human{new Human(Board{70, 50}, rand() % NUM_PIECES, 0, 0, rand() % (NUM_COLORS - 1) + 1, Game::slowfall)};
    std::shared_ptr<Player> robot{new Robot(Board{70, 50 + 700}, rand() % NUM_PIECES, 0, 0, rand() % (NUM_COLORS - 1) + 1, Game::slowfall)};
    while (this->window.isOpen())
    {
        int clockDiff = tick(clock.getElapsedTime().asMilliseconds(), lastClock);
        human->move(clockDiff, this->window);
        if (Game::isQuit)
            return;
        if (Game::isReset)
        {
            Game::reset(human, robot);
            continue;
        }
        robot->move(clockDiff, this->window);
        if (clockDiff)
        {
            std::shared_ptr<Piece> holdp = human->p.clone();

            int xc = 16, yc = 16;
            for (int col = -4; col < 4; ++col)
                for (int row = -4; row < 4; ++row)
            {
                sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE - 10, BLOCK_SIZE - 10));
                block.setFillColor(sf::Color::Black);
                block.setPosition(sf::Vector2f(static_cast<float>((col + xc) * BLOCK_SIZE + 5), static_cast<float>((row + yc) * BLOCK_SIZE + 5)));
                window.draw(block);
            }

            sf::Text text3;
            text3.setFont(font);
            text3.setFillColor(sf::Color::Yellow);
            text3.setPosition(sf::Vector2f(static_cast<float>((xc - 2) * BLOCK_SIZE), static_cast<float>((xc - 4) * BLOCK_SIZE)));
            text3.setString("HOLD");

            for (auto elem : holdp->getBody())
            {
                sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE - 10, BLOCK_SIZE - 10));
                block.setFillColor(sf::Color::Green);
                block.setPosition(sf::Vector2f(static_cast<float>((elem.first + xc) * BLOCK_SIZE + 5), static_cast<float>((elem.second + yc) * BLOCK_SIZE + 5)));
                window.draw(block);
            }

            this->window.draw(text);
            this->window.draw(text2);
            this->window.draw(text3);
            window.display();
        }
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

void Game::reset(std::shared_ptr<Player>& human, std::shared_ptr<Player>& robot)
{
    std::shared_ptr<Player> humantemp = std::make_shared<Human>(Human(Board{70, 50}, rand() % NUM_PIECES, 0, 0, rand() % (NUM_COLORS - 1) + 1, Game::slowfall));
    std::shared_ptr<Player> robottemp = std::make_shared<Robot>(Robot(Board{70, 50 + 700}, rand() % NUM_PIECES, 0, 0, rand() % (NUM_COLORS - 1) + 1, Game::slowfall));

    human = humantemp->clone();
    robot = robottemp->clone();
    Game::isReset = false;
}

void Game::setResetMode()
{
    Game::isReset = true;
}

void Game::setQuitMode()
{
    Game::isQuit = true;
}
