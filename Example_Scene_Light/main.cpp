///This program is a simple scene with Modern OpenGL to exemplify light
///Using Ambient, Diffuse and Specular Light in the Fragment Shader
///Author: Jorge Andres Gonzalez Borboa
///Following: https://learnopengl.com/#!Lighting/Basic-Lighting

///You need GLFW||GLUT||freeGLUT and GLEW||GLAD to compile this example.

///As well as GLM to do all the linear algebra
///https://glm.g-truc.net/0.9.8/index.html

///Write this define before including GLEW
#define GLEW_STATIC

///Library for loading files, (jgp, jpeg, png, etc)
#define STB_IMAGE_IMPLEMENTATION

/// third-party libraries
#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cassert>
#include <stdexcept>

#include <iostream>

using namespace std;
using namespace glm;

#include "Shader.h"
#include "Camera.h"


///\/////////////////////Data for the cube//////////////////////////////////////

///THE CUBE ISN"T GENERATED IN THE LOCAL CENTER, WE'LL FIX THIS LATER WITH A
///SELF DEFINE LOCAL MATRIX TRANSFORMATION

GLfloat verticesLight[] = {

    ///Front
    //Position
	1.0,	1.0,	1.0,    //Top Right [0]
	0.0f,	1.0,	1.0,    //Top Left [1]
	1.0,	0.0f,	1.0,    //Bottom Right [2]
	0.0f,	0.0f,	1.0,    //Bottom Left [3]

	///Back
	//Position
	1.0,	1.0,	0.0f,  //Top Right [4]
	0.0f,	1.0,	0.0f,  //Top Left  [5]
    0.0f,	0.0f,	0.0f,  //Bottom Left [6]
	1.0f,	0.0f,	0.0f   //Bottom Right   [7]

};

GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// Declares the Elements Array, where the indices to be drawn are stored
unsigned int indices [] = {
	3, 2, 6, 7, 4, 2, 0,
	3, 1, 6, 5, 4, 1, 0
};
///\////////////////////////////////////////////////////////////////////////////

///\//////////////////////////DECLARATIONS//////////////////////////////////////

///Declare Window
GLFWwindow* window;
///Declare Window aspect ratio
float fScreenWidth = 800.0f;
float fScreenHeight = 600.0f;

///Declare VAO, VBO and EBO for the cube
//VAO-Vertex Array Object
//VBO-Vertex Buffer Object
//EBO-Element Buffer Object
unsigned int VBO;
unsigned int VAO;

///Declare VAO, VBO, and EBO for the light
unsigned int lightVBO;
unsigned int lightVAO;
unsigned int lightEBO;

///Camera Initial Values
vec3 camPos = vec3(0.0f,0.0f,3.0f);
vec3 camFront = vec3(0.0f,0.0f,-1.0f);
vec3 camRight = vec3(1.0f,0.0f,0.0f);
vec3 camUp = vec3(0.0f,1.0f,0.0f);

///Camera Object
///This is a free roam camera
Camera camera(camPos,camFront,camUp,PERSPECTIVE);

///This camera is locked looking at the center of the world
//Camera camera(camPos,vec3(0,0,0),camUp,PERSPECTIVE,0);

///Time between current and last frame
float deltaTime = 0.0f;
///TimeStamp of last Frame
float lastFrame = 0.0f;

///\////////////////////////////////////////////////////////////////////////////
///Declare the local matrix
mat4 localMat;

///Declare the model matrix
mat4 modelMat;

///Declare the view matrix
mat4 viewMat;

///Declare the projection matrix
mat4 projMat;

///Declare the position of the cubes
vec3 cubePositions[] = {
  vec3( 0.0f,  0.0f,  0.0f),
  vec3( 2.0f,  5.0f, -15.0f),
  vec3(-1.5f, -2.2f, -2.5f),
  vec3(-3.8f, -2.0f, -12.3f),
  vec3( 2.4f, -0.4f, -3.5f),
  vec3(-1.7f,  3.0f, -7.5f),
  vec3( 1.3f, -2.0f, -2.5f),
  vec3( 1.5f,  2.0f, -2.5f),
  vec3( 1.5f,  0.2f, -1.5f),
  vec3(-1.3f,  1.0f, -1.5f)
};

///Position of the light object, (Lamp)
vec3 lightPos;

///\////////////////////////////////////////////////////////////////////////////


///\//////////////////////////CALLBACKS/////////////////////////////////////////

///This is the callback function for when the window gets resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    ///Just adjust the viewport to the new window size
    glViewport(0, 0, width, height);

    fScreenWidth = width;
    fScreenHeight = height;
}


