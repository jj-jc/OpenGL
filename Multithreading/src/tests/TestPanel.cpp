#include "TestPanel.h"

namespace test{
    TestPanel::TestPanel()
        : m_Color{1.0f, 0.0f, 0.0f, 1.0f}
    {
        float positions[] = {
            // positions           // texture coordanates
            300.0f, 100.0f, 0.0f,  // bottom right
            100.0f, 100.0f, 0.0f,  // bottom left
            100.0f, 300.0f, 0.0f,  // top left
            300.0f, 300.0f, 0.0f,  // top right
        };

        unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
        };
        
        glm::mat4 proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f); // projects the view into a field of (16 x 16 normalized)

        VertexBuffer vbo(positions, sizeof(positions));
        VertexBufferLayout layout; // just store the information needed
        layout.push<float>(3);
        m_VAO = std::make_unique<VertexArray>();
        m_VAO->addBuffer(vbo, layout); // creates the buffer with the info of layout and the values of vbo
        m_IBO = std::make_unique<IndexBuffer>(indices, sizeof(indices));

        // Create a complete shader program (with vertex and fragment shaders)    
        m_Shader = std::make_unique<Shader>(Shader::getShaderSource("/home/jjjurado/Dev/OpenGL/VisualObjects/res/shaders/panel.vs"), 
                        Shader::getShaderSource("/home/jjjurado/Dev/OpenGL/VisualObjects/res/shaders/panel.fs"));
        m_Shader->use();
        m_Shader->setUniform4f("u_Color", glm::vec4(m_Color[0], m_Color[1], m_Color[2], m_Color[3]));
        m_Shader->setUniformMatrix4fv("u_MVP", proj);
        m_VAO->unbind();
        vbo.unbind();
        m_IBO->unbind();
        m_Shader->unbind();
    }

    TestPanel::~TestPanel()
    {

    }

    void TestPanel::OnUpdate(float deltaTime)
    {

    }

    void TestPanel::OnRender()
    {

        Renderer renderer;

        {
            m_Shader->use();
            m_Shader->setUniform4f("u_Color", glm::vec4(m_Color[0], m_Color[1], m_Color[2], m_Color[3]));
            renderer.draw(*m_VAO, *m_IBO, *m_Shader); // Although the vao stores the ibo too, it is more flexible if it is possible to change that information
            m_Shader->unbind();
        }

    }

    void TestPanel::OnImGuiRender()
    {
        ImGui::ColorEdit4("Panel color", &m_Color[0]);
    }


}