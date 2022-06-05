#ifndef _TEST_PANEL_H_
#define _TEST_PANEL_H
#include "Test.h"
#include <GL/glew.h>
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Renderer.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>




namespace test {
    class TestPanel : public Test
    {
        public:
            TestPanel();
            ~TestPanel();

            void OnUpdate(float deltaTime) override;
            void OnRender() override;
            void OnImGuiRender() override;
        private:
            float m_Color[4];
            std::unique_ptr<VertexArray> m_VAO;
            std::unique_ptr<IndexBuffer> m_IBO;
            std::unique_ptr<Shader> m_Shader;
    };

}
#endif