///This is the callback function for input data, keyboard, mouse etc
void processInput(GLFWwindow *window)
{
    ///If the 'esc' key was pressed
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        ///Close the window
        glfwSetWindowShouldClose(window, true);
    }

///\/////////////////////////CAMERA CONTROLS////////////////////////////////////

    ///\//////////////////
    ///CAMERA POSITION////
    ///\//////////////////

    ///Move front and back
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.MoveCamera(FORWARD,deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.MoveCamera(BACKWARD,deltaTime);
    }

    ///Move left and right
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.MoveCamera(LEFT,deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.MoveCamera(RIGHT,deltaTime);
    }

    ///Move Up and Down
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
    {
        camera.MoveCamera(UP,deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
    {
        camera.MoveCamera(DOWN,deltaTime);
    }

///\////////////////////////////////////////////////////////////////////////////

    ///\/////////////////
    ///CAMERA ROTATION///
    ///\/////////////////

    ///Update new Yaw angle
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
       camera.MoveCamera(RIGHT_SPIN,deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        camera.MoveCamera(LEFT_SPIN,deltaTime);
    }

    ///Update new Pitch angle
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        camera.MoveCamera(UP_SPIN,deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {

        camera.MoveCamera(DOWN_SPIN,deltaTime);
    }

}

///\////////////////////////////////////////////////////////////////////////////

///\///////////////////INITIALIZE ALL THE FRAMEWORKS////////////////////////////
void initialize()
{
    ///Initialize GLFW
    if(glfwInit() == GLFW_TRUE)
    {
        cout << "GLFW initialized successfully" << endl;
    }
    else
    {
        cout << "Unable to initialize GLFW" << endl;
    }

    ///Set GLFW version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    ///Uncomment this function for MAC-OS
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    ///Create a window
    window = glfwCreateWindow(fScreenWidth, fScreenHeight, "ModernOpenGL", NULL,
                              NULL);
    if (window == NULL)
    {
        ///If it fails, end the program
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
    }

    ///Set the current context, the window we are currently working in.
    glfwMakeContextCurrent(window);

    ///Register 'framebuffer_size_callback' as the resize window callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    ///Set the initial viewport
    glViewport(0, 0, fScreenWidth, fScreenHeight);

    ///Initialize GLEW
    if(glewInit() == GLEW_OK)
    {
        cout << "GLEW initialized successfully" << endl;
    }
    else
    {
        cout << "Unable to initialize GLEW" << endl;
    }


}
///\////////////////////////////////////////////////////////////////////////////


///\////////////////CREATION OF VAOs VBOs and EBOs//////////////////////////////
void setBufferObjects()
{

    ///SETUP THE CUBE CONTAINER INFO

    ///Generate VBO
    glGenBuffers(1, &VBO);

    ///Generate VAO
    glGenVertexArrays(1, &VAO);

    ///First Bind the VAO, so that all the configuration is saved in this VAO
    glBindVertexArray(VAO);

    ///Bind the VBO to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    ///Populate VBO with data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    ///Set the info of how the VBO must be read
    /// position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    /// vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

///\////////////////////////////////////////////////////////////////////////////

    ///SET UP LIGHT OBJECT INFO

    ///Generate VBO
    glGenBuffers(1, &lightVBO);

    ///Generate VAO
    glGenVertexArrays(1, &lightVAO);

    ///Generate EBO
    glGenBuffers(1, &lightEBO);

    ///First Bind the VAO, so that all the configuration is saved in this VAO
    glBindVertexArray(lightVAO);

    ///Bind the VBO to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    ///Bind EBO to GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);

    ///Populate VBO with data
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLight), verticesLight,
                 GL_STATIC_DRAW);

    ///Populate EBO with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    ///Set the info of how the VBO must be read
    /// position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
}
///\////////////////////////////////////////////////////////////////////////////


///\///////////////SET MODEL, VIEW AND PROJECTION MATRIX////////////////////////


void setModelMat(Shader s, vec3 vc3Pos, vec3 Scale)
{
    ///Load Identity Matrix
    modelMat = mat4();

    ///*Remember to read the transformations bottom to top.

    ///Translate to the corresponding positions
    modelMat = translate(modelMat, vc3Pos);

    modelMat = scale(modelMat, Scale);

    ///*Remember to read the transformations bottom to top.

    ///Set Shader
    s.use();

    ///Set uniform - For the Vertex Shader
    s.setMatrix4fv("modelMat",modelMat);
}


