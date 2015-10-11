#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glew.h>
#include "SolarShadowApplication.h"
#include "ApplicationHelper.h"
#include "ModelObject.h"

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"

using namespace std;
using namespace glm;

SolarShadowApplication::SolarShadowApplication(const string& window_title, int window_width, int window_height,
	const string& vertexshader_path, const string& fragmentshader_path)
	: GLFWBaseApplication(window_title, window_width, window_height)
{
	glewExperimental = true;
	GLenum state = glewInit();
	if (state == GLEW_OK)
	{
		opengl_program_id = initialize_shaders(vertexshader_path, fragmentshader_path);
		initialize_3d_objectdata();
		initialize_light();
		initialize_simulation_parameters();
		initialize_camera_parameters();
		initialize_stars(50);
		projection = perspective(45.0f, (4.0f / 3.0f), 0.1f, 6000.0f);
		model = mat4(1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}
	else
		throw runtime_error("Unable to initialize glew runtime.");
}

void SolarShadowApplication::on_keydown(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS))
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (((key == GLFW_KEY_UP) || (key == GLFW_KEY_W)) && ((action == GLFW_REPEAT) || (action == GLFW_PRESS)))
		camera_position += (motion_direction * delta_time * motion_speed);

	if (((key == GLFW_KEY_DOWN) || (key == GLFW_KEY_S)) && ((action == GLFW_REPEAT) || (action == GLFW_PRESS)))
		camera_position -= (motion_direction * delta_time * motion_speed);

	if (((key == GLFW_KEY_RIGHT) || (key == GLFW_KEY_D)) && ((action == GLFW_REPEAT) || (action == GLFW_PRESS)))
		camera_position -= (right_vector * delta_time * motion_speed);

	if (((key == GLFW_KEY_LEFT) || (key == GLFW_KEY_A)) && ((action == GLFW_REPEAT) || (action == GLFW_PRESS)))
		camera_position += (right_vector * delta_time * motion_speed);

	if ((key == GLFW_KEY_K) && ((action == GLFW_REPEAT) || (action == GLFW_PRESS)))
	{
		if (motion_speed < 1000.0f)
			motion_speed += 150.0f;
	}

	if ((key == GLFW_KEY_L) && ((action == GLFW_REPEAT) || (action == GLFW_PRESS)))
	{
		if (motion_speed > 0.0f)
			motion_speed -= 150.0f;
	}

	if ((key == GLFW_KEY_P) && ((action == GLFW_REPEAT) || (action == GLFW_PRESS)))
	{
		if (simulation_speed > -100.0f)
			simulation_speed /= 2.0f;
	}

	if ((key == GLFW_KEY_O) && ((action == GLFW_REPEAT) || (action == GLFW_PRESS)))
	{
		if (simulation_speed > -100.0f)
			simulation_speed *= 2.0f;
	}

	if ((key == GLFW_KEY_N) && ((action == GLFW_REPEAT) || (action == GLFW_PRESS)))
	{
		if (simulation_speed < 100.0f)
			simulation_speed += 0.1f;
	}

	if ((key == GLFW_KEY_M) && ((action == GLFW_REPEAT) || (action == GLFW_PRESS)))
	{
		if (simulation_speed > -100.0f)
			simulation_speed -= 0.1f;
	}
}

GLuint SolarShadowApplication::initialize_shaders(const string& vertexshader_path, const string& fragmentshader_path)
{
	GLuint program_id = ApplicationHelper::load_shaders(vertexshader_path, fragmentshader_path);
	glUseProgram(program_id);
	return program_id;
}

void SolarShadowApplication::initialize_camera_parameters()
{
	camera_position = vec3(0.0f, 200.0f, -300.0f);
	horizontal_angle = 0.0f;
	vertical_angle = 0.0f;
	right_vector = vec3(sin(horizontal_angle - (3.14f / 2.0f)), 0, cos(horizontal_angle - (3.14f / 2.0f)));
	motion_direction = vec3((cos(vertical_angle) * sin(horizontal_angle)), sin(vertical_angle), (cos(vertical_angle) * cos(horizontal_angle)));
	motion_speed = 150.0f;
	mouse_speed = 0.05f;
	delta_time = 0;
	old_time_since_start = glfwGetTime();

	view = lookAt(camera_position, (camera_position + motion_direction), vec3(0, 1, 0));
}

