#pragma once
#include <vector>

class Tetromino {
public:
    Tetromino();
    std::vector<std::vector<int>> Shape;
    int Color;

    int H();
    int W();

    Tetromino(std::vector<std::vector<int>> s, int c);

    static Tetromino Random();
    void Rotate();
};
