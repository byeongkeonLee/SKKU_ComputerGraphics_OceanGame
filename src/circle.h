#pragma once
#include "cgmath.h"			// slee's simple math library
#include "cgut.h"			// slee's OpenGL utility
#include "func.h"
#include "constant.h"
class Circle {
	GLuint	vertex_buffer_a = 0;	// ID holder for vertex buffer
	GLuint	index_buffer_a = 0;	// ID holder for index buffer
	std::vector<vertex>	vertex_list;
	std::vector<uint>	index_list;
	mat4 model_matrix[2] ;
public:
	bool update(vec2 ship_position, float angle,float range) {
		model_matrix[0] = mat4::translate(vec3(ship_position,2.0f)) * mat4::rotate(vec3(0,0,1),3*PI/4-angle) * mat4::scale(range,range, 1.0f);
		model_matrix[1] = mat4::translate(vec3(ship_position, 2.0f)) * mat4::rotate(vec3(0, 0, 1), PI*7 / 4 -angle ) * mat4::scale(range,range, 1.0f);
		return true;
	}
	bool render(GLuint program) {

		glUniform1i(glGetUniformLocation(program, "VertexShaderMode"), 1);
		glUniform1i(glGetUniformLocation(program, "FragmentShaderMode"), SHIP_SHADER);
		glUniform1f(glGetUniformLocation(program, "ship_alpha"), 0.5f);
		glUniform4fv(glGetUniformLocation(program, "diffuse"), 1,vec4(1.0f,0.2f,0.2f,0.0f));
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_a);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_a);
		for (int k = 0; k < 2; k++) {	
			glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix[k]);
			cg_bind_vertex_attributes(program);
			glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_INT, nullptr);
		}
		return true;
	}
	Circle() {
		vertex_list.clear();

		float theta = 0.0f;
		vertex_list.push_back({ vec3(0,0,2.0f),vec3(0,0,1),vec2(0,0) });
		for (int k = 0; k <= 36; k++) {
			theta = PI / 72.0f * k;
			vertex_list.push_back({vec3(cos(theta),sin(theta),2.0f), vec3(0,0,1), vec2(0,0)});
		}

		if (vertex_buffer_a)	glDeleteBuffers(1, &vertex_buffer_a);	vertex_buffer_a = 0;
		if (index_buffer_a)	glDeleteBuffers(1, &index_buffer_a);	index_buffer_a = 0;

		// check exceptions
		if (vertex_list.empty()) { printf("[error] vertex_list is empty.\n"); return; }

		// create buffers
		index_list.clear();
		uint j = 0;
		for (uint k = 1; k <= 36; k++) {
			index_list.push_back(0);
			index_list.push_back(k);
			index_list.push_back(k+1);
		}
		

		// generation of vertex buffer: use vertex_list as it is
		glGenBuffers(1, &vertex_buffer_a);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_a);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertex_list.size(), &vertex_list[0], GL_STATIC_DRAW);

		// geneation of index buffer
		glGenBuffers(1, &index_buffer_a);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_a);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * index_list.size(), &index_list[0], GL_STATIC_DRAW);
	}
};