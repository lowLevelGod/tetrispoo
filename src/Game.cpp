#include "../headers/Game.hpp"


Game::Game(int screenWidth, int screenHeight, const std::string& wName) : window{sf::VideoMode(screenWidth, screenHeight), wName}, board{Board()}, currentScore{0}, highScore{0}, fallingspeed(slowfall) {}

std::ostream &operator<<(std::ostream &os, const Game &game)
{
    std::cout << "Current board is: " << game.board << std::endl;
    std::cout << "Current score is: " << game.currentScore << std::endl;
    std::cout << "Current high score is: " << game.highScore << std::endl;
    return os;
}

void Game::run()
{
    srand(time(NULL));
    int pieceNo = rand() % NUM_PIECES;
    sf::Clock clock;
    this->window.setFramerateLimit(60);
    int incr = 0;
    int col = 0;
    int rotlen = this->board.getRotations()[pieceNo].size();
    int rot = rand() % rotlen;
    Piece p = this->board.getRotations()[pieceNo][rot];
    int color = rand() % (NUM_COLORS - 1) + 1;
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
                    if (col > 0 && !this->board.isFilled(incr, col - 1))
                        col -= 1;
                    break;
                case sf::Keyboard::Right:
                    if (col + p.getWidth() < GRID_WIDTH && !this->board.isFilled(incr, col + p.getWidth() + 1))
                        col += 1;
                    break;
                case sf::Keyboard::Up:
                    rot = (rot + 1) % rotlen;
                    p = this->board.getRotations()[pieceNo][rot];
                    while (col + p.getWidth() > GRID_WIDTH)
                        col -= 1;
                    break;
                case sf::Keyboard::Down:
                    fallingspeed = fastfall;
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }

        int status = this->board.movePieceDown(p, col, incr, color);
        this->board.drawGrid(this->window);
        while (clock.getElapsedTime().asSeconds() < fallingspeed)
            ;
        clock.restart();
        this->window.display();
        fallingspeed = slowfall;
        if (!status)
            this->board.undo();
        else
        {
            pieceNo = rand() % NUM_PIECES;
            rotlen = this->board.getRotations()[pieceNo].size();
            col = 0;
            incr = 0;
            rot = rand() % rotlen;
            p = this->board.getRotations()[pieceNo][rot];
            color = rand() % NUM_COLORS;
            this->board.clearRows();
            this->board.commit();
        }
    }
}
