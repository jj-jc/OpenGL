#ifndef _MENU_H_
#define _MENU_H_

#include <GL/glew.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

enum MODE {USER = 0, SUPERATCO = 1, DEBUG = 2};

class Menu
{
    private:
        MODE mode;
    public:
        Menu();
        ~Menu();
    
    void setMode(MODE mode) {mode = mode;}   
    MODE getMode() const {return mode;} 


};



#endif
