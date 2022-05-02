#include "../headers/Player.hpp"
#include "../headers/Game.hpp"

Player::Player(Board board, int pieceNo, int incr, int col, int color, int fallingspeed) : board{board}, pieceNo{pieceNo}, incr{incr}, col{col}, color{color}, fallingspeed{fallingspeed}
{
    rotlen = static_cast<int>(this->board.getRotations()[pieceNo].size());
    rot = rand() % rotlen;
    p = this->board.getRotations()[pieceNo][rot];
}

Human::Human(Board board, int pieceNo, int incr, int col, int color, int fallingspeed) : Player(board, pieceNo, incr, col, color, fallingspeed) {}

void Human::move(int clockDiff, sf::RenderWindow &window)
{
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
                fallingspeed = Game::getfastfall();
                break;
            default:
                break;
            }
        default:
            break;
        }
    }
    if (clockDiff)
    {
        int status = this->board.movePieceDown(p, col, incr, color, (clockDiff ? clockDiff : 1) * fallingspeed);
        this->board.drawGrid(window);
        fallingspeed = Game::getslowfall();
        if (!status)
            this->board.undo();
        else
        {
            this->pieceNo = rand() % NUM_PIECES;
            this->rotlen = static_cast<int>(this->board.getRotations()[pieceNo].size());
            this->col = 0;
            this->incr = 0;
            this->rot = rand() % rotlen;
            this->p = this->board.getRotations()[pieceNo][rot];
            this->color = rand() % (NUM_COLORS - 1) + 1;
            this->board.clearRows();
            this->board.commit();
        }
    }
}

Robot::Robot(Board board, int pieceNo, int incr, int col, int color, int fallingspeed) : Player(board, pieceNo, incr, col, color, fallingspeed) {}

void Robot::move(int clockDiff, sf::RenderWindow &window)
{
    if (clockDiff)
    {
        col = bestMove(incr, col);
        int status = this->board.movePieceDown(p, col, incr, color, (clockDiff ? clockDiff : 1) * fallingspeed);
        this->board.drawGrid(window);
        fallingspeed = Game::getslowfall();
        if (!status)
            this->board.undo();
        else
        {
            this->pieceNo = rand() % NUM_PIECES;
            this->rotlen = static_cast<int>(this->board.getRotations()[pieceNo].size());
            this->col = 0;
            this->incr = 0;
            this->rot = rand() % rotlen;
            this->p = this->board.getRotations()[pieceNo][rot];
            this->color = rand() % (NUM_COLORS - 1) + 1;
            this->board.clearRows();
            this->board.commit();
        }
    }
}

int Robot::bestMove(int incr, int col)
{
    int nxtCol = rand() % 2 ? 1 : -1;
    if (board.isMoveValid(incr, col + nxtCol))
        return nxtCol;
    return 0;
}