#pragma once
#include "cgmath.h"			// slee's simple math library
#include "cgut.h"			// slee's OpenGL utility
#include "func.h"
#include "constant.h"
#include "assimp_loader.h"
#include "camera.h"
#include "skill_set.h"

class Zombie {
public:
	const char* mesh_obj = "../bin/mesh/ship2/Boat.obj";	// OBJ
	//const char* texture_path = "../bin/mesh/ship/boat.001.jpg";
	mesh2* pMesh = nullptr;
	GLuint texture_buffer = 0;
	mat4 base_model_matrix = mat4::rotate(vec3(0, 0, 1), PI) * mat4::rotate(vec3(1, 0, 0), PI / 2) * mat4::scale(0.005f);

	int cur = 0;
	vec3 position[ZOMBIE_NUM];
	mat4 model_matrix[ZOMBIE_NUM];
	float angle[ZOMBIE_NUM];

	bool add_zombie(vec3 pos, float ang) {
		position[cur] = pos;
		angle[cur] = ang;
		model_matrix[cur] = mat4::translate(position[cur]) * mat4::rotate(vec3(0, 0, 1), -angle[cur]) * base_model_matrix;
		cur++;
		return true;
	}
	bool update(float glTime) {
		for (int i = 0; i < cur; i++) {
			if (terrain->getHeight_ship(vec2(position[i].x, position[i].y)) < position[i].z) {
				position[i].z -= 0.07f;
				model_matrix[i] = mat4::translate(position[i]) * mat4::rotate(vec3(0, 0, 1), -angle[i]) * base_model_matrix;
			}
		}
		return true;
	}
	bool render(GLuint program) {

		//cg_bind_vertex_attributes(program);
		glUniform1i(glGetUniformLocation(program, "VertexShaderMode"), 1);
		glUniform1i(glGetUniformLocation(program, "FragmentShaderMode"), SHIP_SHADER);
		
		glUniform1f(glGetUniformLocation(program, "use_texture"), false);
		cg_bind_vertex_attributes_mesh(program, pMesh);

		for (int i = 0; i < cur; i++) {
			glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix[i]);
			for (size_t k = 0, kn = pMesh->geometry_list.size(); k < kn; k++) {
				geometry& g = pMesh->geometry_list[k];
				glUniform4fv(glGetUniformLocation(program, "diffuse"), 1, (const float*)(&g.mat->diffuse));
				// render vertices: trigger shader programs to process vertex data
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->index_buffer);
				glDrawElements(GL_TRIANGLES, g.index_count, GL_UNSIGNED_INT, (GLvoid*)(g.index_start * sizeof(GLuint)));
			}
		}
		return true;
	}
	Zombie() {
		pMesh = load_model(mesh_obj);
		if (pMesh == nullptr) { printf("Unable to load mesh\n"); }
		//bind_texture(&texture_buffer, texture_path, window_size);
	}
};