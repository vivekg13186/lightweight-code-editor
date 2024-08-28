#ifndef EDITOR_H
#define EDITOR_H
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include "fontstash.h"

#include <GL/glew.h>
#include "GLFW/glfw3.h"



class Window{
  public :
   GLFWwindow* window;
   FONScontext* fs ;
   int fontNormal;
   std::string text;
   Window(std::string title,int widht,int height);
   void keyCallback(int key, int scancode, int action, int mods);
   void loop();
   void line(float sx, float sy, float ex, float ey);

};


class Graphics{
    public:
        void drawtext(const char* text,int x,int y);
};
#endif
