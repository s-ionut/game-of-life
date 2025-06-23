#include "GameOfLife.h"
#include "GameUI.h"

GameOfLife::GameOfLife(const char *title, int width, int height)
    : Application(title, width, height),
      m_grid(std::make_unique<Grid>(GRID_WIDTH, GRID_HEIGHT)),
      m_ui(std::make_unique<GameUI>(this)),
      m_updateTimer(0.0f),
      m_updateInterval(0.1f),
      m_paused(true),
      m_showGrid(true),
      m_generation(0),
      m_populationCount(0),
      m_birthCount(0),
      m_deathCount(0)
{
    m_grid->Randomize(0.25f);
}

GameOfLife::~GameOfLife() = default;

void GameOfLife::Update(float deltaTime)
{
    HandleInput();

    if (m_paused)
        return;

    m_updateTimer += deltaTime;
    if (m_updateTimer <= m_updateInterval)
        return;

    UpdateSimulation();
}

void GameOfLife::UpdateSimulation()
{
    auto nextGrid = m_grid->GetNextGeneration();
    m_generation++;
    m_updateTimer = 0.0f;

    CalculateStatistics(*m_grid, *nextGrid);
    m_grid = std::move(nextGrid);
}

void GameOfLife::CalculateStatistics(const Grid &oldGrid, const Grid &newGrid)
{
    m_populationCount = 0;
    m_birthCount = 0;
    m_deathCount = 0;

    const int width = newGrid.GetWidth();
    const int height = newGrid.GetHeight();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const bool wasAlive = oldGrid.GetCell(x, y);
            const bool isAlive = newGrid.GetCell(x, y);

            if (isAlive)
            {
                m_populationCount++;
                if (!wasAlive)
                    m_birthCount++;
            }
            else if (wasAlive)
            {
                m_deathCount++;
            }
        }
    }
}

void GameOfLife::HandleInput()
{
    HandleKeyboardInput();
    HandleMouseInput();
}

void GameOfLife::HandleKeyboardInput()
{
    if (IsKeyPressed(KEY_SPACE))
        m_paused = !m_paused;
    if (IsKeyPressed(KEY_G))
        m_showGrid = !m_showGrid;

    if (IsKeyPressed(KEY_ENTER) && m_paused)
        Step();
    if (IsKeyPressed(KEY_R))
        RandomizeGrid();
    if (IsKeyPressed(KEY_C))
        ClearGrid();

    HandleSpeedControls();
    HandlePatternShortcuts();
}

void GameOfLife::HandleSpeedControls()
{
    if (IsKeyPressed(KEY_UP) && m_updateInterval > 0.05f)
        m_updateInterval -= 0.05f;

    if (IsKeyPressed(KEY_DOWN) && m_updateInterval < 1.0f)
        m_updateInterval += 0.05f;
}

void GameOfLife::HandlePatternShortcuts()
{
    const int centerX = GRID_WIDTH / 2;
    const int centerY = GRID_HEIGHT / 2;

    if (IsKeyPressed(KEY_ONE))
    {
        std::vector<std::vector<bool>> glider = {
            {false, true, false}, {false, false, true}, {true, true, true}};
        ApplyPattern(glider, centerX, centerY);
        m_paused = true;
    }

    if (IsKeyPressed(KEY_TWO))
    {
        std::vector<std::vector<bool>> blinker = {{true}, {true}, {true}};
        ApplyPattern(blinker, centerX, centerY);
        m_paused = true;
    }

    if (IsKeyPressed(KEY_THREE))
    {
        std::vector<std::vector<bool>> block = {{true, true}, {true, true}};
        ApplyPattern(block, centerX, centerY);
        m_paused = true;
    }
}

void GameOfLife::HandleMouseInput()
{
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || !m_paused)
        return;

    Vector2 mousePos = GetMousePosition();
    Vector2 gridPos = ScreenToGrid(mousePos);

    const int x = static_cast<int>(gridPos.x);
    const int y = static_cast<int>(gridPos.y);

    if (!m_grid->IsValidPosition(x, y))
        return;

    const bool currentState = m_grid->GetCell(x, y);
    m_grid->SetCell(x, y, !currentState);
}

void GameOfLife::Draw()
{
    BeginDrawing();

    ClearBackground(BLACK);

    DrawGrid();

    rlImGuiBegin();
    DrawUI();
    rlImGuiEnd();

    EndDrawing();
}

void GameOfLife::DrawGrid()
{
    for (int y = 0; y < m_grid->GetHeight(); ++y)
    {
        for (int x = 0; x < m_grid->GetWidth(); ++x)
        {
            Vector2 screenPos = GridToScreen(x, y);

            if (m_grid->GetCell(x, y))
            {
                DrawRectangle(
                    static_cast<int>(screenPos.x),
                    static_cast<int>(screenPos.y),
                    CELL_SIZE,
                    CELL_SIZE,
                    WHITE);
            }
            else if (m_showGrid)
            {
                DrawRectangleLines(
                    static_cast<int>(screenPos.x),
                    static_cast<int>(screenPos.y),
                    CELL_SIZE,
                    CELL_SIZE,
                    DARKGRAY);
            }
        }
    }
}

void GameOfLife::DrawUI()
{
    m_ui->Render();
}

Vector2 GameOfLife::ScreenToGrid(Vector2 screenPos) const
{
    return Vector2{
        (screenPos.x - GRID_OFFSET_X) / CELL_SIZE,
        (screenPos.y - GRID_OFFSET_Y) / CELL_SIZE};
}

Vector2 GameOfLife::GridToScreen(int gridX, int gridY) const
{
    return Vector2{
        static_cast<float>(GRID_OFFSET_X + gridX * CELL_SIZE),
        static_cast<float>(GRID_OFFSET_Y + gridY * CELL_SIZE)};
}

void GameOfLife::ApplyPattern(const std::vector<std::vector<bool>> &pattern, int startX, int startY)
{
    for (size_t y = 0; y < pattern.size(); ++y)
    {
        for (size_t x = 0; x < pattern[y].size(); ++x)
        {
            int gridX = startX + static_cast<int>(x) - static_cast<int>(pattern[0].size()) / 2;
            int gridY = startY + static_cast<int>(y) - static_cast<int>(pattern.size()) / 2;

            if (m_grid->IsValidPosition(gridX, gridY))
            {
                m_grid->SetCell(gridX, gridY, pattern[y][x]);
            }
        }
    }
}
