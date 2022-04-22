#pragma once

#define NUM_PIECES 7
#define GRID_WIDTH 10
#define GRID_HEIGHT 24
#define BLOCK_SIZE 35
#define GRID_START 50
#define BOARD_START 2

class Board
{
public:
    explicit Board() : grid(std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT>{}),
                       rotations(std::vector<std::vector<Piece>>(NUM_PIECES)),
                       widths(std::array<int, GRID_HEIGHT>{}),
                       heights(std::array<int, GRID_WIDTH>{}),
                       copy_widths(std::array<int, GRID_HEIGHT>{}),
                       copy_heights(std::array<int, GRID_WIDTH>{}),
                       copy_grid(std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT>{}),
                       committed(true) { computeRotations(); };
    void computeRotations()
    {
        rotations[SQUARE] = {Piece({std::make_pair(0, 0), std::make_pair(0, -1),
                                    std::make_pair(1, 0), std::make_pair(1, -1)})};

        rotations[LDOG] = {Piece({std::make_pair(0, -1), std::make_pair(1, -1),
                                  std::make_pair(1, 0), std::make_pair(2, 0)}),
                           Piece({std::make_pair(0, 0), std::make_pair(0, -1),
                                  std::make_pair(1, -1), std::make_pair(1, -2)})};

        rotations[RDOG] = {Piece({std::make_pair(0, 0), std::make_pair(1, 0),
                                  std::make_pair(1, -1), std::make_pair(2, -1)}),
                           Piece({std::make_pair(0, -1), std::make_pair(0, -2),
                                  std::make_pair(1, 0), std::make_pair(1, -1)})};

        rotations[STICK] = {Piece({std::make_pair(0, 0), std::make_pair(0, -1),
                                   std::make_pair(0, -2), std::make_pair(0, -3)}),
                            Piece({std::make_pair(0, 0), std::make_pair(1, 0),
                                   std::make_pair(2, 0), std::make_pair(3, 0)})};

        rotations[THET] = {Piece({std::make_pair(0, 0), std::make_pair(1, 0),
                                  std::make_pair(2, 0), std::make_pair(1, -1)}),
                           Piece({std::make_pair(0, 0), std::make_pair(0, -1),
                                  std::make_pair(0, -2), std::make_pair(1, -1)}),
                           Piece({std::make_pair(1, 0), std::make_pair(0, -1),
                                  std::make_pair(1, -1), std::make_pair(2, -1)}),
                           Piece({std::make_pair(1, 0), std::make_pair(1, -1),
                                  std::make_pair(1, -2), std::make_pair(0, -1)})};

        rotations[LTHEL] = {Piece({std::make_pair(1, 0), std::make_pair(1, -1),
                                   std::make_pair(1, -2), std::make_pair(0, -2)}),
                            Piece({std::make_pair(0, 0), std::make_pair(1, 0),
                                   std::make_pair(2, 0), std::make_pair(2, -1)}),
                            Piece({std::make_pair(0, 0), std::make_pair(1, 0),
                                   std::make_pair(1, -1), std::make_pair(1, -2)}),
                            Piece({std::make_pair(0, 0), std::make_pair(0, -1),
                                   std::make_pair(1, -1), std::make_pair(2, -1)})};

        rotations[RTHEL] = {Piece({std::make_pair(0, 0), std::make_pair(0, -1),
                                   std::make_pair(0, -2), std::make_pair(1, -2)}),
                            Piece({std::make_pair(2, 0), std::make_pair(0, -1),
                                   std::make_pair(1, -1), std::make_pair(2, -1)}),
                            Piece({std::make_pair(0, 0), std::make_pair(1, 0),
                                   std::make_pair(0, -1), std::make_pair(0, -2)}),
                            Piece({std::make_pair(0, 0), std::make_pair(0, -1),
                                   std::make_pair(1, 0), std::make_pair(2, 0)})};
    }
    void drawGrid(sf::RenderWindow &window)
    {
        for (int i = 0; i < GRID_HEIGHT; ++i)
        {
            for (int j = 0; j < GRID_WIDTH; ++j)
                std::cout << grid[i][j] << " ";
            std::cout << std::endl;
        }
        sf::VertexArray line(sf::Lines, 2);
        line[0].color = sf::Color::Blue;
        line[1].color = sf::Color::Blue;
        for (int x = 0; x < GRID_WIDTH + 1; ++x)
        {
            line[0].position = sf::Vector2f(GRID_START + x * BLOCK_SIZE, GRID_START);
            line[1].position = sf::Vector2f(GRID_START + x * BLOCK_SIZE, GRID_START + GRID_HEIGHT * BLOCK_SIZE);
            window.draw(line);
        }
        for (int y = 0; y < GRID_HEIGHT + 1; ++y)
        {
            line[0].position = sf::Vector2f(GRID_START, GRID_START + y * BLOCK_SIZE);
            line[1].position = sf::Vector2f(GRID_START + GRID_WIDTH * BLOCK_SIZE, GRID_START + y * BLOCK_SIZE);
            window.draw(line);
        }
        for (size_t row = 0; row < GRID_HEIGHT; ++row)
            for (size_t col = 0; col < GRID_WIDTH; ++col)
                if (grid[row][col])
                    drawBlock(col, row, window, sf::Color::Green);
                else
                    drawBlock(col, row, window, sf::Color::Black);
    }
    int place(const Piece &p, const int &x, const int &y)
    {
        if (!committed)
            return -1;

        int res = PLACE_OK;
        copy_widths = widths;
        copy_heights = heights;
        copy_grid = grid;
        for (auto elem : p.getBody())
        {
            if (x + elem.first >= GRID_WIDTH || y + elem.second >= GRID_HEIGHT)
                res = PLACE_OUT_BOUNDS;
            //std::cout << x + elem.first << " " << y + elem.second << std::endl;
            if (grid[x + elem.first][y + elem.second])
                res = PLACE_BAD;
            grid[x + elem.first][y + elem.second] = 1;
            widths[x + elem.first] += 1;
            heights[y + elem.second] += 1;
        }
        for (auto row : widths)
            if (row == GRID_WIDTH)
                res = PLACE_ROW_FILLED;
        committed = false;
        return res;
    }

