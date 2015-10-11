#include <stdio.h>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <direct.h>

#include "ApplicationHelper.h"

using namespace std;
using namespace glm;

ApplicationHelper::ApplicationHelper()
{
}

ApplicationHelper::~ApplicationHelper()
{
}

GLuint ApplicationHelper::load_bmp_texture(const string& bmp_path, unsigned int texture_unit)
{
	char *imagepath = new char[bmp_path.length() + 1];
	strcpy(imagepath, bmp_path.c_str());

	printf("Reading image %s\n", imagepath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath,"rb");
	if (!file)
	{
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); 
		return 0;
	}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if ( fread(header, 1, 54, file)!=54 ){ 
		printf("Not a correct BMP file\n");
		return 0;
	}
	// A BMP files always begins with "BM"
	if ( header[0]!='B' || header[1]!='M' ){
		printf("Not a correct BMP file\n");
		return 0;
	}
	// Make sure this is a 24bpp file
	if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    return 0;}
	if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    return 0;}

	// Read the information about the image
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char [imageSize];

	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);

	// Everything is in memory now, the file wan be closed
	fclose (file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// Texture-Unit aktivieren, mit welcher Texture verbunden werden soll
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// OpenGL has now copied the data. Free our own version
	delete [] data;

	// ... nice trilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	glGenerateMipmap(GL_TEXTURE_2D);

	delete[] imagepath;

	// Return the ID of the texture we just created
	return textureID;
}

