#include "Tetromino.h"
#include <cstdlib>

int Tetromino::H() { return Shape.size(); }
int Tetromino::W() { return Shape[0].size(); }

Tetromino::Tetromino()
{
    Shape = { {1} };
    Color = 7;
}

Tetromino::Tetromino(std::vector<std::vector<int>> s, int c)
{
    Shape = s;
    Color = c;
}

Tetromino Tetromino::Random()
{
    static std::vector<Tetromino> pieces = {
        Tetromino({{1,1,1,1}}, 11),
        Tetromino({{1,1},{1,1}}, 14),
        Tetromino({{0,1,0},{1,1,1}}, 13),
        Tetromino({{1,0,0},{1,1,1}}, 10),
        Tetromino({{0,0,1},{1,1,1}}, 9),
        Tetromino({{0,1,1},{1,1,0}}, 12),
        Tetromino({{1,1,0},{0,1,1}}, 6)
    };

    return pieces[rand() % pieces.size()];
}

void Tetromino::Rotate()
{
    std::vector<std::vector<int>> r(W(), std::vector<int>(H()));
    for (int y = 0; y < H(); y++)
        for (int x = 0; x < W(); x++)
            r[x][H() - y - 1] = Shape[y][x];
    Shape = r;
}