#include "../headers/Board.hpp"
#include <algorithm>
#include <cmath>

std::ostream &operator<<(std::ostream &os, const Board &b)
{
    os << "Square rotations : " << std::endl;
    for (size_t i = 0; i < b.rotations[SQUARE].size(); ++i)
        os << "Rotation +" << 90 * i << "degrees" << std::endl
           << b.rotations[SQUARE][i] << std::endl;
    os << "LDog rotations : " << std::endl;
    for (size_t i = 0; i < b.rotations[LDOG].size(); ++i)
        os << "Rotation +" << 90 * i << "degrees" << std::endl
           << b.rotations[LDOG][i] << std::endl;
    os << "Rdog rotations : " << std::endl;
    for (size_t i = 0; i < b.rotations[RDOG].size(); ++i)
        os << "Rotation +" << 90 * i << "degrees" << std::endl
           << b.rotations[RDOG][i] << std::endl;
    os << "Stick rotations : " << std::endl;
    for (size_t i = 0; i < b.rotations[STICK].size(); ++i)
        os << "Rotation +" << 90 * i << "degrees" << std::endl
           << b.rotations[STICK][i] << std::endl;
    os << "The t rotations : " << std::endl;
    for (size_t i = 0; i < b.rotations[THET].size(); ++i)
        os << "Rotation +" << 90 * i << "degrees" << std::endl
           << b.rotations[THET][i] << std::endl;
    os << "Left l rotations : " << std::endl;
    for (size_t i = 0; i < b.rotations[LTHEL].size(); ++i)
        os << "Rotation +" << 90 * i << "degrees" << std::endl
           << b.rotations[LTHEL][i] << std::endl;
    os << "Right l rotations : " << std::endl;
    for (size_t i = 0; i < b.rotations[RTHEL].size(); ++i)
        os << "Rotation +" << 90 * i << "degrees" << std::endl
           << b.rotations[RTHEL][i] << std::endl;
    return os;
}

int Board::movePieceDown(const Piece &p, int col, int &incr, int color, int clockAmount)
{
    int limit = abs(dropHeight(p, col) - incr);
    // std::cout << incr << " " << limit << std::endl;
    int amountAdded = std::min(clockAmount, limit);
    int status = this->place(p, BOARD_START + incr + amountAdded, col, color);
    // std::cout << status << std::endl;
    if (limit != 0 && status == this->PLACE_OK)
    {
        incr += amountAdded;
    }
    else
    {
        while (status)
        {
            this->undo();
            status = this->place(p, BOARD_START + incr + amountAdded, col, color);
            --amountAdded;
            if (amountAdded < 0)
                break;
        }
        return 1; // piece landed
    }
    return 0;
}

int Board::clearRows()
{
    int rowsCleared = 0;
    for (size_t i = widths.size() - 1; i > 0; --i)
    {
        if (widths[i] == GRID_WIDTH)
        {
            rowsCleared++;
            for (size_t j = i; j > 0; --j)
            {
                grid[j] = grid[j - 1];
                widths[j] = widths[j - 1];
            }
            grid[0] = {};
            widths[0] = 0;
            ++i;
        }
    }
    for (size_t i = 0; i < heights.size(); ++i)
    {
        heights[i] = GRID_HEIGHT - 1;
        for (size_t row = 0; row < GRID_HEIGHT; ++row)
            if (grid[row][i])
            {
                heights[i] = static_cast<int>(row);
                break;
            }
    }

    committed = false;
    return rowsCleared;
}

void Board::undo()
{
    grid = copy_grid;
    widths = copy_widths;
    heights = copy_heights;
    committed = true;
}

int Board::dropHeight(const Piece &p, int col)
{
    int res = GRID_HEIGHT - 1;
    for (auto elem : p.getSkirt())
    {
        res = std::min(heights[col + elem.second] - elem.first - 1, res);
        // std::cout << heights[col + elem.second]  << " " << elem.first << std::endl;
    }
    // std::cout << res << " " << p << std::endl;
    return res;
}