    int dropHeight(const Piece &p, int col)
    {
        int res = GRID_HEIGHT + 2;
        for (auto elem : p.getSkirt())
            res = std::min(GRID_HEIGHT - 1 - heights[col + elem.first] - elem.second, res);
        return res;
    }

    void undo()
    {
        grid = copy_grid;
        widths = copy_widths;
        heights = copy_heights;
        committed = true;
    }

    int clearRows()
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
            int sum = 0;
            for (size_t row = 0; row < GRID_HEIGHT; ++row)
                sum += grid[row][i];
            heights[i] = sum;
        }

        committed = false;
        return rowsCleared;
    }
    const std::vector<std::vector<Piece>> &getRotations() { return rotations; }
    friend std::ostream &operator<<(std::ostream &, const Board &);

private:
    void drawBlock(const int &x, const int &y, sf::RenderWindow &window, const sf::Color color)
    {
        sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE - 10, BLOCK_SIZE - 10));
        block.setFillColor(color);
        block.setPosition(sf::Vector2f(GRID_START + x * BLOCK_SIZE + 5, GRID_START + y * BLOCK_SIZE + 5));
        window.draw(block);
    }
    std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT> grid;
    std::vector<std::vector<Piece>> rotations;
    std::array<int, GRID_HEIGHT> widths;
    std::array<int, GRID_WIDTH> heights;
    std::array<int, GRID_HEIGHT> copy_widths;
    std::array<int, GRID_WIDTH> copy_heights;
    std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT> copy_grid;
    bool committed;
    enum PlaceReturns
    {
        PLACE_OK,
        PLACE_ROW_FILLED,
        PLACE_OUT_BOUNDS,
        PLACE_BAD
    };
};

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