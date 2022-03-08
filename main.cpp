#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Piece
{
    public:
        Piece() = default;
        explicit Piece(const std::vector<std::pair<int, int>> &body):
            body(body){}
        const std::vector<std::pair<int, int>> & getBody() const { return body;}
        void drawPiece(sf::RenderWindow& window)
        {
            for (auto elem : body)
            {   
                sf::RectangleShape rectangle(sf::Vector2f(elem.first * blocksize, elem.second * blocksize));
                rectangle.setFillColor(sf::Color::Green);
                window.draw(rectangle);
            }
        }
    private:
        const int blocksize = 20;
        friend std::ostream& operator<<(std::ostream& os, const Piece& p);
        std::vector<std::pair<int, int>> body;

};

std::ostream& operator<<(std::ostream& os, const Piece& p)
{
    for (auto  elem : p.body)
    {
        os << "(" << elem.first << ", " 
        << elem.second << ")" << std::endl;
    }

    return os;
}

int main()
{
    Piece square({std::make_pair(0, 0), std::make_pair(0, 1), 
    std::make_pair(1, 0), std::make_pair(1, 1)});
    std::vector<std::pair<int, int>> test = square.getBody();
    std::cout << square;
    sf::RenderWindow window(sf::VideoMode(800, 800), "My window");
    while (window.isOpen())
    {
        square.drawPiece(window);
    }
    return 0;
}