void SolarShadowApplication::initialize_stars(const unsigned int count)
{
  time_t t;
  time(&t);
  srand((unsigned int)t);
	for(unsigned int i = 0; i < count; i++)
	{
		ModelObject *temp_star = new ModelObject("Objects/sphere.obj", vec3(2.0, 2.0, 2.0));
		int plusminus_x = -10 + (rand() % (int)(10 - -10 + 1));
		int plusminus_y = -10 + (rand() % (int)(10 - -10 + 1));
		int plusminus_z = -10 + (rand() % (int)(10 - -10 + 1));
		int output_x = 0;
		int output_y = 0;
		int output_z = 0;
		if (plusminus_x < 0)
			output_x = -2000 + (rand() % (int)(-400 - -2000 + 1));
		else
			output_x = 400 + (rand() % (int)(2000 - 400 + 1));

		if (plusminus_y < 0)
			output_y = -2000 + (rand() % (int)(-400 - -2000 + 1));
		else
			output_y = 400 + (rand() % (int)(2000 - 400 + 1));

		if (plusminus_z < 0)
			output_z = -2000 + (rand() % (int)(-400 - -2000 + 1));
		else
			output_z = 400 + (rand() % (int)(2000 - 400 + 1));
		temp_star->set_3d_position((float)(output_x), (float)(output_y), (float)(output_z));
		stars.push_back(temp_star);
	}
}

void SolarShadowApplication::initialize_3d_objectdata()
{
	earth = new ModelObject("Objects/sphere.obj", vec3(15.703, 15.703, 15.703), "Textures/earth.bmp", 0);
	sun = new ModelObject("Objects/sphere.obj", vec3(80.0, 80.0, 80.0), "Textures/sun.bmp", 1);
	moon = new ModelObject("Objects/sphere.obj", vec3(4.0, 4.0, 4.0), "Textures/moon.bmp", 2);

	earth_orbit_vertex_buffer_id = 0;
	create_orbit(300.0f, 310.197f, earth_orbit_vertex_id, earth_orbit_vertex_buffer_id);
	moon_orbit_vertex_buffer_id = 0;
	create_orbit(50.0f, 55.881f, moon_orbit_vertex_id, moon_orbit_vertex_buffer_id); 
}

void SolarShadowApplication::initialize_light()
{
	worldlight_position = vec3(0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(opengl_program_id, "LightPosition_worldspace"), worldlight_position.x, worldlight_position.y, worldlight_position.z);
	worldlight_power = 70000.0f;
	glUniform1f(glGetUniformLocation(opengl_program_id, "LightPower"), worldlight_power);
	glUniform3f(glGetUniformLocation(opengl_program_id, "emissionValue"), 0.1f, 0.1f, 0.1f);
}

void SolarShadowApplication::initialize_simulation_parameters()
{
	dayhour = 0.0f;
	yearday = 0.0f;
	simulation_speed = 0.1f;
}

void SolarShadowApplication::create_orbit(float x_radius, float z_radius, GLuint& vertexarray_id, GLuint& vertexarraybuffer_id)
{
	glGenVertexArrays(1, &vertexarray_id);
	glBindVertexArray(vertexarray_id);
	GLfloat point_vertices_buffer[215883];
	unsigned int k = 0; 
	for (float a = 0; a < 360;) 
	{
		float x = 0;
		float y = 0;
		float z = 0;
		calculate_elliptic_coordinate(a, z_radius, x_radius, x, y, z);
		point_vertices_buffer[k] = x;
		k++; 
		point_vertices_buffer[k] = y;
		k++;
		point_vertices_buffer[k] = z;
		k++; 
		a += 0.005f;
	}
	glGenBuffers(1, &vertexarraybuffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertexarraybuffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point_vertices_buffer), point_vertices_buffer, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexarraybuffer_id);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);	
}

