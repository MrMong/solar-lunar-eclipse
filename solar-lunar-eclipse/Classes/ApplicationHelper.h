#ifndef ApplicationHelper_H
#define	ApplicationHelper_H

#include <string.h>
#include <vector>
#include <GL/glew.h>
#include <GLM/glm.hpp>

class ApplicationHelper
{
	public:
		static GLuint load_shaders(const std::string&, const std::string&);
		static void load_objectfile(const std::string&, std::vector<glm::vec3>&, std::vector<glm::vec2>&, std::vector<glm::vec3>&);
		static GLuint load_bmp_texture(const std::string&, unsigned int);

	private:
		ApplicationHelper();
		~ApplicationHelper();
		static void read_shaderfile(const std::string&, std::string&);
		static void compile_shader(const std::string&, const GLuint);
		static bool check_shader(const GLuint, std::vector<char>&);
		static GLuint link_program(const GLuint, GLuint);
		static bool check_program(const GLuint, std::vector<char>&);
		static void read_objectfile(const std::string&, std::string&);

};

#endif

