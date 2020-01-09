#pragma once
#include "cgmath.h"			// slee's simple math library
#include "cgut.h"			// slee's OpenGL utility
#include "func.h"
#include "constant.h"

class Trap {
	GLuint	vertex_buffer = 0;	// ID holder for vertex buffer
	GLuint	index_buffer = 0;	// ID holder for index buffer
	std::vector<vertex>	vertex_list;
	std::vector<uint>	index_list;
	float duration[5] = { 0 };


public:
	vec3 position[5] = { 0 };
	float dir[5] = { 0 };
	vec2 size[5] = { 0 };
	bool available[5] = { 0 };
	int cur = 0;
	// index of rendering frames

	bool init(vec2 ship_pos, float angle, float glTime, float duration_,vec2 size_) {
		position[cur] = vec3(ship_pos, 8.0f);
		dir[cur] = -angle;
		available[cur] = true;
		duration[cur] = glTime + duration_;
		size[cur] = size_;
		cur = (cur + 1) % 5;
		return true;
	}
	bool update(float glTime) {
		for (int i = 0; i < 5; i++) {
			if (available[i]) {
				if (position[i].z > -2.0f) {
					position[i].z -= 0.2f;
				}
				if (glTime > duration[i]) available[i] = false;
			}
		}

		return true;
	}
	bool render(GLuint program) {

		glUniform1i(glGetUniformLocation(program, "VertexShaderMode"), 1);
		glUniform1i(glGetUniformLocation(program, "FragmentShaderMode"), SHIP_SHADER);
		glUniform4fv(glGetUniformLocation(program, "diffuse"), 1,vec4(0.5f,0.5f,0.8f,0));
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		for (int k = 0; k < 5; k++) {
			if (available[k]) {
				mat4 model_matrix = mat4::translate(position[k]) *mat4::rotate(vec3(0,0,1),dir[k])* mat4::scale(vec3(size[k],1.0f));
				glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
				cg_bind_vertex_attributes(program);
				glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_INT, nullptr);
			}
		}
		return true;
	}
	Trap() {

		vertex_list.clear();
		vertex_list.push_back({ vec3(-1.0f, -1.0f, 0.0f), vec3(0, 0, 1), vec2(0.0f, 0.0f) });
		vertex_list.push_back({ vec3(1.0f, -1.0f, 0.0f), vec3(0, 0, 1), vec2(1.0f, 0.0f) });
		vertex_list.push_back({ vec3(1.0f, 1.0f, 0.0f), vec3(0, 0, 1), vec2(1.0f, 1.0f) });
		vertex_list.push_back({ vec3(-1.0f, 1.0f, 0.0f), vec3(0, 0, 1), vec2(0.0f, 1.0f) });

		index_list.clear();
		index_list.push_back(0);
		index_list.push_back(1);
		index_list.push_back(2);

		index_list.push_back(0);
		index_list.push_back(2);
		index_list.push_back(3);
		
		/*if (vertex_buffer)	glDeleteBuffers(1, &vertex_buffer);	vertex_buffer = 0;
		if (index_buffer)	glDeleteBuffers(1, &index_buffer);	index_buffer = 0;*/
		
		glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertex_list.size(), &vertex_list[0], GL_STATIC_DRAW);

		// geneation of index buffer
		glGenBuffers(1, &index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * index_list.size(), &index_list[0], GL_STATIC_DRAW);

	}
};

