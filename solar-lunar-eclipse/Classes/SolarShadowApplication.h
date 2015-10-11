#ifndef SolarShadowApplication_H
#define	SolarShadowApplication_H

#include <vector>
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include "Base\GLFWBaseApplication.h"
#include "ModelObject.h"

using namespace glm;

/**
*  Represents the OpenGL application. It realize the simulation for
*  eclipse of the sun and moon plus their self-rotation and 
*  speed-control mechanismen
*
*  @author Tino Herrmann, Alexander Ihm
*  @version 1.0
*/
class SolarShadowApplication : public GLFWBaseApplication
{
	private:
	  /** Id of OpenGL programm. */
		GLuint opengl_program_id;

		/** Matrix of projection. */
		mat4 projection;

		/** Matrix of view. */
		mat4 view;

		/** Matrix of model. */
		mat4 model;

		/** Position of world light. */
		vec3 worldlight_position;

		/** Power of world light. */
		float worldlight_power;

		/** Model that represents the earth. */
		ModelObject *earth;

		/** Model that represents the sun. */
		ModelObject *sun;

		/** Model that represents the moon. */
		ModelObject *moon;

		/** Vector which contains all star models. */
		std::vector<ModelObject*> stars;

		/** Vertex id of the moon. */
		GLuint moon_orbit_vertex_id;

		/** Vertex buffer id of the moon. */
		GLuint moon_orbit_vertex_buffer_id;

		/** Vertex id of the earth. */
		GLuint earth_orbit_vertex_id;

		/** Vertex buffer id of the earth. */
		GLuint earth_orbit_vertex_buffer_id;

		/** Hours of a day. */
		float dayhour;

		/** Days of a year. */
		float yearday;

		/** Speed of simulation. */
		float simulation_speed;

		/** Speed of camera. */
		vec3 camera_position;

		/** Speed of motion. */
		float motion_speed;

		/** Direction of motion. */
		vec3 motion_direction;

		/** Delta time. */
		float delta_time;

		/** Time since start of simulation. */
		double old_time_since_start;

		/** Right vector. */
		vec3 right_vector;

		/** Horizontal angle. */
		float horizontal_angle;

		/** Vertical angle. */
		float vertical_angle;

		/** Speed of mouse. */
		float mouse_speed;

	public:
		/** Creates a glfw application with the given window title,
		 *  window width, window height, path to vertexshader and path
		 *  to fragment shader.<br /><br />
		 *  It initializes the glfw library, glew library, window and shaders.
     *  
		 *  @param  window_title   	Title of window to be displayed. 
		 *  @param  window_width    Width of window to be displayed. 
		 *  @param  window_height    Height of window to be displayed.
		 *  @param  vertexshader_path    File path to vertex shader. 
		 *  @param  fragmentshader_path    File path to fragment shader.
     * 
     *  @throw invalid_argument Throws if glew library cannot be initalized.
     */  
		SolarShadowApplication(const std::string& window_title, int window_width, int window_height, 
													 const std::string& vertexshader_path, const std::string& fragmentshader_path);

		/** Destroyes the application and deallocates the memory.
     */
		virtual ~SolarShadowApplication();

	private:
	  /** Called if a key is pressed.
		 *  
     *  @param  window    The window that received the event. 
		 *  @param  key    The keyboard key that was pressed or released. 
		 *  @param  scancode   	The system-specific scancode of the key. 
		 *  @param  action    GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT. 
		 *  @param  mods    Bit field describing which modifier keys were held down.
     */
		virtual void on_keydown(GLFWwindow* window, int key, int scancode, int action, int mods) override;

		/** Hosts all instructions which are executed in the event loop.
     */
		virtual void gl_loop_executions() override;

	  /** Initialize the given shaders and loads they in OpenGL.
		 *  
		 *  @param  vertexshader_path    File path to vertex shader. 
		 *  @param  fragmentshader_path    File path to fragment shader.
     */
		GLuint initialize_shaders(const std::string& vertexshader_path, const std::string& fragmentshader_path);

		/** Initialize the required params for the movable camera.
     */
		void initialize_camera_parameters();

		/** Initialize the 3D objects (vertices and fragments).
     */
		void initialize_3d_objectdata();

		/** Initialize the required params for the lighting.
     */
		void initialize_light();

		/** Initialize the required params for the simulation.
     */
		void initialize_simulation_parameters();

		/** Sends the transformation matrix (mvp) to the vertex shader.
		 *  
		 *  @param  the_model    Matrix of the model. 
		 *  @param  the_view    Matrix of the view.
		 *  @param  the_projection    Matrix of the projection.
		 *  @param  the_program_id    Id of the OpenGL programm to be sent.
     */
		void send_mvp(mat4&, mat4&, mat4&, GLuint);	

		/** Calculates the user motion and add this to the given view matrix.
		 *  
		 *  @param  view_mp    Matrix of view to be added.
     */
		void compute_input_controls(mat4&);	

		/** Calculates the speed of rotation with the given speed value.
		 *  
		 *  @param  speed    Rotation speed in hour of day.
     */
		void compute_rotation_speed(float&);

		/** Calculates a point of an ellipse which is in the x-z-plain and
		 *  dependent on the given der_sin_multiplikator and der_cos_multiplikator. 
		 *  
		 *  @param  der_ellipsenwinkel    Angle of ellipse. 
		 *  @param  der_cos_multiplikator    Multiplier of cosin (x radius).
		 *  @param  der_sin_multiplikator    Multiplier of sin (z radius).
		 *  @param  trans_x_wert
		 *  @param  trans_y_wert
		 *  @param  trans_z_wert
     */
		void calculate_elliptic_coordinate(float, float, float, float&, float&, float&);

		/** Creates an elliptic orbit by point creation.
		 *  
		 *  @param  x_radius
		 *  @param  z_radius
		 *  @param  vertexarray_id
		 *  @param  vertexarraybuffer_id
     */
		void create_orbit(float, float, GLuint&, GLuint&);

		/** Draws an elliptic orbit with the given points.
		 *  
		 *  @param  vertexarray_id	Id of an array which contains vertices to be drew.
     */
		void draw_orbit(GLuint&);

		/** Creates the number of given stars with random coordinates and saved them
		 *  in the internal attribut <i>stars</i>.
		 *  
		 *  @param  count    Number of stars to be created. 
     */
		void initialize_stars(const unsigned int);

	  /** Draws all stars which are in the internal attribut <i>stars</i>.
     */
		void put_stars();
};

#endif

