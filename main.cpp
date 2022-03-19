#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <iostream>
#define NUM_PIECES 7
#define GRID_WIDTH 10
#define GRID_HEIGHT 24
#define BLOCK_SIZE 20
#define GRID_START 50

enum PieceNames
{
       SQUARE,
       LDOG,
       RDOG,
       STICK,
       THET,
       LTHEL,
       RTHEL
};

class Piece
{
public:
       Piece() = default;
       explicit Piece(const std::vector<std::pair<int, int>> &body) : body{body}
       {
              int minx = body[0].first;
              int maxx = body[0].first;
              int miny = body[0].second;
              int maxy = body[0].second;
              for (auto elem : body)
              {
                     minx = std::min(minx, elem.first);
                     maxx = std::max(minx, elem.first);
                     miny = std::min(minx, elem.second);
                     maxy = std::max(minx, elem.second);
              }
              width = maxx - minx + 1;
              height = maxy - miny + 1;
              for (int i = 0; i <= maxx; ++i)
              {
                     int mn = 100;
                     for (auto elem1 : body)
                     {
                            if (elem1.first == i)
                                   mn = std::min(mn, elem1.second);
                     }
                     if (mn != 100)
                            skirt.push_back(std::make_pair(i, mn));
              }
       }
       Piece(const Piece &other) : body{other.body}, skirt{other.skirt}, width{other.width}, height{other.height} {};
       Piece &operator=(const Piece &other)
       {
              body = other.body;
              skirt = other.skirt;
              width = other.width;
              height = other.height;

              return *this;
       }
       ~Piece() {}

       const std::vector<std::pair<int, int>> &getBody() const { return body; }
       int getWidth() { return width; }
       int getHeight() { return height; }
       const std::vector<std::pair<int, int>> &getSkirt() { return skirt; }
       friend std::ostream &operator<<(std::ostream &os, const Piece &a);
       friend bool operator==(const Piece &a, const Piece &b);

private:
       std::vector<std::pair<int, int>> body;
       std::vector<std::pair<int, int>> skirt;
       int width;
       int height;
};

std::ostream &operator<<(std::ostream &os, const Piece &a)
{
       for (auto elem : a.body)
       {
              os << "(" << elem.first << ", "
                 << elem.second << ")" << std::endl;
       }

       return os;
}

bool operator==(const Piece &a, const Piece &b)
{
       for (size_t i = 0; i < a.body.size(); ++i)
              if (a.body[i].first != b.body[i].first || a.body[i].second != b.body[i].second)
                     return false;
       return true;
}

class Board
{
public:
       explicit Board() : rotations(std::vector<std::vector<Piece>>(NUM_PIECES)),
                          widths(std::array<int, GRID_HEIGHT>{}),
                          heights(std::array<int, GRID_WIDTH>{}) { computeRotations(); };
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
                            drawBlock(GRID_START + col, GRID_START + row, window);
       }
       int place(const Piece &p, const int &x, const int &y)
       {
              for (auto elem : p.getBody())
              {
                     if (x + elem.first >= GRID_WIDTH || y + elem.second >= GRID_HEIGHT)
                            return PLACE_OUT_BOUNDS;
                     if (grid[x + elem.first][y + elem.second])
                            return PLACE_BAD;
                     grid[x + elem.first][y + elem.second] = 1;
                     widths[x + elem.first] += 1;
                     heights[y + elem.second] += 1;
              }
              for (auto row : widths)
                     if (row == GRID_WIDTH)
                            return PLACE_ROW_FILLED;
              return PLACE_OK;
       }
       void clearRows()
       {
              for (size_t i = widths.size() - 1; i > 0; --i)
              {
                     if (widths[i] == GRID_WIDTH)
                     {
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
       }
       const std::vector<std::vector<Piece>> &getRotations() { return rotations; }
       friend std::ostream &operator<<(std::ostream &, const Board &);

private:
       void drawBlock(const int &x, const int &y, sf::RenderWindow &window)
       {
              sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE - 10, BLOCK_SIZE - 10));
              block.setFillColor(sf::Color::Green);
              block.setPosition(sf::Vector2f(x * BLOCK_SIZE + 5, y * BLOCK_SIZE + 5));
              window.draw(block);
       }
       std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT> grid;
       std::vector<std::vector<Piece>> rotations;
       std::array<int, GRID_HEIGHT> widths;
       std::array<int, GRID_WIDTH> heights;
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

int main()
{
       Board board;
       sf::RenderWindow window(sf::VideoMode(800, 800), "My window");
       Piece dog = board.getRotations()[LDOG][1];

       // run the program as long as the window is open
       while (window.isOpen())
       {
              // check all the window's events that were triggered since the last iteration of the loop
              sf::Event event;
              while (window.pollEvent(event))
              {
                     // "close requested" event: we close the window
                     if (event.type == sf::Event::Closed)
                            window.close();
              }
              board.place(dog, 10, 10);
              board.drawGrid(window);
              window.display();
       }
       return 0;
}