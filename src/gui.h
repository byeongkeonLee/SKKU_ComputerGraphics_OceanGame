#pragma once
#include "cgmath.h"			// slee's simple math library
#include "cgut.h"			// slee's OpenGL utility
#include "func.h"
#include "constant.h"

class Gui {
	std::vector<vertex>	vertex_list;	// host-side vertices
	std::vector<uint>	index_list;		// host-side indices
	GLuint	vertex_buffer = 0;	// ID holder for vertex buffer
	GLuint	index_buffer = 0;	// ID holder for index buffer
	GLuint	texture_buffer = 0;
	const char* texture_path[2][4] = { {"../bin/skills/Q.jpg","../bin/skills/W.jpg","../bin/skills/R.jpg","../bin/skills/space.jpg"},{"../bin/skills/Q2.jpg","../bin/skills/E2.jpg","../bin/skills/R2.jpg","../bin/skills/space2.jpg"} };	// OBJ
	const char* q1_2_texture = "../bin/skills/Q1_2.jpg";
	uint skill_number;
	vec2 position;
	float remained_cool;
	bool explain = false;
public:
	bool update(float glTime) {
		
		if (skill_number != 9)
			remained_cool = (default_skill_info[skill_number].cooldown_end - glTime) / (default_skill_info[skill_number].cooldown);
		else {
			remained_cool = (q_1->hit_run_time_end - glTime) / q_1->hit_run_time;
		}
		return true;
	}
	bool render(GLuint program) {
		glUniform1i(glGetUniformLocation(program, "VertexShaderMode"), 0);
		glUniform1i(glGetUniformLocation(program, "FragmentShaderMode"), SKILL_GUI_SHADER);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_buffer);

		glUniform2fv(glGetUniformLocation(program, "guiPosition"), 1,position);
		glUniform1f(glGetUniformLocation(program, "remained_cool"), remained_cool);
		if (vertex_buffer)	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		if (index_buffer)	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);
		cg_bind_vertex_attributes(program);
		glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_INT, nullptr);
		return true;
	}
	Gui(vec2 position_,int i) {
		position = position_;
		skill_number = i + (SKILL_SET == 0? 0:4);
		vertex_list.clear();
		vertex_list.push_back({ vec3(-0.05f, -0.1f, 0.0f), vec3(0, 0, 1), vec2(0.0f, 0.0f) });
		vertex_list.push_back({ vec3(0.05f, -0.1f, 0.0f), vec3(0, 0, 1), vec2(1.0f, 0.0f) });
		vertex_list.push_back({ vec3(0.05f, 0.1f, 0.0f), vec3(0, 0, 1), vec2(1.0f, 1.0f) });
		vertex_list.push_back({ vec3(-0.05f, 0.1f, 0.0f), vec3(0, 0, 1), vec2(0.0f, 1.0f) });


		if (i != 9)
			bind_texture(&texture_buffer, texture_path[SKILL_SET][i], window_size);
		else {
			bind_texture(&texture_buffer, q1_2_texture, window_size);
		}

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
	}
};