#pragma once
#include "cgmath.h"			// slee's simple math library
#include "cgut.h"			// slee's OpenGL utility
#include "func.h"
#include "constant.h"
class Sphere2 {
	GLuint	vertex_buffer_a = 0;	// ID holder for vertex buffer
	GLuint	index_buffer_a = 0;	// ID holder for index buffer
	std::vector<vertex>	vertex_list;
	std::vector<uint>	index_list;
public:
	float radius;
	vec3 position[24] = { 0 };
	vec3 dir[24] = { 0 };
	bool is_render = false;
	bool is_hit[24] = { 1,1,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1 ,1,1,1,1,1,1 };
	bool prev = false;
	int damage = 2;
	bool init() {
		for (int i = 0; i < 24; i++) {
			is_hit[i] = false;
		}
		return true;
	}
	bool update(vec2 ship_pos, float angle, bool cur) {
		if (prev != cur) {
			printf("init is called\n");
			prev = cur;
			init();
		}
		for (int i = 0; i < 24; i++) {
			if ((position[i] - vec3(ship_pos, 0.0f)).length() < 18.0f && !is_hit[i]) {
				hp -= int(radius * damage);
				is_hit[i] = true;
				sound_plays[HIT] = true;
				sound_updates[HIT] = true;
				plays_to_enemy[0] = true;
				updates_to_enemy[0] = true;
			}
		}
		return true;
	}
	bool render(GLuint program) {

		glUniform1i(glGetUniformLocation(program, "VertexShaderMode"), 1);
		glUniform1i(glGetUniformLocation(program, "FragmentShaderMode"), -2);
		if (vertex_buffer_a)	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_a);
		if (index_buffer_a)	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_a);
		for (int k = 0; k < NUM_SPHERE; k++) {
			if (is_hit[k]) continue;
			mat4 model_matrix =
				mat4::translate(position[k]) * mat4::scale(vec3(radius));
			glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
			cg_bind_vertex_attributes(program);
			glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_INT, nullptr);
		}
		return true;
	}
	Sphere2() {

		vertex_list.clear();

		float theta = 0.0f, phi = 0.0f;
		for (int k = 0; k <= 36; k++) {
			theta = PI / 36.0f * k;
			for (int j = 0; j <= 72; j++) {
				phi = PI / 36.0f * j;
				vertex_list.push_back({ vec3(sin(theta) * cos(phi),sin(theta) * sin(phi) ,cos(theta)),vec3(sin(theta) * cos(phi),sin(theta) * sin(phi),cos(theta)), vec2(phi / (2 * PI),1 - theta / PI) });
			}
		}

		if (vertex_buffer_a)	glDeleteBuffers(1, &vertex_buffer_a);	vertex_buffer_a = 0;
		if (index_buffer_a)	glDeleteBuffers(1, &index_buffer_a);	index_buffer_a = 0;

		// check exceptions
		if (vertex_list.empty()) { printf("[error] vertex_list is empty.\n"); return; }

		// create buffers
		index_list.clear();
		uint j = 0;
		for (uint k = 0; k < 73; k++) {
			index_list.push_back(k);
			index_list.push_back(k + 73);
			index_list.push_back(k + 1);
		}
		for (j = 1; j < 36; j++) {
			for (uint k = 0; k < 73; k++) {
				index_list.push_back(k + j * 73);
				index_list.push_back(k + 1 + j * 73);
				index_list.push_back(k - 72 + j * 73);
				index_list.push_back(k + j * 73);
				index_list.push_back(k + 73 + j * 73);
				index_list.push_back(k + 1 + j * 73);
			}
		}
		j = 36;
		for (uint k = 0; k < 73; k++) {
			index_list.push_back(k + j * 73);
			index_list.push_back(k + 1 + j * 73);
			index_list.push_back(k - 72 + j * 73);
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