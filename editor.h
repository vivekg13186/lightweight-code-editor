#ifndef EDITOR_H
#define EDITOR_H
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <format>
#include "fontstash.h"

#include <GL/glew.h>

#include "GLFW/glfw3.h"
#include <chrono>

typedef struct {
    int x;int y;
}Pos;


class Document{
    public:
    Document();
    std::vector<std::string*> lines;
    int currentLine =0;
    std::string* getLine(int lno);
    int getLineCount();
    void appendChar(int c);
    int getLineCharLength(int row);

};

class Window{
  public :
   int FONT_COLOR_WHITE;
   Document doc;
   GLFWwindow* window;
   FONScontext* fs ;
   int fontNormal;
   std::string text;
   int line_x=10;
   Window(std::string title,int widht,int height);
   void keyCallback(int key, int scancode, int action, int mods);
   void loop();
   void drawLine(float sx, float sy, float ex, float ey);
    void drawtext(const char* text,int x,int y);

};


#endif
