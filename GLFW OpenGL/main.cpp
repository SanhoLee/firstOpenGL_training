#include <iostream>

// GLAD, it loads needed library by using website.
// openGL version = 3.3
#include <glad/glad.h>
// GLFW
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *winodw);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// compile shaders : vertex, fragment shader
//
// activate vertex shader just like this
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
        "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
//
// activate fragment shader just like this
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
        "FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
    "}\n\0";

int main(){
    //    glfw Option setting , initializing and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    //    make Window, it means it makes window object.
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to Create GLFW window" <<std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // every time the window size is changed, we call framebuffer_size_callback function.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    //    initialize GLAD, load all OpenGL funtion pointers
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to Initialize GLAD. " << std::endl;
        return -1;
    }
    
    int success;
    char infoLog[512];
    // vertex shader 유형의 객채를 생성한다. 동적으로 객체를 생성하는 작업..
    // 객체를 생선한후 shader의 소스코드를 객체에 첨부해서 컴파일!
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
        std::cout<<"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader 유형의 객채를 생성한다. 동적으로 객체를 생성하는 작업..
    // 객체를 생선한후 shader의 소스코드를 객체에 첨부해서 컴파일!
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader,512,NULL,infoLog);
        std::cout<<"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
        
    // shader program을 생성해서, 생성한 shader들(vertex , fragment Shader)을 linking해주는 작업을 한다.
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();  // return shader program object id
    // attach shaders to the shader program.
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // now linking shaders each other which is attached on the program.
    glLinkProgram(shaderProgram);
    
    // check linking shaders is success or not!
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout<< "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    // 이후에 shader와 렌더링 명령은 프로그램 객체를 사용하며, 프로그램을 활성화 한후 shader를 반드시 삭제? 해줘야 한다.
    // 더이상 shader (컴파일된)객체가 필요하지 않음.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    
    //  ------------- shader and program setting end --------------  //
    
    
    // set up vertex data!
    float vertices[]={
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    
    // VBO = VERTEX BUFFER OBJECT
    // VERTEX데이터를 전송하기 위해 버퍼 변수를 생성
    unsigned int VBO;
    
    // glGenBuffers([버퍼생성 갯수],[버퍼 저장할 변수, 배열?])
    glGenBuffers(1, &VBO);
    
    // 버퍼를 바인딩한다. vertex buffer object의 버퍼 유형은 GL_ARRAY_BUFFER이며, 이를 opengl과 바인딩한다.
    // vertex 유형을 갖는 버퍼는 VBO 변수를 통해 사용할수 있게 된다.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // OpenGL에게 정점 데이터를 shader attributes(속성)등을 이용해서 받아들이는? 연결 방법을 알려줘야한다.
    // Linking Vertex Attributes , vertex속성 포인터 설정
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    
    
    
    //    loop rendering.
    while(!glfwWindowShouldClose(window)){
        
        processInput(window);
        
        // state-setting function
        glClearColor(0.5f,0.3f,0.5f, 1.0f);
        
        // state-using function
        // 화면을 위에서 셋팅한 색으로 지워버림.
        glClear(GL_COLOR_BUFFER_BIT);
        
        // activate the program
        glUseProgram(shaderProgram);
        
        
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
