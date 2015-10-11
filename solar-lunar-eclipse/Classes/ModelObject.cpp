#include <windows.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <GL/glew.h>
#include <GL/gl.h>
#include "ApplicationHelper.h"
#include "ModelObject.h"

using namespace std;
using namespace glm;

ModelObject::ModelObject(const string& obj_path)
{
	if (!obj_path.empty())
	{
		ApplicationHelper::load_objectfile(obj_path, vertices, uvs, normals);
		if (!vertices.empty() && !uvs.empty() && !normals.empty())
		{
			create_modelobject(vertices, uvs, normals);
			texture_id = 0;
			texture_unit = 0;
			scale_vector = vec3(1.0, 1.0, 1.0);
			position = vec3(0.0f, 0.0f, 0.0f);
		}
		else
			throw invalid_argument("The read data as one of vertices, uvs or normals of obj-file is empty.");
	}
	else
		throw invalid_argument("The passed path is empty.");
}

ModelObject::ModelObject(const string& obj_path, const vec3& scale)
{
	if (!obj_path.empty() && (scale.length() != 0))
	{
		ApplicationHelper::load_objectfile(obj_path, vertices, uvs, normals);
		if (!vertices.empty() && !uvs.empty() && !normals.empty())
		{
			create_modelobject(vertices, uvs, normals);
			texture_id = 0;
			texture_unit = 0;
			scale_vector = scale;
			position = vec3(0.0f, 0.0f, 0.0f);
		}
		else
			throw invalid_argument("The read data as one of vertices, uvs or normals of obj-file is empty.");
	}
	else
		throw invalid_argument("The passed path is empty.");
}

ModelObject::ModelObject(const string& obj_path, const string& texture_path, const unsigned int tex_unit)
{
	if (!obj_path.empty() && !texture_path.empty())
	{
		ApplicationHelper::load_objectfile(obj_path, vertices, uvs, normals);
		if (!vertices.empty() && !uvs.empty() && !normals.empty())
		{
			create_modelobject(vertices, uvs, normals);
			texture_id = ApplicationHelper::load_bmp_texture(texture_path, tex_unit);
			texture_unit = tex_unit;
			scale_vector = vec3(1.0, 1.0, 1.0);
			position = vec3(0.0f, 0.0f, 0.0f);
		}
		else
			throw invalid_argument("The read data as one of vertices, uvs or normals of obj-file is empty.");
	}
	else
		throw invalid_argument("The passed path of obj or texture is empty.");
}

ModelObject::ModelObject(const string& obj_path, const vec3& scale, const string& texture_path, const unsigned int tex_unit)
{
	if (!obj_path.empty() && !texture_path.empty() && (scale.length() != 0))
	{
		ApplicationHelper::load_objectfile(obj_path, vertices, uvs, normals);
		if (!vertices.empty() && !uvs.empty() && !normals.empty())
		{
			create_modelobject(vertices, uvs, normals);
			texture_id = ApplicationHelper::load_bmp_texture(texture_path, tex_unit);
			texture_unit = tex_unit;
			scale_vector = scale;
			position = vec3(0.0f, 0.0f, 0.0f);
		}
		else
			throw invalid_argument("The read data as one of vertices, uvs or normals of obj-file is empty.");
	}
	else
		throw invalid_argument("The passed path of obj or texture or the passed scale is empty.");
}

void ModelObject::draw()
{
	glBindVertexArray(vertex_array_id);
	glDrawArrays(GL_LINES, 0, vertices.size());
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	if (texture_id > ((GLuint)0))
	{
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glActiveTexture(GL_TEXTURE0 + texture_unit);
	}
	glBindVertexArray(0);
}

void ModelObject::set_scale(const double x, const double y, const double z)
{
	scale_vector = vec3(x, y, z);
}

void ModelObject::create_modelobject(const vector<vec3>& the_vertices, const vector<vec2>& the_uvs, const vector<vec3>& the_normals)
{
	glGenVertexArrays(1, &vertex_array_id);
	glBindVertexArray(vertex_array_id);
	
	glGenBuffers(1, &vertexbuffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_id);
	glBufferData(GL_ARRAY_BUFFER, the_vertices.size() * sizeof(glm::vec3), &the_vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	glGenBuffers(1, &normalbuffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer_id);
	glBufferData(GL_ARRAY_BUFFER, the_normals.size() * sizeof(glm::vec3), &the_normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &uvbuffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_id);
	glBufferData(GL_ARRAY_BUFFER, the_uvs.size() * sizeof(glm::vec2), &the_uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);	
	glBindVertexArray(0);
}

GLuint ModelObject::get_vertex_array_id()
{
	return vertex_array_id;
}

void ModelObject::get_vertices(vector<vec3>& vertices_copy)
{
	vertices_copy.swap(vertices);
}

void ModelObject::get_normals(vector<vec3>& normals_copy)
{
	normals_copy.swap(normals);
}

void ModelObject::get_uvs(vector<vec2>& uvs_copy)
{
	uvs_copy.swap(uvs);
}

GLuint ModelObject::get_vertexbuffer_id()
{
	return vertexbuffer_id;
}

GLuint ModelObject::get_normalbuffer_id()
{
	return normalbuffer_id;
}

GLuint ModelObject::get_uvbuffer_id()
{
	return uvbuffer_id;
}

GLuint ModelObject::get_texture_id()
{
	return texture_id;
}

unsigned int ModelObject::get_texture_unit()
{
	return texture_unit;
}

void ModelObject::get_scale_vector(vec3& scale_copy)
{
	scale_copy.x = scale_vector.x;
	scale_copy.y = scale_vector.y;
	scale_copy.z = scale_vector.z;
}

void ModelObject::set_3d_position(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void ModelObject::get_3d_position_vector(vec3& position_copy)
{
	position_copy.x = position.x;
	position_copy.y = position.y;
	position_copy.z = position.z;
}

ModelObject::~ModelObject()
{
	glDeleteBuffers(1, &vertexbuffer_id);
	glDeleteBuffers(1, &normalbuffer_id);
	glDeleteBuffers(1, &uvbuffer_id);
	if (texture_id > ((GLuint)0))
		glDeleteTextures(1, &texture_id);	
}