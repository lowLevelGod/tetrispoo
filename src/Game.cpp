#include "../headers/Game.hpp"
#include "../headers/Player.hpp"
#include "../headers/Exception.hpp"
#include <memory>

Game::Game(int screenWidth, int screenHeight, const std::string &wName) : window{sf::VideoMode(screenWidth, screenHeight), wName}
{
    Game::isReset = false;
    Game::isQuit = false;
}

Game& Game::getGame()
{
    static Game game(1200, 1200);
    return game;
}

std::ostream &operator<<(std::ostream &os, const Game &game)
{
    // std::cout << "Current board is: " << game.board << std::endl;
    // std::cout << "Current score is: " << game.currentScore << std::endl;
    // std::cout << "Current high score is: " << game.highScore << std::endl;
    std::cout << game.fastfall << std::endl;
    return os;
}

void Game::run()
{
    this->window.setFramerateLimit(60);
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        std::string arg = "Failed to load font !";
        throw IOfailed(arg);
    }
    initRender(font);

    sf::Clock clock;
    sf::Int32 lastClock = clock.getElapsedTime().asMilliseconds();

    srand(static_cast<unsigned int>(time(NULL)));
    std::shared_ptr<Player> human{new Human(Board{70, 50}, rand() % NUM_PIECES, 0, 0, rand() % (NUM_COLORS - 1) + 1, Game::slowfall, Score<int>(0))};
    std::shared_ptr<Player> robot{new Robot(Board{70, 50 + 700}, rand() % NUM_PIECES, 0, 0, rand() % (NUM_COLORS - 1) + 1, Game::slowfall,Score<float>(0.f))};
    while (this->window.isOpen())
    {
        int clockDiff = tick(clock.getElapsedTime().asMilliseconds(), lastClock);
        human->move(clockDiff, this->window);

        sf::Text tex;
        tex.setFont(font);
        tex.setFillColor(sf::Color::Blue);
        tex.setPosition(sf::Vector2f((410.f), (300.f)));
        tex.setString("Current score : " + std::to_string(std::dynamic_pointer_cast<Human>(human)->getCurrentScore().getScore()));
        text[text.size() - 1] = tex;

        if (Game::isQuit)
            return;
        if (Game::isReset)
        {
            human->reset();
            robot->reset();
            Game::isReset = false;
            continue;
        }
        robot->move(clockDiff, this->window);
        if (clockDiff)
        {
            this->drawToWindow(human->p->clone());
            window.display();
        }
    }
}

int Game::tick(sf::Int32 currentClock, sf::Int32 &lastClock)
{
    sf::Int32 elapsed = (currentClock - lastClock);
    if (elapsed > sf::milliseconds(Game::timeDelay).asMilliseconds())
    {
        lastClock = currentClock;
        return static_cast<int>(elapsed / sf::milliseconds(Game::timeDelay).asMilliseconds());
    }

    return 0;
}

int Game::getMaxpwrupcount()
{
    return Game::maxpwrupcount;
}

int Game::getfastfall()
{
    return Game::fastfall;
}

int Game::getslowfall()
{
    return Game::slowfall;
}

void Game::setResetMode()
{
    Game::isReset = true;
}

void Game::setQuitMode()
{
    Game::isQuit = true;
}

void Game::initRender(const sf::Font &font)
{
    sf::Text tex;
    tex.setFont(font);
    tex.setFillColor(sf::Color::Green);
    tex.setPosition(sf::Vector2f((200.f), (20.f)));
    tex.setString("YOU");

    text.push_back(tex);

    tex.setFillColor(sf::Color::Red);
    tex.setPosition(sf::Vector2f((900.f), (20.f)));
    tex.setString("BOT");

    text.push_back(tex);

    tex.setFillColor(sf::Color::Yellow);
    tex.setPosition(sf::Vector2f(static_cast<float>((16 - 2) * BLOCK_SIZE), static_cast<float>((16 - 4) * BLOCK_SIZE)));
    tex.setString("CURRENT PIECE");

    text.push_back(tex);

    tex.setFillColor(sf::Color::Blue);
    tex.setPosition(sf::Vector2f((410.f), (300.f)));
    tex.setString("Current score : 0");
    text.push_back(tex);
}

void Game::drawToWindow(const std::shared_ptr<Piece> &holdp)
{

    // clear currentScore
    for (int x = 410; x <= 650; ++x)
    {
        sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
        block.setFillColor(sf::Color::Black);
        block.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(300)));
        window.draw(block);
    }
    // prepare area for current piece
    int xc = 16, yc = 16;
    for (int col = -4; col < 4; ++col)
        for (int row = -4; row < 4; ++row)
        {
            sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE - 10, BLOCK_SIZE - 10));
            block.setFillColor(sf::Color::Black);
            block.setPosition(sf::Vector2f(static_cast<float>((col + xc) * BLOCK_SIZE + 5), static_cast<float>((row + yc) * BLOCK_SIZE + 5)));
            window.draw(block);
        }
    // draw piece for current piece
    for (auto elem : holdp->getBody())
    {
        sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE - 10, BLOCK_SIZE - 10));
        block.setFillColor(sf::Color::Green);
        block.setPosition(sf::Vector2f(static_cast<float>((elem.second + xc) * BLOCK_SIZE + 5), static_cast<float>((elem.first + yc) * BLOCK_SIZE + 5)));
        window.draw(block);
    }
    for (auto elem : text)
        this->window.draw(elem);
}