void SolarShadowApplication::send_mvp(mat4 &the_model, mat4 &the_view, mat4 &the_projection, GLuint the_program_id)
{
	mat4 mvp_matrix = the_projection * the_view * the_model;
	glUniformMatrix4fv(glGetUniformLocation(the_program_id, "MVP"), 1, GL_FALSE, &mvp_matrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(the_program_id, "M"), 1, GL_FALSE, &the_model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(the_program_id, "V"), 1, GL_FALSE, &the_view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(the_program_id, "P"), 1, GL_FALSE, &the_projection[0][0]);
}

void SolarShadowApplication::gl_loop_executions()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	mat4 model_base = mat4(1.0f);
	compute_input_controls(view);
	compute_rotation_speed(simulation_speed);

	glUniform3f(glGetUniformLocation(opengl_program_id, "emissionValue"), 0.3f, 0.3f, 0.3f);
	vec3 earth_scale;
	earth->get_scale_vector(earth_scale);
	model = model_base;
	float erde_umlaufbahn_multiplikator_sin = 300.0f; 
	float erde_umlaufbahn_multiplikator_cos = erde_umlaufbahn_multiplikator_sin * 1.03399f; 
	float kreiswinkel_erde_um_sonne = (360.0f * yearday / 365.0f);
	float bogenmass_erde_um_sonne = radians(kreiswinkel_erde_um_sonne);
	float trans_x_wert_erde = sin(bogenmass_erde_um_sonne) * erde_umlaufbahn_multiplikator_sin;
	float trans_z_wert_erde = cos(bogenmass_erde_um_sonne) * erde_umlaufbahn_multiplikator_cos;
	model = translate(model, vec3(trans_x_wert_erde, 0.0f, trans_z_wert_erde));
	model = scale(model, earth_scale);
	float kreiswinkel_erde = (360.0f * dayhour / 24.0f);
	model = rotate(model, kreiswinkel_erde, vec3(0.0f, 1.0f, 0.0f));
	send_mvp(model, view, projection, opengl_program_id);
	earth->draw();
	glUniform1i(glGetUniformLocation(opengl_program_id, "myTexture"), earth->get_texture_unit());

	put_stars();

	glUniform3f(glGetUniformLocation(opengl_program_id, "emissionValue"), 1.0f, 1.0f, 1.0f);
	vec3 sun_scale;
	sun->get_scale_vector(sun_scale);
	model = model_base;
	model = translate(model, vec3(0.0f, 0.0f, 0.0f));
	model = scale(model, sun_scale);
	float kreiswinkel_sonne = (360.0f * dayhour / 609.12f);
	model = rotate(model, kreiswinkel_sonne, vec3(0.0f, 1.0f, 0.0f));
	send_mvp(model, view, projection, opengl_program_id);
	sun->draw();
	glUniform1i(glGetUniformLocation(opengl_program_id, "myTexture"), sun->get_texture_unit());

	glUniform3f(glGetUniformLocation(opengl_program_id, "emissionValue"), 0.3f, 0.3f, 0.3f);
	vec3 moon_scale;
	moon->get_scale_vector(moon_scale);
	model = model_base;
	float mond_umlaufbahn_multiplikator_sin = 50.0f;
	float mond_umlaufbahn_multiplikator_cos = mond_umlaufbahn_multiplikator_sin * 1.11762f;
	float kreiswinkel_mond_um_erde = (360.0f * 12.0f * yearday / 365.0f); 
	float bogenmass_mond_um_erde = radians(kreiswinkel_mond_um_erde); 
	float trans_x_wert_mond = (sin(bogenmass_mond_um_erde) * mond_umlaufbahn_multiplikator_sin) + (sin(bogenmass_erde_um_sonne) * erde_umlaufbahn_multiplikator_sin);
	float trans_z_wert_mond = (cos(bogenmass_mond_um_erde) * mond_umlaufbahn_multiplikator_cos) + (cos(bogenmass_erde_um_sonne) * erde_umlaufbahn_multiplikator_cos);
	model = translate(model, vec3(trans_x_wert_mond, 0.0f, trans_z_wert_mond));
	model = scale(model, moon_scale);
	float kreiswinkel_mond = (360.0f * dayhour / 708.0f);
	model = rotate(model, kreiswinkel_mond, vec3(0.0f, 1.0f, 0.0f));
	send_mvp(model, view, projection, opengl_program_id);
	moon->draw();
	glUniform1i(glGetUniformLocation(opengl_program_id, "myTexture"), moon->get_texture_unit());

	glUniform3f(glGetUniformLocation(opengl_program_id, "emissionValue"), 0.3f, 0.3f, 0.3f);
	model = model_base;
	send_mvp(model, view, projection, opengl_program_id);
	draw_orbit(earth_orbit_vertex_id);
	model = model_base;
	model = translate(model, vec3(trans_x_wert_erde, 0.0f, trans_z_wert_erde));
	send_mvp(model, view, projection, opengl_program_id);
	draw_orbit(moon_orbit_vertex_id);
}

