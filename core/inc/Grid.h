#pragma once

#include <vector>
#include <memory>

class Grid
{
public:
    Grid(int width, int height);
    ~Grid() = default;

    void SetCell(int x, int y, bool alive);
    bool GetCell(int x, int y) const;
    bool IsValidPosition(int x, int y) const;

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

    void Clear();
    void Randomize(float probability = 0.3f);

    int CountNeighbors(int x, int y) const;
    std::unique_ptr<Grid> GetNextGeneration() const;

private:
    int m_width;
    int m_height;
    std::vector<std::vector<bool>> m_cells;

    static constexpr int NEIGHBOR_OFFSETS[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
};