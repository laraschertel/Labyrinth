/* ---------------------------------------------------------------
   name:           Beleg.cpp
   purpose:        Beleg Template
   version:	   STARTER CODE
   author:         katrin lang
                   htw berlin
   ------------------------------------------------------------- */

#include "GLIncludes.hpp"

#include <iostream> 

#include "Beleg.hpp"
#include "Context.hpp"
#include "Input.hpp"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



// use this with care
// might cause name collisions
using namespace glm;
using namespace std;

// field of view
GLfloat Beleg::fov= 45.0;
GLfloat Beleg::cameraY = 10;
GLfloat Beleg::cameraZ= 3;

mat4 Beleg::projectionMatrix, Beleg::viewMatrix, Beleg::modelMatrix(1), Beleg::rotationMatrix(1);
TriangleMesh Beleg::cube;
TriangleMesh Beleg::fig;

glsl::Shader Beleg::diffuseShader;
float Beleg::scaling = 2.0; // scale


float Beleg::step = 0.2;

std::pair<glm::vec3, glm::vec3> Beleg::box;

LightSource Beleg::lightSource={
    // position
  glm::vec4(20, 200, 0, 1),
  // ambient color
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  // diffuse color
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  // specular color
  glm::vec4(1.0, 1.0, 1.0, 1.0),
};

vec3 Beleg::translationStart(5, 0, 11);


static vec3 lastPosition = vec3(5, 0, 11);


void Beleg::init(){
    cube.load("meshes/cube.off", false);
    fig.setWinding(TriangleMesh::CCW);
    fig.load("meshes/teapot.obj");
    
    std::pair<vec3, vec3> box = fig.getBoundingBox();

    modelMatrix = glm::translate(mat4(1), translationStart);

    mat4 rotate = glm::rotate(mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0));

    modelMatrix = modelMatrix*rotate;

    modelMatrix = scale(modelMatrix, vec3(0.45));
    
    box.first *= 0.45;
    box.second *= 0.45;

    
    

 

    const std::string version = "#version 120\n";

    diffuseShader.addVertexShader(version);
    diffuseShader.loadVertexShader("shaders/diffuse.vert");
    diffuseShader.compileVertexShader();
    diffuseShader.addFragmentShader(version);
    diffuseShader.loadFragmentShader("shaders/diffuse.frag");
    diffuseShader.compileFragmentShader();
    diffuseShader.bindVertexAttrib("position", TriangleMesh::attribPosition);
    diffuseShader.bindVertexAttrib("normal", TriangleMesh::attribNormal);
    diffuseShader.link();
  // set background color to black
  glClearColor(0.0,0.0,0.0,1.0);
  glPolygonMode(GL_FRONT, GL_FILL);
    
  // enable depth test (z-buffer)
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);

  // enable antialiasing
  glEnable(GL_MULTISAMPLE);
}

// adjust to new window size
void Beleg::reshape(){
    
  // viewport
  glViewport(0, 0, (GLsizei) window->width(), (GLsizei) window->height());

  computeViewMatrix();
  computeProjectionMatrix();
}

void Beleg::computeViewMatrix(void){

    viewMatrix = glm::lookAt(vec3(5, 20, 20), vec3(5, 0, 5), vec3(0, 1, 0));

}

void Beleg::computeProjectionMatrix(void){

  // reshaped window aspect ratio
  float aspect= (float) window->width() / (float) window->height();
  
  // compute near and far plane
  float nearPlane=cameraZ/10.0f; 
  float farPlane= cameraZ*10.0f;
  
  projectionMatrix= glm::perspective(radians(fov), aspect, nearPlane, farPlane);
}

