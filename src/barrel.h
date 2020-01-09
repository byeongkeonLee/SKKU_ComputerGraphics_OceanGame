#pragma once

#include "cgmath.h"			// slee's simple math library
#include "constant.h"
#include "cgut.h"			// slee's OpenGL utility
#include "func.h"
#include "assimp_loader.h"

class Barrel {
	const char* mesh_obj = "../bin/mesh/barrel/barrel.obj";	// OBJ
	const char* texture_path = "../bin/mesh/barrel/barile_tex.jpg";	// OBJ
	mesh2* pMesh = nullptr;
	GLuint texture_buffer = 0;
	mat4 base_model_matrix = mat4::scale(0.03f, 0.03f, 0.03f);
	mat4 model_matrix;
public:
	int p2barrel = 0;
	int p2completed = 0;
	vec3 position[BARREL_NUM] = { 0 };
	float height[BARREL_NUM] = { 0 };
	float rotate[BARREL_NUM] = { 0 };
	bool player2update() {
		if (SKILL_SET == 1) {
			if (p2completed < p2barrel) {
				p2completed = p2barrel;
				exper1++;
			}
		}
		return true;
	}
	bool update(float gl_time,vec2 pos1, vec2 pos2) {
		for (int i = 0; i < BARREL_NUM; i++) {
			if ((pos1 - vec2(position[i].x, position[i].y)).length() < 6.5f) {
				regen(i);
				exper1 += 1;
			}
			if ((pos2 - vec2(position[i].x, position[i].y)).length() < 6.5f) {
				regen(i);
				p2barrel++;
			}
		}
		for (int i = 0; i < BARREL_NUM; i++)
			position[i].z = sin(height[i] + gl_time);
		return true;
	}
	bool regen(int i) {
		position[i] = vec3((random()-0.5f) * WORLD_WIDTH, (random() - 0.5f) * WORLD_HEIGHT, 0.0f);
		height[i] = random() * 2 * PI;
		rotate[i] = random() * 2 * PI;
		return true;
	}
	bool render(GLuint program) {

		//cg_bind_vertex_attributes(program);

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(program, "VertexShaderMode"), 1);
		glUniform1i(glGetUniformLocation(program, "FragmentShaderMode"), 0);
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);
		glUniform1i(glGetUniformLocation(program, "use_texture"), true);

		cg_bind_vertex_attributes_mesh(program, pMesh);



		geometry& g = pMesh->geometry_list[0];
		glBindTexture(GL_TEXTURE_2D, texture_buffer);
		// render vertices: trigger shader programs to process vertex data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->index_buffer);

		for (int i = 0; i < BARREL_NUM; i++) {
			model_matrix = mat4::translate(position[i]) * mat4::rotate(vec3(0, 0, 1), rotate[i]) * base_model_matrix;
			glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
			glDrawElements(GL_TRIANGLES, g.index_count, GL_UNSIGNED_INT, (GLvoid*)(g.index_start * sizeof(GLuint)));
		}

		return true;
	}
	Barrel() {
		pMesh = load_model(mesh_obj);
		if (pMesh == nullptr) { printf("Unable to load mesh\n"); }
		bind_texture(&texture_buffer, texture_path, window_size);

		initialize();
	}
	void initialize() {
		for (int i = 0; i < BARREL_NUM; i++) {
			regen(i);
		}
	}

};