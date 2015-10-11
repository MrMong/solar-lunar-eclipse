#include "GLFWApplicationManager.h"

GLFWBaseApplication* GLFWApplicationManager::glfw_application;

void GLFWApplicationManager::set_callbacks(GLFWBaseApplication* the_managed_glfw_application)
{
  GLFWApplicationManager::glfw_application = the_managed_glfw_application; 
  glfwSetKeyCallback(GLFWApplicationManager::glfw_application->get_window(), GLFWApplicationManager::on_keydown);
  glfwSetFramebufferSizeCallback(GLFWApplicationManager::glfw_application->get_window(), GLFWApplicationManager::on_resize);
  glfwSetErrorCallback(GLFWApplicationManager::on_error);
}

void GLFWApplicationManager::on_keydown(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if(GLFWApplicationManager::glfw_application)
		GLFWApplicationManager::glfw_application->on_keydown(window, key, scancode, action, mods);
}
    
void GLFWApplicationManager::on_error(int error, const char* desc)
{
  if(GLFWApplicationManager::glfw_application) 
		GLFWApplicationManager::glfw_application->on_error(error, desc);
}

void GLFWApplicationManager::on_resize(GLFWwindow* window, int width, int height)
{
  if(GLFWApplicationManager::glfw_application) 
		GLFWApplicationManager::glfw_application->on_resize(window, width, height);
}

GLFWApplicationManager::GLFWApplicationManager(void)
{
}

GLFWApplicationManager::~GLFWApplicationManager(void)
{
}
