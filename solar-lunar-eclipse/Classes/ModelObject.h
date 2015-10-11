#ifndef ModelObject_H
#define	ModelObject_H

#include <vector>
#include <GLM\glm.hpp>


class ModelObject
{
	private:
		GLuint vertex_array_id;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals; 
		GLuint vertexbuffer_id;
		GLuint uvbuffer_id;
		GLuint normalbuffer_id;
		GLuint texture_id;
		unsigned int texture_unit;
		glm::vec3 scale_vector;
		glm::vec3 position;

	public:
		ModelObject(const std::string&);
		ModelObject(const std::string&, const glm::vec3&);
		ModelObject(const std::string&, const std::string&, const unsigned int);
		ModelObject(const std::string&, const glm::vec3&, const std::string&, const unsigned int);
		virtual ~ModelObject();
		void draw();
		void set_scale(const double, const double, const double);
		GLuint get_vertex_array_id();
		void get_vertices(std::vector<glm::vec3>&);
		void get_uvs(std::vector<glm::vec2>&);
		void get_normals(std::vector<glm::vec3>&);
		GLuint get_vertexbuffer_id();
		GLuint get_uvbuffer_id();
		GLuint get_normalbuffer_id();
		GLuint get_texture_id();
		unsigned int get_texture_unit();
		void get_scale_vector(glm::vec3&);
		void set_3d_position(float, float, float);
		void get_3d_position_vector(glm::vec3&);

	private:
		void create_modelobject(const std::vector<glm::vec3>&, const std::vector<glm::vec2>&, const std::vector<glm::vec3>&);
};

#endif