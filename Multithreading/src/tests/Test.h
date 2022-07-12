#ifndef _TEST_H_
#define _TEST_H_

#include <functional>
#include <vector>
#include <iostream>

// imgui lib
#include "imgui/imgui.h"



namespace test {
    class Test
    {
        public:
            Test() {}
            virtual ~Test(){} 

            virtual void OnUpdate(float deltaTime){} // it is not pure virtual because of the = 0; this means it is not necessary implemente that function
            virtual void OnRender(){}
            virtual void OnImGuiRender(){}

    };

    class TestMenu : public Test
    {
        public:
            TestMenu(Test*& currentTestPointer);
            ~TestMenu();

            void OnRender() override;
            void OnImGuiRender() override;

            template<typename T>
            void registerTest(const std::string& name)
            {
                std::cout << "Registering test " << name << std::endl;
                m_Tests.push_back(std::make_pair(name, []() {return new T();}));
            }
        private:
            Test*& m_CurrentTest;
            std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;

    };

}

#endif