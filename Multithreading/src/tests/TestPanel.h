#ifndef _TEST_PANEL_H_
#define _TEST_PANEL_H
#include "Test.h"
#include <GL/glew.h>
#include "CSU_OPENGL/VertexBuffer.h"
#include "CSU_OPENGL/VertexBufferLayout.h"
#include "CSU_OPENGL/VertexArray.h"
#include "CSU_OPENGL/IndexBuffer.h"
#include "CSU_OPENGL/Shader.h"
#include "CSU_OPENGL/Renderer.h"
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