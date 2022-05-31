#ifndef _TEST_CLEAR_COLOR_H_
#define _TEST_CLEAR_COLOR_H_

#include "Test.h"
#include <GL/glew.h>
#include "Renderer.h"
#include "imgui.h"

namespace test {
    class TestClearColor : public Test
    {
        public:
            TestClearColor();
            ~TestClearColor();

            void OnUpdate(float deltaTime) override;
            void OnRender() override;
            void OnImGuiRender() override;
        private:
            float m_ClearColor[4];
    };

}

#endif
