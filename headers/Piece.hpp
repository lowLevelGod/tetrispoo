#pragma once
#include <vector>
#include <iostream>
#include <memory>

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
       explicit Piece(const std::vector<std::pair<int, int>> &body);
       Piece(const Piece &other);
       Piece &operator=(const Piece &other);
       virtual std::shared_ptr<Piece> clone();
       virtual ~Piece() = default;

       const std::vector<std::pair<int, int>> &getBody() const { return body; }
       int getWidth() { return width; }
       int getHeight() { return height; }
       const std::vector<std::pair<int, int>> &getSkirt() const { return skirt; }
       friend std::ostream &operator<<(std::ostream &os, const Piece &a);
       friend bool operator==(const Piece &a, const Piece &b);

protected:
       std::vector<std::pair<int, int>> body;
       std::vector<std::pair<int, int>> skirt;
       int width;
       int height;
};

class Powerup : public Piece
{
public:
       explicit Powerup(const std::vector<std::pair<int, int>> &body);
       Powerup(const Powerup &other);
       Powerup(const Piece &other);
       Powerup &operator=(const Powerup &other);
       std::shared_ptr<Piece> clone() override;
       virtual ~Powerup() = default;
       //TO-DO special powerup function
};