void SolarShadowApplication::calculate_elliptic_coordinate(float der_ellipsenwinkel, 
																													 float der_cos_multiplikator, 
																													 float der_sin_multiplikator, 
																													 float& trans_x_wert, 
																													 float& trans_y_wert, 
																													 float& trans_z_wert)
{
	float bogenmass = radians(der_ellipsenwinkel); 
	trans_x_wert = sin(bogenmass) * der_sin_multiplikator;
	trans_y_wert = 0;
	trans_z_wert = cos(bogenmass) * der_cos_multiplikator;
}

void SolarShadowApplication::compute_input_controls(mat4& view_mp)
{
	double xpos, ypos;
	glfwGetCursorPos(this->get_window(), &xpos, &ypos);
	int width, height;
	glfwGetWindowSize(this->get_window(), &width, &height);
	glfwSetCursorPos(this->get_window(), (width / 2), (height / 2));
	double currentTime = glfwGetTime();
	delta_time = float(currentTime - old_time_since_start);
	old_time_since_start = currentTime;
	horizontal_angle += (mouse_speed * delta_time * float((width / 2) - xpos));
	vertical_angle += (mouse_speed * delta_time * float((height / 2) - ypos));
	motion_direction = vec3((cos(vertical_angle) * sin(horizontal_angle)), sin(vertical_angle), (cos(vertical_angle) * cos(horizontal_angle)));
	right_vector = vec3(sin(horizontal_angle - (3.14f / 2.0f)), 0, cos(horizontal_angle - (3.14f / 2.0f)));
	vec3 up = cross(right_vector, motion_direction);

	view_mp = lookAt(camera_position, (camera_position + motion_direction), up);
}

void SolarShadowApplication::compute_rotation_speed(float& speed)
{
	dayhour += speed;
	yearday += (speed / 24.0f);
	dayhour = dayhour - ((int)(dayhour / 24)) * 24;
	yearday = yearday - ((int)(yearday / 365)) * 365;
}

void SolarShadowApplication::draw_orbit(GLuint& vertexarray_id)
{
	glBindVertexArray(vertexarray_id);
	glDrawArrays(GL_POINTS, 0, 215883);
	glBindVertexArray(0);
}

void SolarShadowApplication::put_stars()
{
	glUniform3f(glGetUniformLocation(opengl_program_id, "emissionValue"), 1000.0f, 1000.0f, 1000.0f);
	for(unsigned int i = 0; i < stars.size(); i++)
	{
		vec3 star_scale;
		stars[i]->get_scale_vector(star_scale);
		model = mat4(1.0f);
		vec3 star_position;
		stars[i]->get_3d_position_vector(star_position);
		model = translate(model, star_position);
		model = scale(model, star_scale);
		send_mvp(model, view, projection, opengl_program_id);
		stars[i]->draw();
	}
}

SolarShadowApplication::~SolarShadowApplication()
{
	if (earth != 0)
		delete earth;
	if (moon != 0)
		delete moon;
	if (sun != 0)
		delete sun;
	if (stars.size() != 0)
	{
		for (unsigned int i = 0; i < stars.size(); i++)
			delete stars[i];
	}
	if (earth_orbit_vertex_buffer_id != 0)
		glDeleteBuffers(1, &earth_orbit_vertex_buffer_id);
	if (moon_orbit_vertex_buffer_id != 0)
		glDeleteBuffers(1, &moon_orbit_vertex_buffer_id);
	glDeleteProgram(opengl_program_id);
}
