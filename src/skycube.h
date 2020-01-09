#pragma once
#include "cgmath.h"			// slee's simple math library
#include "constant.h"
#include "cgut.h"			// slee's OpenGL utility
#include "func.h"

class Skycube {
private:
	GLuint vertex_buffer[6];
	GLuint index_buffer[6];
	GLuint texture_buffer[6] = { 0 };


	const char* path[6] = { "../bin/skybox/bottom.jpg","../bin/skybox/left.jpg","../bin/skybox/right.jpg","../bin/skybox/top.jpg","../bin/skybox/back.jpg", "../bin/skybox/front.jpg" };

	//const char* path[6] = { "../bin/skybox/bottom.jpg","../bin/skybox/bottom.jpg","../bin/skybox/bottom.jpg","../bin/skybox/top.jpg","../bin/skybox/bottom.jpg","../bin/skybox/back.jpg" };
	std::vector<vertex>	vertex_list[6];
	std::vector<uint>	index_list[6];

	const float size = 2000.0f;

public:
	bool render(GLuint program) {
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(program, "use_texture"), true);
		for (int i = 0; i < 6; i++) {
			glBindTexture(GL_TEXTURE_2D, texture_buffer[i]);
			if (vertex_buffer)	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[i]);
			if (index_buffer)	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer[i]);
			mat4 model_matrix = mat4::identity();
			glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
			glUniform1i(glGetUniformLocation(program, "VertexShaderMode"), SKYBOX_SHADE);
			glUniform1i(glGetUniformLocation(program, "FragmentShaderMode"), 0);
			cg_bind_vertex_attributes(program);
			glDrawElements(GL_TRIANGLES, index_list[i].size(), GL_UNSIGNED_INT, nullptr);
		}
		return true;
	}
	Skycube() {
		//bottom
		vertex_list[0].push_back({ vec3(-size,  size, -size),vec3(0,0,-1),vec2(1.0f,0.0f) });
		vertex_list[0].push_back({ vec3(-size, -size, -size),vec3(0,0,-1),vec2(0.0f,0.0f) });
		vertex_list[0].push_back({ vec3(size, -size, -size),vec3(0,0,-1),vec2(0.0f,1.0f) });
		vertex_list[0].push_back({ vec3(size, -size, -size),vec3(0,0,-1),vec2(0.0f,1.0f) });
		vertex_list[0].push_back({ vec3(size,  size, -size),vec3(0,0,-1),vec2(1.0f,1.0f) });
		vertex_list[0].push_back({ vec3(-size,  size, -size),vec3(0,0,-1),vec2(1.0f,0.0f) });

		//left
		vertex_list[1].push_back({ vec3(-size, -size,  size),vec3(1,0,0),vec2(0.0f,1.0f) });
		vertex_list[1].push_back({ vec3(-size, -size, -size),vec3(1,0,0),vec2(0.0f,0.0f) });
		vertex_list[1].push_back({ vec3(-size,  size, -size),vec3(1,0,0),vec2(1.0f,0.0f) });
		vertex_list[1].push_back({ vec3(-size,  size, -size),vec3(1,0,0),vec2(1.0f,0.0f) });
		vertex_list[1].push_back({ vec3(-size,  size,  size),vec3(1,0,0),vec2(1.0f,1.0f) });
		vertex_list[1].push_back({ vec3(-size, -size,  size),vec3(1,0,0),vec2(0.0f,1.0f) });

		//right
		vertex_list[2].push_back({ vec3(size, -size, -size),vec3(-1,0,0),vec2(1.0f,0.0f) });
		vertex_list[2].push_back({ vec3(size, -size,  size),vec3(-1,0,0),vec2(1.0f,1.0f) });
		vertex_list[2].push_back({ vec3(size,  size,  size),vec3(-1,0,0),vec2(0.0f,1.0f) });
		vertex_list[2].push_back({ vec3(size,  size,  size),vec3(-1,0,0),vec2(0.0f,1.0f) });
		vertex_list[2].push_back({ vec3(size,  size, -size),vec3(-1,0,0),vec2(0.0f,0.0f) });
		vertex_list[2].push_back({ vec3(size, -size, -size),vec3(-1,0,0),vec2(1.0f,0.0f) });

		//top
		vertex_list[3].push_back({ vec3(-size, -size,  size),vec3(0,0,-1),vec2(0.0f,1.0f) });
		vertex_list[3].push_back({ vec3(-size,  size,  size),vec3(0,0,-1),vec2(0.0f,0.0f) });
		vertex_list[3].push_back({ vec3(size,  size,  size),vec3(0,0,-1),vec2(1.0f,0.0f) });
		vertex_list[3].push_back({ vec3(size,  size,  size),vec3(0,0,-1),vec2(1.0f,0.0f) });
		vertex_list[3].push_back({ vec3(size, -size,  size),vec3(0,0,-1),vec2(1.0f,1.0f) });
		vertex_list[3].push_back({ vec3(-size, -size,  size),vec3(0,0,-1),vec2(0.0f,1.0f) });

		//front
		vertex_list[4].push_back({ vec3(-size,  size, -size),vec3(0,-1,0),vec2(0.0f,0.0f) });
		vertex_list[4].push_back({ vec3(size,  size, -size),vec3(0,-1,0),vec2(1.0f,0.0f) });
		vertex_list[4].push_back({ vec3(size,  size,  size),vec3(0,-1,0),vec2(1.0f,1.0f) });
		vertex_list[4].push_back({ vec3(size,  size,  size),vec3(0,-1,0),vec2(1.0f,1.0f) });
		vertex_list[4].push_back({ vec3(-size,  size,  size),vec3(0,-1,0),vec2(0.0f,1.0f) });
		vertex_list[4].push_back({ vec3(-size,  size, -size),vec3(0,-1,0),vec2(0.0f,0.0f) });
	
		//back
		vertex_list[5].push_back({ vec3(-size, -size, -size),vec3(0,1,0),vec2(1.0f,0.0f) });
		vertex_list[5].push_back({ vec3(-size, -size,  size),vec3(0,1,0),vec2(1.0f,1.0f) });
		vertex_list[5].push_back({ vec3(size, -size, -size),vec3(0,1,0),vec2(0.0f,0.0f) });
		vertex_list[5].push_back({ vec3(size, -size, -size),vec3(0,1,0),vec2(0.0f,0.0f) });
		vertex_list[5].push_back({ vec3(-size, -size,  size),vec3(0,1,0),vec2(1.0f,1.0f) });
		vertex_list[5].push_back({ vec3(size, -size,  size),vec3(0,1,0),vec2(0.0f,1.0f) });

		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				index_list[i].push_back(j);
			}
			bind_texture(&texture_buffer[i], path[i], window_size);

			glGenBuffers(1, &vertex_buffer[i]);
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[i]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertex_list[i].size(), &vertex_list[i][0], GL_STATIC_DRAW);

			// geneation of index buffer
			glGenBuffers(1, &index_buffer[i]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer[i]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * index_list[i].size(), &index_list[i][0], GL_STATIC_DRAW);

		}

	}
};