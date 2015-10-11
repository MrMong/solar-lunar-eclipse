#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <GL\glew.h>
#define GLFW_DLL
#include <GL\glfw3.h>
#include <GLUT\glut.h>
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>

#include "Classes\Base\GLFWApplicationManager.h"
#include "Classes\SolarShadowApplication.h"

using namespace glm;

int main (int argc, char** argv) 
{
	SolarShadowApplication *solar_application = new SolarShadowApplication("Planetenmodell", 1024, 768, "Shader/StandardShading.vertexshader", "Shader/StandardShading.fragmentshader");

	GLFWApplicationManager::set_callbacks(solar_application);

	solar_application->start();

	return 0;
}



