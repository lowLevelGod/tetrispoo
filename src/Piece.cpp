#include "../headers/Piece.hpp"
Piece &Piece::operator=(const Piece &other)
{
    body = other.body;
    skirt = other.skirt;
    width = other.width;
    height = other.height;

    return *this;
}
Piece::Piece(const std::vector<std::pair<int, int>> &body) : body{body}
{
    int miny = body[0].first;
    int maxy = miny;
    int minx = body[0].second;
    int maxx = minx;
    for (auto elem : body)
    {
        miny = std::min(minx, elem.first);
        maxy = std::max(minx, elem.first);
        minx = std::min(minx, elem.second);
        maxx = std::max(minx, elem.second);
    }
    width = maxx - minx + 1;
    height = maxy - miny + 1;
    for (int i = 0; i <= maxx; ++i)
    {
        int mx = -100;
        for (auto elem1 : body)
        {
            if (elem1.second == i)
                mx = std::max(mx, elem1.first);
        }
        if (mx != -100)
            this->skirt.push_back(std::make_pair(mx, i));
    }
}

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

// Piece::~Piece() { /*std::cout << "Piece destructor" << std::endl;*/ }
Piece::Piece(const Piece &other) : body{other.body}, skirt{other.skirt}, width{other.width}, height{other.height} {}

std::shared_ptr<Piece> Piece::clone()
{
    return std::make_shared<Piece>(*this);
}

std::shared_ptr<Piece> Powerup::clone()
{
    return std::make_shared<Powerup>(*this);
}

Powerup::Powerup(const std::vector<std::pair<int, int>> &body) : Piece(body) {}
Powerup::Powerup(const Powerup &other) : Piece(other) {}
Powerup &Powerup::operator=(const Powerup &other)
{
    Piece::operator=(other);

    return *this;
}
Powerup::Powerup(const Piece &other) : Piece(other)
{
}

std::vector<Piece> PieceFactory::square()
{
    return std::vector<Piece>({Piece({std::make_pair(0, 0), std::make_pair(-1, 0),
                                      std::make_pair(0, 1), std::make_pair(-1, 1)})});
}

std::vector<Piece> PieceFactory::ldog()
{
    return std::vector<Piece>({Piece({std::make_pair(-1, 0), std::make_pair(-1, 1),
                                      std::make_pair(0, 1), std::make_pair(0, 2)}),
                               Piece({std::make_pair(0, 0), std::make_pair(-1, 0),
                                      std::make_pair(-1, 1), std::make_pair(-2, 1)})});
}
std::vector<Piece> PieceFactory::rdog()
{
    return std::vector<Piece>({Piece({std::make_pair(0, 0), std::make_pair(0, 1),
                                      std::make_pair(-1, 1), std::make_pair(-1, 2)}),
                               Piece({std::make_pair(-1, 0), std::make_pair(-2, 0),
                                      std::make_pair(0, 1), std::make_pair(-1, 1)})});
}
std::vector<Piece> PieceFactory::stick()
{
    return std::vector<Piece>({Piece({std::make_pair(0, 0), std::make_pair(-1, 0),
                                      std::make_pair(-2, 0), std::make_pair(-3, 0)}),
                               Piece({std::make_pair(0, 0), std::make_pair(0, 1),
                                      std::make_pair(0, 2), std::make_pair(0, 3)})});
}
std::vector<Piece> PieceFactory::thet()
{
    return std::vector<Piece>({Piece({std::make_pair(0, 0), std::make_pair(0, 1),
                                      std::make_pair(0, 2), std::make_pair(-1, 1)}),
                               Piece({std::make_pair(0, 0), std::make_pair(-1, 0),
                                      std::make_pair(-2, 0), std::make_pair(-1, 1)}),
                               Piece({std::make_pair(0, 1), std::make_pair(-1, 0),
                                      std::make_pair(-1, 1), std::make_pair(-1, 2)}),
                               Piece({std::make_pair(0, 1), std::make_pair(-1, 1),
                                      std::make_pair(-2, 1), std::make_pair(-1, 0)})});
}
std::vector<Piece> PieceFactory::lthel()
{
    return std::vector<Piece>({Piece({std::make_pair(0, 1), std::make_pair(-1, 1),
                                      std::make_pair(-2, 1), std::make_pair(-2, 0)}),
                               Piece({std::make_pair(0, 0), std::make_pair(0, 1),
                                      std::make_pair(0, 2), std::make_pair(-1, 2)}),
                               Piece({std::make_pair(0, 0), std::make_pair(0, 1),
                                      std::make_pair(-1, 1), std::make_pair(-2, 1)}),
                               Piece({std::make_pair(0, 0), std::make_pair(-1, 0),
                                      std::make_pair(-1, 1), std::make_pair(-1, 2)})});
}
std::vector<Piece> PieceFactory::rthel()
{
    return std::vector<Piece>({Piece({std::make_pair(0, 0), std::make_pair(-1, 0),
                                      std::make_pair(-2, 0), std::make_pair(-2, 1)}),
                               Piece({std::make_pair(0, 2), std::make_pair(-1, 0),
                                      std::make_pair(-1, 1), std::make_pair(-1, 2)}),
                               Piece({std::make_pair(0, 0), std::make_pair(0, 1),
                                      std::make_pair(-1, 0), std::make_pair(-2, 0)}),
                               Piece({std::make_pair(0, 0), std::make_pair(-1, 0),
                                      std::make_pair(0, 1), std::make_pair(0, 2)})});
}
