#include "../headers/Player.hpp"
#include "../headers/Game.hpp"
#include "../headers/Exception.hpp"

Player::Player(const Board &board, int pieceNo, int incr, int col, int color, int fallingspeed, int currentScore, int pwrupcount) : board{board}, pieceNo{pieceNo}, incr{incr},
                                                                                                                    col{col}, color{color}, fallingspeed{fallingspeed}, currentScore{currentScore}, pwrupcount{pwrupcount}
{
    rotlen = static_cast<int>(this->board.getRotations()[pieceNo].size());
    rot = rand() % rotlen;
    p = std::make_shared<Piece>(this->board.getRotations()[pieceNo][rot]);
}

Human::Human(const Board &board, int pieceNo, int incr, int col, int color, int fallingspeed, int currentScore, int pwrupcount) : Player(board, pieceNo, incr, col, color, fallingspeed, currentScore, pwrupcount) {}

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
                ++pwrupcount;
                if (pwrupcount > Game::getMaxpwrupcount())
                    throw TooManyPowerups("Too many consecutive powerups activated !");
                // std::cout << "Powerup" << std::endl;
                int score = castedp->computeScore(this->color);
                // destroy last 2 rows
                int blocksDestroyed = 0;
                Piece oneblock = Piece({std::make_pair<int, int>(0, 0)});
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
            else
            {
                pwrupcount = 0;
            }

            this->pieceNo = rand() % NUM_PIECES;
            this->rotlen = static_cast<int>(this->board.getRotations()[pieceNo].size());
            this->col = 0;
            this->incr = 0;
            this->rot = rand() % rotlen;
            // Next piece can be powerup or normal
            double pwrup = static_cast<double>(rand()) / 1000000000;
            // std::cout << pwrup << std::endl;
            if (pwrup < 0.15)
            {
                this->p = std::make_shared<Powerup>(Powerup(this->board.getRotations()[pieceNo][rot]));
                // std::cout << "Powerup" << std::endl;
            }
            else
                this->p = std::make_shared<Piece>(this->board.getRotations()[pieceNo][rot]);

            this->color = rand() % (NUM_COLORS - 1) + 1;
            this->board.clearRows();
            this->board.commit();
        }
    }
}

Robot::Robot(const Board &board, int pieceNo, int incr, int col, int color, int fallingspeed, int currentScore, int pwrupcount, int bestCol) : Player(board, pieceNo, incr, col, color, fallingspeed, currentScore, pwrupcount), bestCol{bestCol} {}

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
            std::shared_ptr<Powerup> castedp = std::dynamic_pointer_cast<Powerup>(this->p);
            if (castedp)
            {
                ++pwrupcount;
                if (pwrupcount > Game::getMaxpwrupcount())
                    throw TooManyPowerups("Too many consecutive powerups activated !");
                // std::cout << "Powerup" << std::endl;
                int score = castedp->computeScore(this->color);
                // destroy last 2 rows
                int blocksDestroyed = 0;
                Piece oneblock = Piece({std::make_pair<int, int>(0, 0)});
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
            else
            {
                pwrupcount = 0;
            }

            this->setBestCol(std::distance(board.getHeights().begin(), std::max_element(board.getHeights().begin(), board.getHeights().end())));
            this->pieceNo = rand() % NUM_PIECES;
            this->rotlen = static_cast<int>(this->board.getRotations()[pieceNo].size());
            this->col = 0;
            this->incr = 0;
            this->rot = rand() % rotlen;
            // Next piece can be powerup or normal
            double pwrup = static_cast<double>(rand()) / 1000000000;
            // std::cout << pwrup << std::endl;
            if (pwrup < 0.15)
            {
                this->p = std::make_shared<Powerup>(Powerup(this->board.getRotations()[pieceNo][rot]));
                // std::cout << "Powerup" << std::endl;
            }
            else
                this->p = std::make_shared<Piece>(this->board.getRotations()[pieceNo][rot]);

            this->color = rand() % (NUM_COLORS - 1) + 1;
            this->board.clearRows();
            this->board.commit();
        }
    }
}

int Robot::bestMove()
{
    int nxtCol = this->bestCol - this->col;
    if (nxtCol < 0)
        nxtCol = -1;
    else if (nxtCol > 0)
        nxtCol = 1;
    else
        nxtCol = 0;

    if (board.isMoveValid(this->incr, this->col + nxtCol))
    {
        // std::cout << nxtCol << std::endl;
        return this->col + nxtCol;
    }
    return 0;
}

Player &Player::operator=(const Player &p)
{
    this->board = p.board;
    this->pieceNo = p.pieceNo;
    this->incr = p.incr;
    this->col = p.incr;
    this->rotlen = p.rotlen;
    this->rot = p.rot;
    this->p = p.p;
    this->color = p.color;
    this->fallingspeed = p.fallingspeed;
    this->currentScore = p.currentScore;
    this->pwrupcount = p.pwrupcount;
    return *this;
}

Human &Human::operator=(const Human &h)
{
    Player::operator=(h);
    return *this;
}

Robot &Robot::operator=(const Robot &r)
{
    Player::operator=(r);
    this->bestCol = r.bestCol;
    return *this;
}

void Human::reset()
{
    *this = Human(Board{70, 50}, rand() % NUM_PIECES, 0, 0, rand() % (NUM_COLORS - 1) + 1, Game::getslowfall(), 0);
}

void Robot::reset()
{
    *this = Robot(Board{70, 50 + 700}, rand() % NUM_PIECES, 0, 0, rand() % (NUM_COLORS - 1) + 1, Game::getslowfall(), 0);
}

Player::Player(const Player &p) : board{p.board}, pieceNo{p.pieceNo}, incr{p.incr}, col{p.col}, color{p.color},
                                  fallingspeed{p.fallingspeed}, currentScore{p.currentScore}, rotlen{p.rotlen}, rot{p.rot}, p{p.p}
{
}

Human::Human(const Human &h) : Player(h)
{
}

Robot::Robot(const Robot &r) : Player(r), bestCol{r.bestCol}
{
}

std::shared_ptr<Player> Human::clone()
{
    return std::make_shared<Human>(*this);
}

std::shared_ptr<Player> Robot::clone()
{
    return std::make_shared<Robot>(*this);
}