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
   int FONT_COLOR_WHITE;
  GLFWwindow* window;
   FONScontext* fs ;
   int fontNormal;
   std::string text;
   int line_x=10;
   Window(std::string title,int widht,int height);
   void keyCallback(int key, int scancode, int action, int mods);
   void loop();
   void line(float sx, float sy, float ex, float ey);
    void drawtext(const char* text,int x,int y);

};



#endif
