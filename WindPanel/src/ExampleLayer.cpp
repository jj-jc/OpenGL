#include "ExampleLayer.h"

ExampleLayer::ExampleLayer(): Layer("ExampleLayer") {}
ExampleLayer::ExampleLayer(const std::string name): Layer(name) {}

ExampleLayer::~ExampleLayer() {}

void ExampleLayer::attach()
{
   // TODO:enable debugging
   // EnableGLDebugging();

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   // Create a complete shader program (with vertex and fragment shaders)
   static Shader myShader(
      Shader::getShaderSource(
         "/home/jjjurado/Dev/OpenGL/WindPanel/res/shaders/triangle.vs"),
      Shader::getShaderSource(
         "/home/jjjurado/Dev/OpenGL/WindPanel/res/shaders/triangle.fs"));

   m_Shader = &myShader;
   // myShader.use();

   glCreateVertexArrays(1, &m_QuadVA);
   glBindVertexArray(m_QuadVA);

   float vertices[] = {-0.5f,
                       -0.5f,
                       0.0f,
                       0.5f,
                       -0.5f,
                       0.0f,
                       0.5f,
                       0.5f,
                       0.0f,
                       -0.5f,
                       0.5f,
                       0.0f};

   glCreateBuffers(1, &m_QuadVB);
   glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

   uint32_t indices[] = {0, 1, 2, 2, 3, 0};
   glCreateBuffers(1, &m_QuadIB);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                sizeof(indices),
                indices,
                GL_STATIC_DRAW);
}

void ExampleLayer::detach()
{
   glDeleteVertexArrays(1, &m_QuadVA);
   glDeleteBuffers(1, &m_QuadVB);
   glDeleteBuffers(1, &m_QuadIB);
}

void ExampleLayer::update(double ts)
{
   // glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
   // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ExampleLayer::renderImGui()
{
   uintptr_t aux = reinterpret_cast<std::uintptr_t>(this);
   ImGui::Begin((getName() + "##" + std::to_string(aux)).c_str());
   if(ImGui::ColorEdit4("Square Base Color", glm::value_ptr(m_SquareBaseColor)))
      m_SquareColor = m_SquareBaseColor;
   ImGui::ColorEdit4("Square Alternate Color",
                     glm::value_ptr(m_SquareAlternateColor));
   ImGui::End();
}