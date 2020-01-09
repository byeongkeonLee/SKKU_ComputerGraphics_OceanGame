#pragma once
#include "cgmath.h"			// slee's simple math library
#include "cgut.h"			// slee's OpenGL utility
#include "func.h"
#include "constant.h"
#include "assimp_loader.h"
#include "camera.h"
#include "skill_set.h"

class Ship2 {
public:
	const char* mesh_obj = "../bin/mesh/ship2/Boat.obj";	// OBJ
	//const char* texture_path = "../bin/mesh/ship/boat.001.jpg";
	mesh2* pMesh = nullptr;
	GLuint texture_buffer = 0;
	mat4 base_model_matrix = mat4::rotate(vec3(0, 0, 1), PI) * mat4::rotate(vec3(1, 0, 0), PI / 2) * mat4::scale(0.005f);
	mat4 model_matrix;

	float acc = 0.003f;
	float distance_z = 1.0f;
	vec2 dir = vec2(0, 1.0f);
	vec2 speed = vec2(0, 0);
	vec2 distance = vec2(0, 0);
	vec2 total_speed = vec2(0, 0);
	float additional_speed = 0.0f;
	float max_speed = 3.0f;
	float fraction = 0.992f;
	float angle_acc = 0.00004f;
	float angle_speed = 0.0f;
	float angle = 0.0f;

	bool update(float glTime) {
		dir = vec2(sin(angle), cos(angle));
		model_matrix = mat4::translate(vec3(distance.x, distance.y, distance_z)) * mat4::rotate(vec3(0, 0, 1), -angle) * base_model_matrix;
		return true;
	}
	bool render(GLuint program) {

		//cg_bind_vertex_attributes(program);
		glUniform1i(glGetUniformLocation(program, "VertexShaderMode"), 1);
		glUniform1i(glGetUniformLocation(program, "FragmentShaderMode"), SHIP_SHADER);
		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
		glUniform1f(glGetUniformLocation(program, "use_texture"), false);

		float alpha = (SKILL_SET != 0 && e_1->turn_on) ? e_1->alpha : 1.0f;
		glUniform1f(glGetUniformLocation(program, "ship_alpha"), e_1->turn_on ? alpha : 1.0f);
		cg_bind_vertex_attributes_mesh(program, pMesh);
		for (size_t k = 0, kn = pMesh->geometry_list.size(); k < kn; k++) {
			geometry& g = pMesh->geometry_list[k];
			glUniform4fv(glGetUniformLocation(program, "diffuse"), 1, (const float*)(&g.mat->diffuse));
			// render vertices: trigger shader programs to process vertex data
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->index_buffer);
			glDrawElements(GL_TRIANGLES, g.index_count, GL_UNSIGNED_INT, (GLvoid*)(g.index_start * sizeof(GLuint)));
		}
		return true;
	}
	Ship2() {
		pMesh = load_model(mesh_obj);
		if (pMesh == nullptr) { printf("Unable to load mesh\n"); }
		//bind_texture(&texture_buffer, texture_path, window_size);
	}
};

Ship2* ship2;