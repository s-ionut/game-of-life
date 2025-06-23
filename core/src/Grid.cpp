#include "Grid.h"

#include <random>

Grid::Grid(int width, int height)
    : m_width(width),
      m_height(height),
      m_cells(height, std::vector<bool>(width, false))
{
}

void Grid::SetCell(int x, int y, bool alive)
{
    if (IsValidPosition(x, y))
    {
        m_cells[y][x] = alive;
    }
}

bool Grid::GetCell(int x, int y) const
{
    if (IsValidPosition(x, y))
    {
        return m_cells[y][x];
    }
    return false;
}

bool Grid::IsValidPosition(int x, int y) const
{
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

void Grid::Clear()
{
    for (auto &row : m_cells)
    {
        std::fill(row.begin(), row.end(), false);
    }
}

void Grid::Randomize(float probability)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            m_cells[y][x] = dis(gen) < probability;
        }
    }
}

int Grid::CountNeighbors(int x, int y) const
{
    int count = 0;

    for (int i = 0; i < 8; ++i)
    {
        int nx = x + NEIGHBOR_OFFSETS[i][0];
        int ny = y + NEIGHBOR_OFFSETS[i][1];

        if (GetCell(nx, ny))
        {
            count++;
        }
    }

    return count;
}

std::unique_ptr<Grid> Grid::GetNextGeneration() const
{
    auto nextGrid = std::make_unique<Grid>(m_width, m_height);

    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            int neighbors = CountNeighbors(x, y);
            bool currentCell = GetCell(x, y);
            bool nextState = false;

            if (currentCell)
            {
                nextState = (neighbors == 2 || neighbors == 3);
            }
            else
            {
                nextState = (neighbors == 3);
            }

            nextGrid->SetCell(x, y, nextState);
        }
    }

    return nextGrid;
}