#ifndef GLFWApplicationManager_H
#define	GLFWApplicationManager_H

#include "GLFWBaseApplication.h"

/**
*  Represents a manager, wich handles the application instance.
*
*  @author Tino Herrmann, Alexander Ihm
*  @version 1.0
*/
class GLFWApplicationManager
{
	private:
		/** GLFW base application. */
		static GLFWBaseApplication* glfw_application;

	public:
	  /** Saves the given glfw base instance and sets all required callback
		 *  callback-handler for the events of glfw.
     *  
     *  @param  app    GLFW base application to be saved
     */
		static void set_callbacks(GLFWBaseApplication* app);

	private:

    /** Called if a key is pressed.
		 *  
     *  @param  window    The window that received the event. 
		 *  @param  key    The keyboard key that was pressed or released. 
		 *  @param  scancode   	The system-specific scancode of the key. 
		 *  @param  action    GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT. 
		 *  @param  mods    Bit field describing which modifier keys were held down.
     */
		static void on_keydown(GLFWwindow* window, int key, int scancode, int action, int mods);

    /** Called if an error occurred.
     *  
		 *  @param  error    Error number which represents the occured error.
		 *  @param  desc    Description of error.
     */
    static void on_error(int error, const char* desc);

    /** Called if the window is resized.
     *  
		 *  @param  window   	The window that received the event. 
		 *  @param  width    New window width.
		 *  @param  height    New window height.
     */
    static void on_resize(GLFWwindow* window, int width, int height);

    /** Empty constructor because is a static class.
     */
		GLFWApplicationManager::GLFWApplicationManager();

    /** Empty deconstructor because is a static class.
     */
		GLFWApplicationManager::~GLFWApplicationManager();
};

#endif