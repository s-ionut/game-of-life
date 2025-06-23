#pragma once

#include "Application.h"
#include "Grid.h"
#include <memory>
#include <vector>

class GameUI;

class GameOfLife : public Application
{
public:
    GameOfLife(const char *title, int width = 1200, int height = 800);
    ~GameOfLife() override;

protected:
    void Update(float deltaTime) override;
    void Draw() override;
    void DrawUI() override;

public:
    // Public interface for UI panels
    bool IsPaused() const { return m_paused; }
    void SetPaused(bool paused) { m_paused = paused; }
    void Step()
    {
        if (m_paused)
        {
            m_grid = m_grid->GetNextGeneration();
            m_generation++;
        }
    }

    int GetGeneration() const { return m_generation; }
    int GetPopulation() const { return m_populationCount; }
    int GetBirths() const { return m_birthCount; }
    int GetDeaths() const { return m_deathCount; }
    float GetSpeed() const { return m_updateInterval; }
    void SetSpeed(float speed) { m_updateInterval = speed; }

    bool IsGridVisible() const { return m_showGrid; }
    void SetGridVisible(bool visible) { m_showGrid = visible; }

    void RandomizeGrid(float probability = 0.25f)
    {
        m_grid->Randomize(probability);
        m_generation = 0;
        m_paused = true;
    }
    void ClearGrid()
    {
        m_grid->Clear();
        m_generation = 0;
        m_paused = true;
    }

    void ApplyPattern(const std::vector<std::vector<bool>> &pattern, int startX, int startY);

    const Grid *GetGrid() const { return m_grid.get(); }

private:
    void HandleInput();
    void HandleKeyboardInput();
    void HandleMouseInput();
    void HandleSpeedControls();
    void HandlePatternShortcuts();

    void DrawGrid();
    void UpdateSimulation();
    void CalculateStatistics(const Grid &oldGrid, const Grid &newGrid);

    Vector2 ScreenToGrid(Vector2 screenPos) const;
    Vector2 GridToScreen(int gridX, int gridY) const;

    std::unique_ptr<Grid> m_grid;
    std::unique_ptr<GameUI> m_ui;

    static constexpr int GRID_WIDTH = 120;
    static constexpr int GRID_HEIGHT = 80;
    static constexpr int CELL_SIZE = 8;
    static constexpr int GRID_OFFSET_X = 300; // Moved right to make space for UI
    static constexpr int GRID_OFFSET_Y = 50;

    float m_updateTimer;
    float m_updateInterval;
    bool m_paused;
    bool m_showGrid;

    int m_generation;
    int m_populationCount;
    int m_birthCount;
    int m_deathCount;
};