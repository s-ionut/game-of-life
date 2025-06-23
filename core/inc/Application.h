#pragma once

#include "raylib-cpp.hpp"
#include "imgui.h"
#include "rlImGui.h"

class Application
{
private:
    raylib::Window m_window;
    bool m_running;
    int m_screenWidth;
    int m_screenHeight;
    const char *m_title;

public:
    Application(const char *title = "Game Application", int width = 800, int height = 450);
    virtual ~Application();

    void Run();
    virtual void Update(float deltaTime);
    virtual void Draw();
    virtual void DrawUI();
    bool IsRunning() const;

private:
    void Initialize();
    void Shutdown();
};