#include "editor.h"
#include <GLFW/glfw3.h>
#include <string>
#define FONTSTASH_IMPLEMENTATION
#include "fontstash.h"
#define GLFONTSTASH_IMPLEMENTATION
#include "glfontstash.h"
  static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods){
        Window* w1  = (Window*) glfwGetWindowUserPointer(window);
         w1->keyCallback(  key,   scancode,   action,   mods);
  }

Window::Window(std::string title,int width,int height){
    std::cout << title << "\nwidth x height "<<width << " x " << "height" << "\n";
    if (!glfwInit()) {
		std::cout << "Failed to init GLFW.";
		exit(1);
	}
    //glfwSetErrorCallback(errorcb);


	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
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

	//Font

	fs = glfonsCreate(512, 512, FONS_ZERO_TOPLEFT);
		if (fs == NULL) {
			printf("Could not create stash.\n");
			exit(1);
		}
		  fontNormal = FONS_INVALID;
		fontNormal = fonsAddFont(fs, "sans", "/home/vivek/Developer/editor1/JetBrainsMonoNerdFont-Regular.ttf");
			if (fontNormal == FONS_INVALID) {
				printf("Could not add font normal.\n");
				exit(-1);
			}
			text.push_back('v');
        	fonsSetSize(fs, 18);
			fonsSetFont(fs, fontNormal);

 FONT_COLOR_WHITE = glfonsRGBA(236,146,85,255);
 	fonsSetColor(fs, FONT_COLOR_WHITE);
}
void Window::loop(){
    while (!glfwWindowShouldClose(window))
	{
	float sx, sy, dx, dy, lh = 0;
			int width, height;

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
		 //line(10,10,width,height);
			int linex=20;
		for(int i=0;i<doc.getLineCount();i++){
		        std::string* line = doc.getLine(i);

				drawtext(std::to_string(i).c_str(),5,linex);
				drawtext(line->c_str(),40,linex);
				linex+=16;
		}


			glEnable(GL_DEPTH_TEST);
	glfwSwapBuffers(window);

	glfwPollEvents();
	}
}

void Window::keyCallback(int  key,int   scancode,int   action,int   mods){
    std::cout<<"Key "<<key<<" Scancode " <<scancode <<" Action "<<action<<" Mods "<<mods<<"\n";
    if(key>=GLFW_KEY_A && key<=GLFW_KEY_Z){
       doc.appendChar((char)key);
        //text.push_back((char)scancode);
    }
    if(key==GLFW_KEY_ENTER){
        doc.appendChar('\n');
    }
    if(key ==GLFW_KEY_Q && scancode ==24){
        exit(0);
    }

}

void Window::line(float sx, float sy, float ex, float ey)
{
	glBegin(GL_LINES);
	glColor4ub(255,255,255,255);
	glVertex2f(sx,sy);
	glVertex2f(ex,ey);
	glEnd();
}

 void Window::drawtext(const char* text,int x,int y){

			 fonsDrawText(fs, x,y,text  ,NULL);
 }


 std::string* Document::getLine(int lno){
     return  lines.at(lno);
 }
 int Document::getLineCount(){
     return lines.size();
 }
 Document::Document(){
     currentLine=0;
     lines.push_back(new std::string(""));
 }
 void Document::appendChar(char c){
     if(c=='\n'){
        std::cout<<"new line";
        lines.push_back(new std::string(""));
        currentLine++;
     }else{
         lines.at(currentLine)->push_back(c);
     }

 }
