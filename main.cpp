#include "editor.h"

#include <GLFW/glfw3.h>

int main(){
    std::cout<<"Editor1 \n";
    Window window  = Window("Editor1",1000,600);
    glfwSetWindowUserPointer(window.window, &window);
    window.loop();
    return 0;
}