void setViewMat (Shader s)
{
    ///Load Identity Matrix
    viewMat = mat4();

    ///Populate the view Matrix
    viewMat = camera.GetViewMatrix();

    ///Set Shader
    s.use();

    ///Set uniform - For the Vertex Shader
    s.setMatrix4fv("viewMat", viewMat);
}

void setProjMat (Shader s)
{
    ///Load Identity Matrix
    projMat = mat4();

    ///Projection.
    ///Type: Perspective
    ///FOV: 45 degrees
    ///Aspect Ratio: 800/600
    ///Near Clipping Plane: 0.1
    ///Far Clipping Plane: 100
    projMat = camera.GetProjectionMatrix();

    ///Set Shader
    s.use();

    ///Set uniform - For the Vertex Shader
    s.setMatrix4fv("projMat", projMat);

}

///\////////////////////////////////////////////////////////////////////////////

void drawCube(Shader s)
{
    ///Set the shader program
    s.use();

    ///Set the VAO
    glBindVertexArray(VAO);

    ///Draw
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawLight(Shader s)
{
    ///Set the shader program
    s.use();

    ///Set the VAO
    glBindVertexArray(lightVAO);

    ///Draw
    glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_INT, 0);
}

void calcDeltaTime()
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}


int main ()
{
    ///Initialize all the frameworks
    initialize();

    ///Compile and Link Shaders into Shader Program for the container
    Shader shader("shaders/vShader.vs","shaders/fShader.fs");

    ///Compile and Link Shaders into Shader Program for the light
    Shader lightShader("shaders/vShader.vs","shaders/lightShader.fs");

    ///Set all the info regarding buffer Objects
    setBufferObjects();

    ///Enable depth testing
    glEnable(GL_DEPTH_TEST);

    ///Draw in Wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    ///Draw in Fillmode, this is default.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    ///Tell the user to use the arrow to move
    cout << "\n-----------------------------------" << endl;
    cout << "W-A-S-D to move the camera" << endl;
    cout << "page Up and page Down to change camera elevation" << endl;
    cout << "Arrow Keys to rotate the camera" << endl;
    cout << "-----------------------------------" << endl;


    ///Set the color of the light *White Light*
    shader.use();
    shader.setVec3("lightColor",vec3(1.0f,1.0f,1.0f));

    ///This is the render loop *While the window is open*
    while(!glfwWindowShouldClose(window))
    {
        ///Get time in between frames for camera transformations
        calcDeltaTime();

        ///Set background color
        //Grey Blue
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        //Black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        ///Refresh Color Bit and Z-Buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ///Modify the light Position
        //Moving light
        /*lightPos.z = 10 * sin(glfwGetTime());
        lightPos.x = 10 * cos(glfwGetTime());
        lightPos.y = 10 * sin(glfwGetTime());*/

        //Static light
        lightPos = vec3(2.5f,2.5f,-2.5f);

        ///Update the position of the camera in the fragment shader
        shader.use();
        shader.setVec3("camPos",camera.GetPos());

        ///Set the position of the light
        shader.setVec3("lightPos",lightPos);

        ///Draw 10 cubes in different positions
        for(int i=0; i<10; i++)
        {
            ///Retrieve Position for this cube
            vec3 pos = cubePositions[i];

            ///Set up the Model Matrix (World coordinates)
            setModelMat(shader, pos, vec3(1.0f,1.0f,1.0f));

            ///Set the color of the cubes
            shader.setVec3("objColor",vec3(1.0f, 0.5f, 0.31f));

            ///Draw
            drawCube(shader);
        }

        ///Set the View Matrix (Camera Coordinates)
        setViewMat(shader);

        ///Set the Projection Matrix (the perspective of the camera)
        setProjMat(shader);

///\////////////////////////////////////////////////////////////////////////////
        ///DRAW THE LAMP

        ///Set the model Matrix (World Coordinates)
        setModelMat(lightShader,lightPos, vec3(0.5f,0.5f,0.5f));

        ///Draw the light object (Lamp)
        drawLight(lightShader);

        ///Set the View Matrix (Camera Coordinates)
        setViewMat(lightShader);

        ///Set the Projection Matrix (the perspective of the camera)
        setProjMat(lightShader);

///\////////////////////////////////////////////////////////////////////////////

        ///Process user input, in this case if the user presses the 'esc' key
        ///to close the application
        processInput(window);

        ///Swap the Front and Back buffer.
        glfwSwapBuffers(window);

        ///Poll CallBack Events
        glfwPollEvents();

    }

    ///Free resources when application ends.
    glfwTerminate();
    return 0;
}





