#include <fstream>
#include <stdexcept>
#include "GLFWBaseApplication.h"

using namespace std;

GLFWBaseApplication::GLFWBaseApplication(const std::string& window_title , int window_width , int window_height)
{
  if(glfwInit())
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		GLFWwindow* temp_window = glfwCreateWindow(window_width , window_height , window_title.c_str() , nullptr , nullptr);
		if(temp_window)
		{
			window = temp_window;
			glfwMakeContextCurrent(window);
			glfwSwapInterval(1);
		}
		else
			throw runtime_error("Unable to initialize glfw window.");
	}
	else
		throw runtime_error("Unable to initialize glfw runtime.");
}

GLFWBaseApplication::~GLFWBaseApplication()
{
  glfwDestroyWindow(window);
  glfwTerminate();
}

void GLFWBaseApplication::start()
{
	this->glfw_mainloop();
}

void GLFWBaseApplication::glfw_mainloop()
{
  while(!glfwWindowShouldClose(window))
  {
    this->gl_loop_executions(); 
    glfwSwapBuffers(window);
    glfwPollEvents();
  }	
}

GLFWwindow* GLFWBaseApplication::get_window() const
{
	return window;
}

void GLFWBaseApplication::on_keydown(GLFWwindow* window , int key , int scancode , int action , int mods) 
{
}

void GLFWBaseApplication::on_error(int error , const char* desc) 
{
  fstream fstream;
  fstream.open ("log.txt", fstream::in | fstream::out | fstream::app);
  fstream << desc;
  fstream.close();	
}

void GLFWBaseApplication::on_resize(GLFWwindow* window , int width , int height)
{
  glViewport(0 , 0 , width , height);
}