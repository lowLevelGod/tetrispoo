#include "../headers/Player.hpp"
#include "../headers/Game.hpp"

Player::Player(const Board& board, int pieceNo, int incr, int col, int color, int fallingspeed, int currentScore) : board{board}, pieceNo{pieceNo}, incr{incr}, col{col}, color{color}, fallingspeed{fallingspeed}, currentScore{currentScore}
{
    rotlen = static_cast<int>(this->board.getRotations()[pieceNo].size());
    rot = rand() % rotlen;
    p = std::make_shared<Piece>(this->board.getRotations()[pieceNo][rot]);
}

Human::Human(const Board& board, int pieceNo, int incr, int col, int color, int fallingspeed, int currentScore) : Player(board, pieceNo, incr, col, color, fallingspeed, currentScore) {}

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
                if (col + p->getWidth() < GRID_WIDTH && !this->board.isFilled(incr, col + p->getWidth() + 1))
                    col += 1;
                break;
            case sf::Keyboard::Up:
                rot = (rot + 1) % rotlen;
                p = std::make_shared<Piece>(this->board.getRotations()[pieceNo][rot]);
                while (col + p->getWidth() > GRID_WIDTH)
                    col -= 1;
                break;
            case sf::Keyboard::Down:
                fallingspeed = Game::getfastfall();
                break;
            case sf::Keyboard::R:
                Game::setResetMode();
                return;
            case sf::Keyboard::Q:
                Game::setQuitMode();
                return;
            default:
                break;
            }
        default:
            break;
        }
    }
    if (clockDiff)
    {
        int status = this->board.movePieceDown(*p, col, incr, color, clockDiff * fallingspeed);
        this->board.drawGrid(window);
        fallingspeed = Game::getslowfall();
        if (!status)
            this->board.undo();
        else
        {
            std::shared_ptr<Powerup> castedp = std::dynamic_pointer_cast<Powerup>(this->p);
            if (castedp)
            {
                std::cout << "Powerup" << std::endl;
                int score = castedp->computeScore(this->color);   
                //destroy last 2 rows
                int blocksDestroyed = 0;
                Piece oneblock = Piece({std::make_pair<int,int>(0, 0)});
                for (int row = GRID_HEIGHT - 2; row <= GRID_HEIGHT - 1; ++row)
                    for (int col = 0; col < GRID_WIDTH; ++col)
                    {
                        if (this->board.isFilled(row, col))
                            ++blocksDestroyed;
                        this->board.place(oneblock, row, col, 1);
                        this->board.commit();
                    }
                this->currentScore += score * blocksDestroyed;
            }

            this->pieceNo = rand() % NUM_PIECES;
            this->rotlen = static_cast<int>(this->board.getRotations()[pieceNo].size());
            this->col = 0;
            this->incr = 0;
            this->rot = rand() % rotlen;
            //Next piece can be powerup or normal
            int pwrup = rand() % 2;
            if (pwrup)
                this->p = std::make_shared<Powerup>(Powerup(this->board.getRotations()[pieceNo][rot]));
            else
                this->p = std::make_shared<Piece>(this->board.getRotations()[pieceNo][rot]);

            this->color = rand() % (NUM_COLORS - 1) + 1;
            this->board.clearRows();
            this->board.commit();
        }
    }
}

Robot::Robot(const Board& board, int pieceNo, int incr, int col, int color, int fallingspeed, int currentScore) : Player(board, pieceNo, incr, col, color, fallingspeed, currentScore) {}

void Robot::move(int clockDiff, sf::RenderWindow &window)
{
    if (clockDiff)
    {
        col = bestMove();
        int status = this->board.movePieceDown(*p, col, incr, color, clockDiff * fallingspeed);
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
            this->p = std::make_shared<Piece>(this->board.getRotations()[pieceNo][rot]);
            this->color = rand() % (NUM_COLORS - 1) + 1;
            this->board.clearRows();
            this->board.commit();
        }
    }
}

int Robot::bestMove()
{
    int nxtCol = rand() % 2 ? 1 : -1;
    if (board.isMoveValid(this->incr, this->col + nxtCol))
        return nxtCol;
    return 0;
}

std::shared_ptr<Player> Human::clone()
{
    return std::make_shared<Human>(*this);
}

std::shared_ptr<Player> Robot::clone()
{
    return std::make_shared<Robot>(*this);
}