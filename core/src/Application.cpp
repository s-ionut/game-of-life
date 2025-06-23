#include "Application.h"

Application::Application(const char *title, int width, int height)
    : m_screenWidth(width),
      m_screenHeight(height),
      m_title(title),
      m_window(width, height, title),
      m_running(true)
{
    Initialize();
}

Application::~Application()
{
    Shutdown();
}

void Application::Initialize()
{
    SetTargetFPS(60);
    rlImGuiSetup(true);

    // Setup ImGui configuration
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
}

void Application::Shutdown()
{
    rlImGuiShutdown();
}

void Application::Run()
{
    while (m_running && !m_window.ShouldClose())
    {
        float deltaTime = GetFrameTime();

        Update(deltaTime);
        Draw();
    }
}

void Application::Update(float deltaTime)
{
}

void Application::Draw()
{
    BeginDrawing();

    m_window.ClearBackground(RAYWHITE);

    DrawText("Game Application", 190, 200, 20, LIGHTGRAY);

    rlImGuiBegin();
    DrawUI();
    rlImGuiEnd();

    EndDrawing();
}

void Application::DrawUI()
{
}

bool Application::IsRunning() const
{
    return m_running;
}