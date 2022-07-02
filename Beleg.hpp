#pragma once

/* ----------------------------------------------------------------
   name:           Control.hpp
   purpose:        robot control
   version:	   SKELETON CODE
   TODO:           add more callbacks (see Application.hpp)
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#include <vector>
   // for pair
#include <utility>

#include "glm/glm.hpp"

#include "Context.hpp"
#include "Application.hpp"

#include "LightSource.h"
#include "Material.h"

#include "TriangleMesh.hpp"
#include "GLSL.hpp"

OpenGLConfiguration config(glm::uvec2(2, 1), //opengl version
	OpenGLConfiguration::Profile::COMPATIBILITY,
	(Window::DOUBLE | Window::DEPTH | Window::RGB | Window::MULTISAMPLE),
	8,
	glm::uvec2(50, 50),
	glm::uvec2(512, 512),
	"Computer Graphics - Beleg");


class Beleg : public OpenGLApplication<config> {

public:

	// initialization
	static void init();

	// display scene
	static void display(void);

	// called after window rehape
	static void reshape();

	// called when menu item was selected          
	static void menu(int id);

	// keyboard callback
	static void keyPressed();

	// keyboard callback for special keys 
	static void specialKey();

	// menu entries
	static std::vector< std::pair< int, std::string > >  menuEntries;

private:
	//to test
	static float scaling; // scale

	static TriangleMesh cube;
	static TriangleMesh fig;

	static std::pair<glm::vec3, glm::vec3> box;


	static glsl::Shader diffuseShader;

	static void computeViewMatrix(void);
	static void computeProjectionMatrix(void);

	// field of view (in degrees)                                               
	static GLfloat fov;

	static float pan;

	// camera position                                                           
	static float cameraZ;
	static float cameraY;

	// near and far plane                                                        
	static float nearPlane, farPlane;

	//step size
	static float step;

	//start punkt
	static glm::vec3 translationStart;

	static glm::mat4 rotationMatrix;
	static glm::mat4 projectionMatrix;
	static glm::mat4 viewMatrix;
	static glm::mat4 modelMatrix;

	static LightSource lightSource;
	static Material material;

	struct Menu {
		enum Item { QUIT };
	};

};