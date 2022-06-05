#include "TestClearColor.h"


namespace test{
    TestClearColor::TestClearColor()
        :m_ClearColor {0.2f, 0.3f, 0.8f, 1.0f}
    {
    }
    
    TestClearColor::~TestClearColor()
    {
    }
    
    void TestClearColor::OnUpdate(float deltaTime)
    {
    }
    
    void TestClearColor::OnRender()
    {
        glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    
    void TestClearColor::OnImGuiRender()
    {
        // ImGui::Begin("Debug configuration");                          // Create a window called "Hello, world!" and append into it.
        ImGui::ColorEdit4("Clear Color", m_ClearColor);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}