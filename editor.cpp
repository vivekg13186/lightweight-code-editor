#include "editor.h"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>
#include <string>
#define FONTSTASH_IMPLEMENTATION
#include "fontstash.h"
#define GLFONTSTASH_IMPLEMENTATION
#include "glfontstash.h"

#define DELAY_BETWEEN_KEYSTROKE_MS 20
int EDITOR_FONT_SIZE=16;
int EDITOR_LINE_SPACE=7;
int keycode[16];
int keycode_q_index;
uint64_t last_cusor_render_time =0;
int cursor_on=0
;

int cursor_row=0;
int cursor_col=0;


static inline int min(int i,int j){
    return  i>j ? i  : j;
}

static inline int max(int i,int j){
    return  i>j ? j  : i;
}
unsigned long long last_key_actioned=0;
unsigned long long timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}



  static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods){
      if(action != GLFW_PRESS)return;
     // unsigned long long current_time = timeSinceEpochMillisec();
     // unsigned long long diff_time = current_time-last_key_actioned;
     // std::cout<< last_key_actioned <<" last "<<current_time << " : " <<diff_time<<" dif \n";
     // if(diff_time<DELAY_BETWEEN_KEYSTROKE_MS){
      //    return;
      //}
      //last_key_actioned=current_time;
       if(keycode_q_index<16){
           keycode[keycode_q_index++]=key;
       }

       if(key ==GLFW_KEY_Q && scancode ==24){
         exit(0);
     }
  }

  void drawCursor(int x,int y){
      glBegin(GL_LINES);
	glColor4ub(255,244,244,255);
	glVertex2f(x,y);
	glVertex2f(x,y+14);
	glEnd();
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
		fontNormal = fonsAddFont(fs, "sans", "/home/vivek/Developer/lightweight-code-editor/Monaco.ttf");
			if (fontNormal == FONS_INVALID) {
				printf("Could not add font normal.\n");
				exit(-1);
			}
			text.push_back('v');
        	fonsSetSize(fs, EDITOR_FONT_SIZE);
			fonsSetFont(fs, fontNormal);

 FONT_COLOR_WHITE = glfonsRGBA(236,146,85,255);
 	fonsSetColor(fs, FONT_COLOR_WHITE);

}
void Window::loop(){
    while (!glfwWindowShouldClose(window))
	{
	float sx, sy, dx, dy, lh = 0;
			int width, height;



			for(int i=0;i<keycode_q_index;i++){
			 int k =keycode[i];
			 if(k==GLFW_KEY_LEFT){
			    cursor_col--;
				if(cursor_col<0){
				    cursor_row=min(cursor_row-1,0);
					cursor_col=doc.getLineCharLength(cursor_row);
				}
			}else if(k==GLFW_KEY_RIGHT){
			     cursor_col++;
				if(cursor_col>doc.getLineCharLength(cursor_row)){
				    int new_row  = min(cursor_row+1,doc.getLineCount());
					if(cursor_row!=new_row){
					   cursor_col=0;
					   cursor_row=new_row;
					}else{
					  cursor_col--;
					}
				}
			}else if(k==GLFW_KEY_UP){
			     cursor_row = min(cursor_row-1,0);
				 cursor_col  = min(cursor_col ,doc.getLineCharLength(cursor_row));
			}else if(k==GLFW_KEY_DOWN){
			    cursor_row = min(cursor_row,doc.getLineCount()-1);
				cursor_col = min(cursor_col ,doc.getLineCharLength(cursor_row));
			}else if(k==GLFW_KEY_ENTER){
			 cursor_col=0;
				cursor_row++;
		      doc.appendChar(keycode[i]);
			}else {
			 cursor_col++;
			 doc.appendChar(keycode[i]);
			}
}
            //std::cout << "cursor cr cc" << cursor_row << ":" <<cursor_col << "\n";
			keycode_q_index=0;
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
			uint64_t current_time=timeSinceEpochMillisec();

		    if((current_time - last_cusor_render_time)>500){
						cursor_on = cursor_on == 0 ?1 :0;
						last_cusor_render_time = current_time;
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
		//int cy = linex-(EDITOR_FONT_SIZE+EDITOR_LINE_SPACE);
		//int cx = doc.getLine(doc.currentLine)->length()*(EDITOR_FONT_SIZE/2);
		 int cx = cursor_col*(EDITOR_FONT_SIZE/2);
	     cx+=40+3;
		int cy = (cursor_row*line_row_height)+ editor_text_offset_height;
		//int cy =abs((cursor_row*EDITOR_FONT_SIZE+EDITOR_LINE_SPACE)-(EDITOR_FONT_SIZE+EDITOR_LINE_SPACE));
		 drawCursor(cx,cy);
		}


			glEnable(GL_DEPTH_TEST);
	glfwSwapBuffers(window);

	glfwPollEvents();
	}
}

void Window::keyCallback(int  key,int   scancode,int   action,int   mods){
    /*std::cout<<"Key "<<key<<" Scancode " <<scancode <<" Action "<<action<<" Mods "<<mods<<"\n";
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
*/
}


void Window::drawLine(float sx, float sy, float ex, float ey)
{
	glBegin(GL_LINES);
	glColor4ub(210,210,210,255);
	glVertex2f(sx,sy);
	glVertex2f(ex,ey);
	glEnd();
}

 void Window::drawtext(const char* text,int x,int y){
fonsSetAlign(fs, FONS_ALIGN_LEFT | FONS_ALIGN_TOP);
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
 void Document::appendChar(int c){

     if(c==GLFW_KEY_BACKSPACE){
         if(lines.at(currentLine)->size()==0){
             if(lines.size()>1){
             lines.pop_back();
             }

         }else {
         lines.at(currentLine)->pop_back();
         }

     }else if(c==GLFW_KEY_ENTER){
        std::cout<<"new line";
        lines.push_back(new std::string(""));
        currentLine++;
     }else{
         lines.at(currentLine)->push_back(c);
     }

 }
 int Document::getLineCharLength(int row){
     if(row>=lines.size()){
         return -1;
     }
     return  lines.at(row)->length();
 }
