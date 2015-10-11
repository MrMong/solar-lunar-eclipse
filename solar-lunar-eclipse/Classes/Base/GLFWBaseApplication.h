#ifndef GLFWBaseApplication_H
#define	GLFWBaseApplication_H

#include <string>
#define GLFW_DLL
#include <GL\glfw3.h>

/**
*  Represents a wrapper, wich encapsulates the glfw library and manage
*  the window functionality of the OpenGL-application.
*
*  @author Tino Herrmann, Alexander Ihm
*  @version 1.0
*/
class GLFWBaseApplication
{
	private:
	  /** GLFW window, on which the simulation is displayed. */
		GLFWwindow* window;

	public:
	  /** Creates a glfw base application with the given window title,
		 *  window width and window height.<br /><br />
		 *  It initializes the glfw library and window.
     *  
		 *  @param  window_title   	Title of window to be displayed. 
		 *  @param  window_width    Width of window to be displayed. 
		 *  @param  window_height    Height of window to be displayed.
     * 
     *  @throw invalid_argument Throws if glfw library or glfw window cannot be initalized.
     */  
		GLFWBaseApplication(const std::string& window_title, int window_width, int window_height);

		/** Destroyes the glfw base application with glfw window and deallocates their memory.
     */
		virtual ~GLFWBaseApplication();

		/** Called if a key is pressed.
		 *  
     *  @param  window    The window that received the event. 
		 *  @param  key    The keyboard key that was pressed or released. 
		 *  @param  scancode   	The system-specific scancode of the key. 
		 *  @param  action    GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT. 
		 *  @param  mods    Bit field describing which modifier keys were held down.
     */
		virtual void on_keydown(GLFWwindow* window, int key, int scancode, int action, int mods);

	  /** Called if an error occurred.
     *  
		 *  @param  error    Error number which represents the occured error.
		 *  @param  desc    Description of error.
     */
    virtual void on_error(int error, const char* desc);

		/** Called if the window is resized.
     *  
		 *  @param  window   	The window that received the event. 
		 *  @param  width    New window width.
		 *  @param  height    New window height.
     */
    virtual void on_resize(GLFWwindow* window, int width, int height);

    /** Hosts all instructions which are executed in the event loop.
     */
    virtual void gl_loop_executions() = 0;

		/** Starts the glfw base application.
     */
		void start();

		/** Returns the pointer of the current used glfw window.
     */
		GLFWwindow* get_window() const;

		private:
		 /** Starts the event loop.<br />
		  *  The event loop remains active until the window is shut down.
			*/
			void glfw_mainloop();
};

#endif
