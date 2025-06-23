#include "GameUI.h"
#include "GameOfLife.h"

#include "imgui.h"

GameUI::GameUI(GameOfLife *game)
    : m_game(game)
{
    InitializePatterns();
}

void GameUI::Render()
{
    if (!ImGui::GetCurrentContext())
        return;

    RenderSimulationControls();
    RenderStatistics();
    RenderPatternLibrary();
    RenderKeyboardControls();
}

void GameUI::RenderSimulationControls()
{
    if (!ImGui::Begin("Simulation Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::End();
        return;
    }

    // Play/Pause button with early return pattern
    const char *buttonText = m_game->IsPaused() ? "Play" : "Pause";
    if (ImGui::Button(buttonText, ImVec2(80, 30)))
    {
        m_game->SetPaused(!m_game->IsPaused());
    }

    ImGui::SameLine();
    if (ImGui::Button("Step", ImVec2(80, 30)) && m_game->IsPaused())
    {
        m_game->Step();
    }

    ImGui::Separator();

    // Speed control
    float speed = m_game->GetSpeed();
    if (ImGui::SliderFloat("Speed", &speed, 0.01f, 2.0f, "%.2f sec"))
    {
        m_game->SetSpeed(speed);
    }

    ImGui::Separator();

    // Grid operations
    if (ImGui::Button("Randomize", ImVec2(120, 25)))
    {
        m_game->RandomizeGrid();
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear", ImVec2(120, 25)))
    {
        m_game->ClearGrid();
    }

    bool showGrid = m_game->IsGridVisible();
    if (ImGui::Checkbox("Show Grid", &showGrid))
    {
        m_game->SetGridVisible(showGrid);
    }

    ImGui::End();
}

void GameUI::RenderStatistics()
{
    if (!ImGui::Begin("Statistics", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::End();
        return;
    }

    ImGui::Text("Generation: %d", m_game->GetGeneration());
    ImGui::Text("Population: %d", m_game->GetPopulation());
    ImGui::Text("Births: %d", m_game->GetBirths());
    ImGui::Text("Deaths: %d", m_game->GetDeaths());

    ImGui::Separator();

    const char *status = m_game->IsPaused() ? "PAUSED" : "RUNNING";
    ImGui::Text("Status: %s", status);
    ImGui::Text("FPS: %.1f", (float)GetFPS());

    ImGui::End();
}

void GameUI::RenderPatternLibrary()
{
    if (!ImGui::Begin("Pattern Library", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::End();
        return;
    }

    // Build pattern names array
    std::vector<const char *> patternNames;
    for (const auto &pattern : m_patterns)
    {
        patternNames.push_back(pattern.name.c_str());
    }

    ImGui::Combo("Pattern", &m_selectedPattern, patternNames.data(), static_cast<int>(patternNames.size()));

    if (ImGui::Button("Place Pattern at Center", ImVec2(200, 30)))
    {
        if (m_selectedPattern >= 0 && m_selectedPattern < static_cast<int>(m_patterns.size()))
        {
            const auto &pattern = m_patterns[m_selectedPattern];
            m_game->ApplyPattern(pattern.cells, 60, 40); // Center of 120x80 grid
        }
    }

    ImGui::Separator();
    ImGui::TextWrapped("Tip: Left-click on grid when paused to edit cells manually!");

    ImGui::End();
}

void GameUI::RenderKeyboardControls()
{
    if (!ImGui::Begin("Keyboard Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::End();
        return;
    }

    ImGui::Text("SPACE - Play/Pause");
    ImGui::Text("ENTER - Step Forward");
    ImGui::Text("R - Randomize Grid");
    ImGui::Text("C - Clear Grid");
    ImGui::Text("G - Toggle Grid Lines");
    ImGui::Text("UP/DOWN - Speed Control");
    ImGui::Text("1-3 - Quick Patterns");

    ImGui::End();
}

void GameUI::InitializePatterns()
{
    m_patterns = {
        {"Glider", {{false, true, false}, {false, false, true}, {true, true, true}}},
        {"Blinker", {{true}, {true}, {true}}},
        {"Block", {{true, true}, {true, true}}},
        {"Toad", {{false, true, true, true}, {true, true, true, false}}},
        {"Beacon", {{true, true, false, false}, {true, true, false, false}, {false, false, true, true}, {false, false, true, true}}}};
}