#pragma once

#include "cgmath.h"			// slee's simple math library
#include "constant.h"
#include "cgut.h"			// slee's OpenGL utility
#include "func.h"
#include "assimp_loader.h"

class Chain {
	const char* mesh_obj = "../bin/mesh/anchor/Anchor.obj";	// OBJ
	//const char* texture_path = "../bin/mesh/barrel/barile_tex.jpg";	// OBJ
	mesh2* pMesh = nullptr;
	mat4 base_model_matrix =  mat4::rotate(vec3(0, 0, 1), PI) * mat4::rotate(vec3(0,1,0),PI/2)*mat4::scale(0.1f);
	mat4 model_matrix= base_model_matrix;
public:
	float angle,angle2;
	bool update(float glTime) {
		if (!q_1->hit_client) {
			q_1->position += q_1->dir * q_1->launch_spd + q_1->ship_spd;
		}
		if (q_1->hit_client && !q_1->hit_server) {
			q_1->hit_server = true;
			q_1->hit_run_time_end = glTime + q_1->hit_run_time;
		}
		if (q_1->hit_server && q_1->hit_run_time_end < glTime) {
			q_1->turn_on = false;
			q_1->hit_server = q_1->hit_client = false;
		}
		angle = acos(dot(normalize(q_1->dir), vec2(0, 1.0f)));
		angle2 = glTime;
		if (q_1->dir.x > 0) angle *= -1;
		if (distance(q_1->position, q_1->init_position) > q_1->range) q_1->turn_on = false;
		return true;
	}
	bool render(GLuint program) {
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);

		glUniform1i(glGetUniformLocation(program, "VertexShaderMode"), 1);
		glUniform1i(glGetUniformLocation(program, "FragmentShaderMode"), -2);
		model_matrix = mat4::translate(vec3(q_1->position, 2.0f)) * mat4::rotate(vec3(0, 0, 1), angle) * mat4::rotate(vec3(0, 1, 0), angle2) * base_model_matrix;
		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
		cg_bind_vertex_attributes_mesh(program, pMesh);
		for (size_t k = 0, kn = pMesh->geometry_list.size(); k < kn; k++) {
			geometry& g = pMesh->geometry_list[k];
			glUniform4fv(glGetUniformLocation(program, "diffuse"), 1, (const float*)(&g.mat->diffuse));
			glUniform1i(glGetUniformLocation(program, "use_texture"), false);
			// render vertices: trigger shader programs to process vertex data
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->index_buffer);
			glDrawElements(GL_TRIANGLES, g.index_count, GL_UNSIGNED_INT, (GLvoid*)(g.index_start * sizeof(GLuint)));
		}
		return true;
	}
	Chain() {
		pMesh = load_model(mesh_obj);
		if (pMesh == nullptr) { printf("Unable to load mesh\n"); }
	}

};