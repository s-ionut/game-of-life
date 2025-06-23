#pragma once

#include <vector>
#include <string>

class GameOfLife;

class GameUI
{
public:
    explicit GameUI(GameOfLife *game);
    ~GameUI() = default;

    void Render();

private:
    void RenderSimulationControls();
    void RenderStatistics();
    void RenderPatternLibrary();
    void RenderKeyboardControls();

    // UI state
    int m_selectedPattern = 0;

    // Pattern data
    struct Pattern
    {
        std::string name;
        std::vector<std::vector<bool>> cells;
    };

    std::vector<Pattern> m_patterns;
    void InitializePatterns();

    GameOfLife *m_game;
};