void ApplicationHelper::load_objectfile(const string& path, vector<vec3>& out_vertices, vector<vec2>& out_uvs, vector<vec3>& out_normals)
{
	printf("Loading OBJ file %s...\n", path);

	vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	vector<vec3> temp_vertices; 
	vector<vec2> temp_uvs;
	vector<vec3> temp_normals;

	string objectfile_code;
	read_objectfile(path, objectfile_code);

	if (!objectfile_code.empty())
	{
		vector<string> line_strings;
		string::size_type pos = 0;
		string::size_type prev = 0;
		while ((pos = objectfile_code.find("\n", prev)) != string::npos)
		{
			line_strings.push_back(objectfile_code.substr(prev, pos - prev));
			prev = pos + 1;
		}
		line_strings.push_back(objectfile_code.substr(prev));

		for(vector<int>::size_type i = 0; i < line_strings.size(); i++) 
		{
			char *c_line_string = new char[line_strings[i].length() +1];
			strcpy(c_line_string, line_strings[i].c_str());
			string line_header = line_strings[i].substr(0, 128);
			char *c_line_header = new char[line_header.length() +1];
			strcpy(c_line_header, line_header.c_str());
			int result = sscanf(c_line_string, "%s", c_line_header);
			if (strcmp(c_line_header, "v") == 0)
			{
				vec3 vertex;
				char str[5];
				sscanf(c_line_string, "%s %f %f %f", str, &vertex.x, &vertex.y, &vertex.z );
				temp_vertices.push_back(vertex);
			}
			else if (strcmp(c_line_header, "vt") == 0)
			{
				vec2 uv;
				char str[5];
				sscanf(c_line_string, "%s %f %f", str, &uv.x, &uv.y );
				uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
				temp_uvs.push_back(uv);
			}
			else if (strcmp(c_line_header, "vn") == 0)
			{
				vec3 normal;
				char str[5];
				sscanf(c_line_string, "%s %f %f %f", str, &normal.x, &normal.y, &normal.z );
				temp_normals.push_back(normal);
			}
			else if (strcmp(c_line_header, "f") == 0)
			{
				string vertex1, vertex2, vertex3;
				char str[5];
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = sscanf(c_line_string, "%s %d/%d/%d %d/%d/%d %d/%d/%d", str, &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9)
				{
					if (matches == 1)
					{
						matches = sscanf(c_line_string, "%d %d", &vertexIndex[1], &vertexIndex[2]);
						if (matches != 2)
						{
							throw new runtime_error("File can't be read by our simple parser :-( Try exporting with other options\n"); 
							//printf("File can't be read by our simple parser (%d matches) :-( Try exporting with other options\n", matches);
						}
						else
							uvIndex[0] = uvIndex[1] = uvIndex[2] = normalIndex[0] = normalIndex[1] = normalIndex[2] = 0;
					}
				}
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices    .push_back(uvIndex[0]);
				uvIndices    .push_back(uvIndex[1]);
				uvIndices    .push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
			delete[] c_line_header;
			delete[] c_line_string;
		}
	}

	// For each vertex of each triangle
	for(unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		vec3 vertex = temp_vertices[ vertexIndex-1 ];

		vec2 uv = uvIndex ? temp_uvs[ uvIndex-1 ] : vec2(0.0, 0.0);
		
		vec3 normal = normalIndex ? temp_normals[ normalIndex-1 ] : vec3(0.0, 0.0, 0.0);

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs     .push_back(uv);
		out_normals .push_back(normal);
	}
}

void ApplicationHelper::read_objectfile(const string& path, string& objectfile_code)
{
	ifstream objectfile_stream(path, ios::in);
	if(objectfile_stream.is_open())
	{
		string line = "";
		while(getline(objectfile_stream, line))
			objectfile_code += "\n" + line;
		objectfile_stream.close();
	}
	else
		throw new runtime_error("Impossible to open " + path + " Are you in the right directory ? Don't forget to read the FAQ !\n"); 
}

GLuint ApplicationHelper::load_shaders(const string& vertexshader_path, const string& fragmentshader_path)
{
	GLuint vertexshader_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentshader_id = glCreateShader(GL_FRAGMENT_SHADER);
	
	string vertex_code;
	read_shaderfile(vertexshader_path, vertex_code);
	string fragment_code;
	read_shaderfile(fragmentshader_path, fragment_code);

	bool is_ok = false;
	vector<char> error_messages;

	compile_shader(vertex_code, vertexshader_id);
	is_ok = check_shader(vertexshader_id, error_messages);
	if (!is_ok)
	{
		printf("%s\n", &error_messages[0]);
		throw new runtime_error("VertexShader Problem!");
	}
	compile_shader(fragment_code, fragmentshader_id);
	is_ok = check_shader(fragmentshader_id, error_messages);
	if (!is_ok)
	{
		printf("%s\n", &error_messages[0]);
		throw new runtime_error("FragmentShader Problem!");
	}

	GLuint program_id = link_program(vertexshader_id, fragmentshader_id);
	is_ok = check_program(program_id, error_messages);
	if (!is_ok)
	{
		printf("%s\n", &error_messages[0]);
		throw new runtime_error("Problem beim Linken der Shader für OpenGL-Programm!");
	}

	glDeleteShader(vertexshader_id);
	glDeleteShader(fragmentshader_id);

	return program_id;
}

void ApplicationHelper::read_shaderfile(const string& path, string& shader_code)
{
	ifstream shader_stream(path, ios::in);
	if(shader_stream.is_open())
	{
		string line = "";
		while(getline(shader_stream, line))
			shader_code += "\n" + line;
		shader_stream.close();
	}
	else
		throw new runtime_error("Impossible to open " + path + " Are you in the right directory ? Don't forget to read the FAQ !\n"); 
}

void ApplicationHelper::compile_shader(const string& shader_code, const GLuint shader_id)
{
	char const * source_pointer = shader_code.c_str();
	glShaderSource(shader_id, 1, &source_pointer, NULL);
	glCompileShader(shader_id);	
}

bool ApplicationHelper::check_shader(const GLuint shader_id, vector<char>& error_messages)
{
	bool is_ok = true;
	GLint result = GL_FALSE;
	int infolog_length = 0;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &infolog_length);
	if (infolog_length > 0)
	{
		vector<char> temp_vector(infolog_length + 1);
		glGetShaderInfoLog(shader_id, infolog_length, NULL, &temp_vector[0]);
		if (temp_vector[0] != '\0')
		{
			error_messages.swap(temp_vector);
			is_ok = false;
		}
	}
	return is_ok;
}

GLuint ApplicationHelper::link_program(const GLuint vertexshader_id, const GLuint fragmentshader_id)
{
	GLuint program_id = glCreateProgram();
	glAttachShader(program_id, vertexshader_id);
	glAttachShader(program_id, fragmentshader_id);
	glLinkProgram(program_id);
	return program_id;
}

bool ApplicationHelper::check_program(const GLuint program_id, vector<char>& error_messages)
{
	bool is_ok = true;
	GLint result = GL_FALSE;
	int infolog_length = 0;
	glGetProgramiv(program_id, GL_LINK_STATUS, &result);
	glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &infolog_length);
	if (infolog_length > 0)
	{
		vector<char> temp_vector(infolog_length + 1);
		glGetProgramInfoLog(program_id, infolog_length, NULL, &temp_vector[0]);
		if (temp_vector[0] != '\0')
		{
			error_messages.swap(temp_vector);
			is_ok = false;
		}
	}	
	return is_ok;
}
