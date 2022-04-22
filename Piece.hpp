#pragma once
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
                     int mx = -100;
                     for (auto elem1 : body)
                     {
                            if (elem1.first == i)
                                   mx = std::max(mx, elem1.second);
                     }
                     if (mx != -100)
                            skirt.push_back(std::make_pair(i, mx));
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
       ~Piece() { std::cout << "Piece destructor" << std::endl; }

       const std::vector<std::pair<int, int>> &getBody() const { return body; }
       int getWidth() { return width; }
       int getHeight() { return height; }
       const std::vector<std::pair<int, int>> &getSkirt() const { return skirt; }
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