#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <iostream>
#define NUM_PIECES 7

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
        explicit Piece(const std::vector<std::pair<int, int>> &body):
            body(body){}
        const std::vector<std::pair<int, int>> & getBody() const { return body;}
        void drawPiece(const int &offset, sf::RenderWindow& window)
        {
            const int blocksize = 20;
            for (auto elem : body)
            {   
                sf::RectangleShape rectangle(sf::Vector2f(blocksize, blocksize));
                rectangle.setFillColor(sf::Color::Green);
                rectangle.setPosition(sf::Vector2f(offset + elem.first * blocksize, offset + elem.second * blocksize));
                window.draw(rectangle);
            }
            window.display();
        }
    private:
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

std::vector<std::vector<Piece>> rotations(NUM_PIECES);

int main()
{
    Piece square({std::make_pair(0, 0), std::make_pair(0, 1), 
    std::make_pair(1, 0), std::make_pair(1, 1)});
    Piece dog({std::make_pair(0, 1), std::make_pair(0, 2), 
    std::make_pair(1, 0), std::make_pair(1, 1)});
    Piece stick({std::make_pair(0, 0), std::make_pair(0, 1), 
    std::make_pair(0, 2), std::make_pair(0, 3)});
    Piece t({std::make_pair(0, 0), std::make_pair(1, 0), 
    std::make_pair(2, 0), std::make_pair(1, 1)});

    rotations[SQUARE] = {Piece({std::make_pair(0, 0), std::make_pair(0, 1), 
    std::make_pair(1, 0), std::make_pair(1, 1)})};

    rotations[LDOG] = {Piece({std::make_pair(0, 1), std::make_pair(1, 1), 
    std::make_pair(1, 0), std::make_pair(2, 0)}), 
    Piece({std::make_pair(0, 0), std::make_pair(0, 1), 
    std::make_pair(1, 1), std::make_pair(1, 2)})};

    rotations[RDOG] = {Piece({std::make_pair(0, 0), std::make_pair(1, 0), 
    std::make_pair(1, 1), std::make_pair(2, 1)}), 
    Piece({std::make_pair(0, 1), std::make_pair(0, 2), 
    std::make_pair(1, 0), std::make_pair(1, 1)})};

    rotations[STICK] = {Piece({std::make_pair(0, 0), std::make_pair(0, 1), 
    std::make_pair(0, 2), std::make_pair(0, 3)}), 
    Piece({std::make_pair(0, 0), std::make_pair(1, 0), 
    std::make_pair(2, 0), std::make_pair(3, 0)})};
    
    //--- TO DO --- /
    rotations[THET] = {Piece({std::make_pair(0, 0), std::make_pair(1, 0), 
    std::make_pair(2, 0), std::make_pair(1, 1)}), 
    Piece({std::make_pair(0, 0), std::make_pair(0, 1), 
    std::make_pair(0, 2), std::make_pair(1, 1)}),
    Piece({std::make_pair(1, 0), std::make_pair(0, 1), 
    std::make_pair(1, 1), std::make_pair(2, 1)}),
    Piece({std::make_pair(1, 0), std::make_pair(1, 1), 
    std::make_pair(1, 2), std::make_pair(0, 1)})};

    rotations[LTHEL] = {Piece({std::make_pair(1, 0), std::make_pair(1, 1), 
    std::make_pair(1, 2), std::make_pair(0, 2)}), 
    Piece({std::make_pair(0, 0), std::make_pair(1, 0), 
    std::make_pair(2, 0), std::make_pair(2, 1)}),
    Piece({std::make_pair(0, 0), std::make_pair(1, 0), 
    std::make_pair(1, 1), std::make_pair(1, 2)}),
    Piece({std::make_pair(0, 0), std::make_pair(0, 1), 
    std::make_pair(1, 1), std::make_pair(2, 1)})};

    rotations[RTHEL] = {Piece({std::make_pair(0, 0), std::make_pair(0, 1), 
    std::make_pair(0, 2), std::make_pair(1, 2)}), 
    Piece({std::make_pair(2, 0), std::make_pair(0, 1), 
    std::make_pair(1, 1), std::make_pair(2, 1)}),
    Piece({std::make_pair(0, 0), std::make_pair(1, 0), 
    std::make_pair(0, 1), std::make_pair(0, 2)}),
    Piece({std::make_pair(0, 0), std::make_pair(0, 1), 
    std::make_pair(1, 0), std::make_pair(2, 0)})};
    //--- TO DO --- /

    std::vector<std::pair<int, int>> test = square.getBody();
    std::cout << square;
    sf::RenderWindow window(sf::VideoMode(1600, 1600), "My window");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        int offset = 0;
        for (auto elem : rotations)
        {
            for (auto piece : elem)
            {
                piece.drawPiece(offset, window);
                offset += 50;
            }
        }
    }
    return 0;
}