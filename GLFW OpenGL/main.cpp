#include <iostream>

// GLAD, it loads needed library by using website.
#include <glad/glad.h>
// GLFW
#include <GLFW/glfw3.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *winodw);

int main(){
    
    //    glfw Option setting
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    //    make Window
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_WIDTH, "Learn OpenGL", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to Create GLFW window" <<std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    //    initialize GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to Initialize GLAD. " << std::endl;
        return -1;
    }
    
    //    every time the window size is changed, we call framebuffer_size_callback function.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    //    loop rendering.
    while(!glfwWindowShouldClose(window)){
        
        processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    //    delete or free all sources for rendering(buffers)?
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window){
//    when escape key is pressed, It closes the window?
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *winodw, int width, int heigh){
    glViewport(0,0,width,heigh);
}
