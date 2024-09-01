
#include <cstdint>
#include <string>
#include <iostream>
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <sys/types.h>
#define FONTSTASH_IMPLEMENTATION
#include "fontstash.h"
#define GLFONTSTASH_IMPLEMENTATION
#include "glfontstash.h"
using namespace std;

typedef  struct ColorRGB{ unsigned char r;unsigned char g;unsigned char b;} ColorRGB;
typedef  struct Cursor{ int row;int col;u_int64_t lasteRendered;char off;} Cursor;

GLFWwindow* window;
FONScontext* fs ;
Cursor cursor;
int keycodes[16];
int keycodes_index=0;
//config
int DELAY_BETWEEN_KEYSTROKE_MS=20;
int EDITOR_FONT_SIZE=16;
int EDITOR_LINE_SPACE=7;
string FONT_LOCATION="/home/vivek/Developer/lightweight-code-editor/Monaco.ttf";
ColorRGB CURSOR_COLOR = {255, 255,255};

static inline int RGBToInt(int r, int g, int b) {
   int c = r;
   c = (c << 8) | g;
   c = (c << 8) | b;
   return (r) | (g << 8) | (b << 16) | 255 << 24;
 }
 static inline int min(int i,int j){
     return  i>j ? i  : j;
 }

 static inline int max(int i,int j){
     return  i>j ? j  : i;
 }
uint64_t GetSystemTimeInMillSec() {
   using namespace chrono;
   return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
 }
 static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods){
     if(action != GLFW_PRESS)return;
     if(keycodes_index<16){
          keycodes[keycodes_index++]=key;
      }
      if(key ==GLFW_KEY_Q && scancode ==24){
        exit(0);
    }
 }

GLFWwindow* CreateNewWindow(string title,int width,int height){
    if (!glfwInit()) {
		std::cout << "Failed to init GLFW.";
		exit(1);
	}
    //glfwSetErrorCallback(errorcb)
	GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!window) {
		glfwTerminate();
	    exit(1);
	}

	glfwSetKeyCallback(window, KeyCallback);

	glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK) {
	 std::cout<< "Could not init glew.\n";
		exit(1);
	}
	glGetError();
	glfwSwapInterval(0);
	return window;
}

FONScontext* initFont(){
     FONScontext*   fs = glfonsCreate(512, 512, FONS_ZERO_TOPLEFT);
		if (fs == NULL) {
			printf("Could not create stash.\n");
			exit(1);
		}
	int fontNormal = FONS_INVALID;
	fontNormal = fonsAddFont(fs, "sans", FONT_LOCATION.c_str());
	if (fontNormal == FONS_INVALID) {
		printf("Could not add font normal.\n");
		exit(-1);
	}

    fonsSetSize(fs, EDITOR_FONT_SIZE);
	fonsSetFont(fs, fontNormal);
    fonsSetColor(fs, RGBToInt(255,255,255));
    fonsSetAlign(fs, FONS_ALIGN_LEFT | FONS_ALIGN_TOP);
    return  fs;
}

void drawCursor(int x,int y){
    glBegin(GL_LINES);
	glColor4ub(CURSOR_COLOR.r,CURSOR_COLOR.g,CURSOR_COLOR.b,255);
	glVertex2f(x,y);
	glVertex2f(x,y+14);
	glEnd();
}

void loop(){

        while (!glfwWindowShouldClose(window))
	{
	float sx, sy, dx, dy, lh = 0;
			int width, height;



			for(int i=0;i<keycodes_index;i++){
			 int k =keycodes[i];
			 if(k==GLFW_KEY_LEFT){
			    cursor.col--;
				if(cursor.col<0){
				   cursor.row=min(cursor.row-1,0);
					cursor.col=doc.getLineCharLength(cursor.row);
				}
			}else if(k==GLFW_KEY_RIGHT){
			     cursor.col++;
				if(cursor.col>doc.getLineCharLength(cursor.row)){
				    int new_row  = min(cursor.row+1,doc.getLineCount());
					if(cursor.row!=new_row){
					   cursor.col=0;
					   cursor.row=new_row;
					}else{
					  cursor.col--;
					}
				}
			}else if(k==GLFW_KEY_UP){
			     cursor.row = min(cursor.row-1,0);
				 cursor.col  = min(cursor.col ,doc.getLineCharLength(cursor_row));
			}else if(k==GLFW_KEY_DOWN){
			    cursor.row = min(cursor.row,doc.getLineCount()-1);
				cursor.col = min(cursor.col ,doc.getLineCharLength(cursor_row));
			}else if(k==GLFW_KEY_ENTER){
			  cursor.col=0;
			  cursor.row++;
		      doc.appendChar(keycodes[i]);
			}else {
			 cursor.col++;
			 doc.appendChar(keycodes[i]);
			}
    }
                //std::cout << "cursor cr cc" << cursor_row << ":" <<cursor_col << "\n";
			keycodes_index=0;
			glfwGetFramebufferSize(window, &width, &height);
			// Update and render
			glViewport(0, 0, width, height);
			glClearColor(0.1607f, 0.152f, 0.141f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_TEXTURE_2D);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0,width,height,0,-1,1);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glDisable(GL_DEPTH_TEST);
			glColor4ub(255,255,255,255);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_CULL_FACE);
			uint64_t current_time=GetSystemTimeInMillSec();

		    if((current_time - cursor.lasteRendered)>500){
						cursor.off = cursor.off == 0 ?1 :0;
						cursor.lasteRendered = current_time;
			}

		int line_row_height  = EDITOR_FONT_SIZE+EDITOR_LINE_SPACE;
		int editor_text_offset_height = 20;
		int linex=editor_text_offset_height;

		for(int i=0;i<doc.getLineCount();i++){
		        std::string* line = doc.getLine(i);
				drawtext(std::to_string(i).c_str(),5,linex);
				drawtext(line->c_str(),40,linex);
				//drawLine(5,linex,100.0f,linex);
				linex+=line_row_height;
				//drawLine(5,linex,100.0f,linex);
		}
		if(cursor_on==0){

		 int cx = cursor_col*(EDITOR_FONT_SIZE/2);
	     cx+=40+3;
		int cy = (cursor_row*line_row_height)+ editor_text_offset_height;

		 drawCursor(cx,cy);
		}


			glEnable(GL_DEPTH_TEST);
	glfwSwapBuffers(window);

	glfwPollEvents();
	}
}
int main(){
    std::cout<<"starting lce .. \n";
    window  = CreateNewWindow("Lce",1000,600);
    fs = initFont();
    return 0;
}
