#include "Menu.h"

Menu::Menu()
    :mode{USER}
{
    
    ImGui::Begin("Menu 300");                          // Create a window called "Hello, world!" and append into it.
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();


};

Menu::~Menu()
{

};

