#pragma once
#include <iostream>
#include <array>
#include <SFML/Graphics.hpp>
#include "Piece.hpp"

const int NUM_PIECES = 7;
const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 24;
const int BLOCK_SIZE = 35;
const int GRID_START = 50;
const int BOARD_START = 2;
const int NUM_COLORS = 6;

class Board
{
public:
    enum PlaceReturns
    {
        PLACE_OK,
        PLACE_ROW_FILLED,
        PLACE_OUT_BOUNDS,
        PLACE_BAD
    };
    explicit Board();
    void drawGrid(sf::RenderWindow &window);
    int place(const Piece &p, int x, int y, int color);

    int dropHeight(const Piece &p, int col);

    void undo();

    int clearRows();

    int movePieceDown(const Piece &p, int col, int &incr, int color);
    void commit() { committed = true; }
    bool isFilled(int row, int col) { return grid[row][col]; }
    const std::vector<std::vector<Piece>> &getRotations() { return rotations; }
    friend std::ostream &operator<<(std::ostream &, const Board &);

private:
    void computeRotations();
    void drawBlock(int x, int y, sf::RenderWindow &window, const sf::Color color);
    std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT> grid;
    std::vector<std::vector<Piece>> rotations;
    std::array<int, GRID_HEIGHT> widths;
    std::array<int, GRID_WIDTH> heights;
    std::array<int, GRID_HEIGHT> copy_widths;
    std::array<int, GRID_WIDTH> copy_heights;
    std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT> copy_grid;
    std::array<sf::Color, NUM_COLORS> colors;
    bool committed;
};
