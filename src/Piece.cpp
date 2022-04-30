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
            skirt.push_back(std::make_pair(mx, i));
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

Piece::~Piece() { std::cout << "Piece destructor" << std::endl; }
Piece::Piece(const Piece &other) : body{other.body}, skirt{other.skirt}, width{other.width}, height{other.height} {};