int Board::place(const Piece &p, int x, int y, int color)
{
    if (!committed)
        return -1;

    int res = PLACE_OK;
    copy_widths = widths;
    copy_heights = heights;
    copy_grid = grid;
    for (auto elem : p.getBody())
    {
        if (x + elem.first >= GRID_HEIGHT || y + elem.second >= GRID_WIDTH)
        {
            res = PLACE_OUT_BOUNDS;
            break;
        }
        // std::cout << x + elem.first << " " << y + elem.second << std::endl;
        if (grid[x + elem.first][y + elem.second])
        {
            res = PLACE_BAD;
            break;
        }
        grid[x + elem.first][y + elem.second] = color;
        widths[x + elem.first] += 1;
    }
    for (size_t i = 0; i < heights.size(); ++i)
    {
        heights[i] = GRID_HEIGHT - 1;
        for (size_t row = 0; row < GRID_HEIGHT; ++row)
            if (grid[row][i])
            {
                heights[i] = static_cast<int>(row);
                break;
            }
    }
    /*for (auto row : widths)
        if (row == GRID_WIDTH)
        {
            res = PLACE_ROW_FILLED;
            break;
        }*/
    if (std::any_of(widths.begin(), widths.end(), [](int row)
                    { return row == GRID_WIDTH; }))
        res = PLACE_ROW_FILLED;
    committed = false;
    return res;
}

void Board::drawGrid(sf::RenderWindow &window)
{
    /*for (int i = 0; i < GRID_HEIGHT; ++i)
    {
        for (int j = 0; j < GRID_WIDTH; ++j)
            std::cout << grid[i][j] << " ";
        std::cout << std::endl;
    }*/
    sf::VertexArray line(sf::Lines, 2);
    line[0].color = sf::Color::Blue;
    line[1].color = sf::Color::Blue;
    for (int x = 0; x < GRID_WIDTH + 1; ++x)
    {
        line[0].position = sf::Vector2f(static_cast<float>(GRID_COL_START + x * BLOCK_SIZE), static_cast<float>(GRID_ROW_START));
        line[1].position = sf::Vector2f(static_cast<float>(GRID_COL_START + x * BLOCK_SIZE), static_cast<float>(GRID_ROW_START + GRID_HEIGHT * BLOCK_SIZE));
        window.draw(line);
    }
    for (int y = 0; y < GRID_HEIGHT + 1; ++y)
    {
        line[0].position = sf::Vector2f(static_cast<float>(GRID_COL_START), static_cast<float>(GRID_ROW_START + y * BLOCK_SIZE));
        line[1].position = sf::Vector2f(static_cast<float>(GRID_COL_START + GRID_WIDTH * BLOCK_SIZE), static_cast<float>(GRID_ROW_START + y * BLOCK_SIZE));
        window.draw(line);
    }
    for (size_t row = 0; row < GRID_HEIGHT; ++row)
        for (size_t col = 0; col < GRID_WIDTH; ++col)
            drawBlock(static_cast<int>(col), static_cast<int>(row), window, colors[grid[row][col]]);
}

void Board::computeRotations()
{
    rotations[SQUARE] = PieceFactory::square();
    rotations[LDOG] = PieceFactory::ldog();
    rotations[RDOG] = PieceFactory::rdog();
    rotations[STICK] = PieceFactory::stick();
    rotations[THET] = PieceFactory::thet();
    rotations[LTHEL] = PieceFactory::lthel();
    rotations[RTHEL] = PieceFactory::rthel();
}

Board::Board(int GRID_ROW_START, int GRID_COL_START) : grid(std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT>{}),
                                                       rotations(std::vector<std::vector<Piece>>(NUM_PIECES)),
                                                       widths(std::array<int, GRID_HEIGHT>{}),
                                                       copy_widths(std::array<int, GRID_HEIGHT>{}),
                                                       copy_heights(std::array<int, GRID_WIDTH>{}),
                                                       copy_grid(std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT>{}),
                                                       colors({sf::Color::Black, sf::Color::Green, sf::Color::Red, sf::Color::Cyan, sf::Color::Magenta, sf::Color::Yellow}),
                                                       committed(true),
                                                       GRID_ROW_START(GRID_ROW_START),
                                                       GRID_COL_START(GRID_COL_START)
{
    computeRotations();
    heights = std::array<int, GRID_WIDTH>{};
    heights.fill(GRID_HEIGHT - 1);
}

void Board::drawBlock(int x, int y, sf::RenderWindow &window, const sf::Color color)
{
    // magic numbers 5 and 10 used to center tetris block in grid cell
    sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE - 10, BLOCK_SIZE - 10));
    block.setFillColor(color);
    block.setPosition(sf::Vector2f((GRID_COL_START + x * BLOCK_SIZE + 5.f), (GRID_ROW_START + y * BLOCK_SIZE + 5.f)));
    window.draw(block);
}

bool Board::isFilled(int row, int col) { return grid[row][col]; }

bool Board::isMoveValid(int incr, int col)
{
    return col > 0 && col < GRID_WIDTH && !isFilled(incr, col);
}