// this is where the drawing happens
void Beleg::display(void) {

    bool labyrith[10][10] = {
       {true, true, true, true, true, true, true, true, true, true},
       {true, false, true, false, false, false,false, true, false,false},
       {true, false, true, false, true, true, false, false, false, true},
       {true, false, false, false, true, true, false, true, true, true},
       {true, false, true,false, false, false, false, true, false, true},
       {true,false, true, false, true, true, true, true,false, true},
       {true, false, false, false, true, false, false, false, false, true},
       {true, false, true, true, true, false, true, true, false, true},
       {true, false,false, false, false, false, false, false, false, true},
       {true, true, true, true, true, false, true, true, true, true},
    };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    diffuseShader.bind();

    {
        bool colision = false;
        modelMatrix = modelMatrix * rotationMatrix;
        box.first = vec3(rotationMatrix * vec4(box.first, 1));
        box.second = vec3(rotationMatrix * vec4(box.second, 1));
         for (int x = 0; x < 10; x++) {
            for (int z = 0; z < 10; z++) {
                if (labyrith[x][z] == true) {
                    vec3 cubePosition = vec3(z, 0, x);
                    vec3 position = lastPosition;
                    position += vec3(rotationMatrix * vec4(step, 0, 0, 1));
                    if (cubePosition[2] == position[2] && abs(cubePosition[0] - position[0]) <= 2) {
                        colision = true;
                    }
                    else if (cubePosition[0] == position[0] && abs(cubePosition[2] - position[2]) <= 2) {
                        colision = true;
                    }
                    //else if()
                    //else colision = true;

                }
            }
        }
         if (colision == false) {
            modelMatrix = glm::translate(modelMatrix, vec3(step, 0, 0));
            lastPosition = vec3(modelMatrix * vec4(lastPosition, 1));
        }
       
        step = 0;
        rotationMatrix = mat4(1);

        glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

        diffuseShader.setUniform("transformation", mvp);
        diffuseShader.setUniform("color", vec3(1, 1, 1));
        diffuseShader.setUniform("lightPosition", inverse(modelMatrix) * lightSource.position);
        fig.draw();
    }



    for (int x = 0; x < 10; x++) {
        for (int z = 0; z < 10;z++) {

            if (labyrith[x][z] == true) {
                glm::vec3 translation(z, 0, x);
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
                model = scale(model, vec3(0.5));
                glm::mat4 mvp = projectionMatrix * viewMatrix * model;
                diffuseShader.setUniform("transformation", mvp);
                diffuseShader.setUniform("color", vec3(1, 1, 1));
                diffuseShader.setUniform("lightPosition", inverse(modelMatrix) * lightSource.position);
                cube.draw();
            }
            else continue;
        }
    }

    
  diffuseShader.unbind();
  
  // display back buffer
  window->swapBuffers();
}

// keyboard control
void Beleg::keyPressed(){
  
  // rotate selected node around 
  // x,y and z axes with keypresses
  switch(keyboard->key){
    
  case 'q':
  case 'Q':
    exit(0);
    break;

  case 'w':
      step = 0.2;
      window->redisplay();
      break;
  case 'a':
      rotationMatrix = glm::rotate(mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0));
      window->redisplay();
      break;

  case 'd':
      rotationMatrix = glm::rotate(mat4(1), glm::radians(-90.0f), glm::vec3(0, 1, 0));
      window->redisplay();
      break;
  case 's':
      rotationMatrix = glm::rotate(mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0));
      window->redisplay();
      break;
  default:
    break;
  }
}

// keyboard callback for special keys
// (arrow keys for node selection)
void Beleg::specialKey(){

  // rotate selected node around 
  // x,y and z axes with keypresses
  switch(keyboard->code) {

  case Keyboard::Code::UP:
    
    break;
  case Keyboard::Code::DOWN:
    
    break;
  case Keyboard::Code::LEFT:
    
    break;
  case Keyboard::Code::RIGHT:
    
    break;
  default:
    break;
  }
}

vector< pair < int, string > > Beleg::menuEntries{{Menu::QUIT, "quit"}};

// mouse menu control
void Beleg::menu(int id){
  
  switch (id) {
  case Menu::QUIT: 
    exit(0);

  default:
    break;
  }
}

int main(int argc, char** argv){

  // initialize OpenGL context
  OpenGLContext<Beleg>::init(&argc, argv);

  // some diagnostic output
  std::cout << "GPU: " << glGetString(GL_RENDERER) << ", OpenGL version: " << glGetString(GL_VERSION) << ", GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    
  //start event loop
  InputManager<Beleg>::startEventLoop();
  
  return 0;
}