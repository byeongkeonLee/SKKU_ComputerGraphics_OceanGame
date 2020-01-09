#pragma once
#include "cgmath.h"			// slee's simple math library
#include "cgut.h"			// slee's OpenGL utility
#include "func.h"
#include "constant.h"

class End_image {
	std::vector<vertex>	vertex_list;	// host-side vertices
	std::vector<uint>	index_list;		// host-side indices
	GLuint	vertex_buffer = 0;	// ID holder for vertex buffer
	GLuint	index_buffer = 0;	// ID holder for index buffer
	GLuint	texture_buffer = 0;
	GLuint	texture_buffer2 = 0;
	const char* texture_path = "../bin/images/depeat.png";
	const char* texture_path2 = "../bin/images/victory.png";
public:
	bool update(float glTime) {

		return true;
	}
	bool render(GLuint program, bool victory) {
		glUniform1i(glGetUniformLocation(program, "VertexShaderMode"), 0);
		glUniform1i(glGetUniformLocation(program, "FragmentShaderMode"), 0);
		glUniform1i(glGetUniformLocation(program, "use_texture"), 1);

		glUniform2fv(glGetUniformLocation(program, "guiPosition"), 1, vec2(0, 0));
		if (vertex_buffer)	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		if (index_buffer)	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);
		glActiveTexture(GL_TEXTURE0);
		if(!victory)
			glBindTexture(GL_TEXTURE_2D, texture_buffer);
		else {
			glBindTexture(GL_TEXTURE_2D, texture_buffer2);
		}
		cg_bind_vertex_attributes(program);
		glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_INT, nullptr);

		return true;
	}
	End_image() {

		vertex_list.clear();
		vertex_list.push_back({ vec3(-0.6f, -0.8f, 0.0f), vec3(0, 0, 1), vec2(0.0f, 0.0f) });
		vertex_list.push_back({ vec3(0.6f, -0.8f, 0.0f), vec3(0, 0, 1), vec2(1.0f, 0.0f) });
		vertex_list.push_back({ vec3(0.6f, 0.8f, 0.0f), vec3(0, 0, 1), vec2(1.0f, 1.0f) });
		vertex_list.push_back({ vec3(-0.6f, 0.8f, 0.0f), vec3(0, 0, 1), vec2(0.0f, 1.0f) });



		index_list.clear();

		index_list.push_back(0);
		index_list.push_back(1);
		index_list.push_back(2);

		index_list.push_back(0);
		index_list.push_back(2);
		index_list.push_back(3);

		if (vertex_buffer)	glDeleteBuffers(1, &vertex_buffer);	vertex_buffer = 0;
		if (index_buffer)	glDeleteBuffers(1, &index_buffer);	index_buffer = 0;

		glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertex_list.size(), &vertex_list[0], GL_STATIC_DRAW);

		// geneation of index buffer
		glGenBuffers(1, &index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * index_list.size(), &index_list[0], GL_STATIC_DRAW);

		bind_texture_png(&texture_buffer, texture_path, window_size);
		bind_texture_png(&texture_buffer2, texture_path2, window_size);
	}
};