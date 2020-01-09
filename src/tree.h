#pragma once

#include "cgmath.h"			// slee's simple math library
#include "constant.h"
#include "cgut.h"			// slee's OpenGL utility
#include "func.h"
#include "assimp_loader.h"

class Tree{
	const char* mesh_obj = "../bin/mesh/tree/tree228.obj";	// OBJ
	const char* opacity = "../bin/mesh/tree/opacity.png";
	mesh2* pMesh = nullptr;
	GLuint texture_buffer = 0;
	mat4 base_model_matrix = mat4::scale(5.0f) * mat4::rotate(vec3(1, 0, 0), PI / 2);//mat4::scale(0.03f, 0.03f, 0.03f);
	mat4 model_matrix;
	vec3 position[TREE_NUM] = { 0 };
	float size[TREE_NUM] = { 0 };
	float rotate[TREE_NUM] = { 0 };
public:
	bool update(float gl_time) {
	}
	bool render(GLuint program) {

		glUniform1i(glGetUniformLocation(program, "VertexShaderMode"), 1);
		glUniform1i(glGetUniformLocation(program, "FragmentShaderMode"), TREE_SHADER);

		cg_bind_vertex_attributes_mesh(program, pMesh);
		for (int i = 0; i < TREE_NUM; i++) {
			model_matrix = mat4::translate(position[i])*mat4::scale(size[i]) * base_model_matrix;
			glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
			for (size_t k = 0, kn = pMesh->geometry_list.size(); k < kn; k++) {
				geometry& g = pMesh->geometry_list[k];
				glBindTexture(GL_TEXTURE_2D, g.mat->textures.diffuse->id);
				glUniform1i(glGetUniformLocation(program, "TEX"), 0);	 // GL_TEXTURE0


				glUniform1i(glGetUniformLocation(program, "alphaTexture"), 1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, texture_buffer);


				glUniform1i(glGetUniformLocation(program, "use_texture"), true);
				// render vertices: trigger shader programs to process vertex data
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->index_buffer);
				glDrawElements(GL_TRIANGLES, g.index_count, GL_UNSIGNED_INT, (GLvoid*)(g.index_start * sizeof(GLuint)));
			}
		}
		return true;
	}
	Tree() {
		pMesh = load_model(mesh_obj);
		if (pMesh == nullptr) { printf("Unable to load mesh\n"); }
		bind_texture_png(&texture_buffer, opacity, window_size);

		//initialize();
	}
	void initialize() {
	}

};