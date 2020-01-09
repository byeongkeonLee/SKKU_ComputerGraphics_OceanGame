#pragma once
#include "cgmath.h"			// slee's simple math library
#include "cgut.h"			// slee's OpenGL utility
#include "func.h"
#include "constant.h"

class Skill_help {
	std::vector<vertex>	vertex_list;	// host-side vertices
	std::vector<uint>	index_list;		// host-side indices
	GLuint	vertex_buffer = 0;	// ID holder for vertex buffer
	GLuint	index_buffer = 0;	// ID holder for index buffer
public:
	int skill_help_num = -1;
	bool explain = true;
	bool update(float glTime) {
		return true;
	}
	bool render(GLuint program, void render_text(std::string, GLint, GLint, GLfloat, vec4)) {
		if (skill_help_num != -1) {
			glUniform1i(glGetUniformLocation(program, "VertexShaderMode"), 0);
			glUniform1i(glGetUniformLocation(program, "FragmentShaderMode"), 0);
			glUniform1i(glGetUniformLocation(program, "use_texture"), 0);

			glUniform2fv(glGetUniformLocation(program, "guiPosition"), 1, vec2(0, 0));
			glUniform4fv(glGetUniformLocation(program, "diffuse"), 1, vec4(0.2f, 0.2f, 0.2f, 0.8f));
			if (vertex_buffer)	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
			if (index_buffer)	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
			cg_bind_vertex_attributes(program);
			glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_INT, nullptr);


			render_text(default_skill_info[skill_help_num].skill_name, int(window_size.x * 0.18f), int(window_size.y * 0.86f), 0.35f, vec4(1.0f, 1.0f, 1.0f, 1)); //context
			render_text("Level : ", int(window_size.x * 0.3f), int(window_size.y * 0.86f), 0.35f, vec4(1, 1, 1, 1)); //context
			render_text(std::to_string(default_skill_info[skill_help_num].level), int(window_size.x * 0.38f), int(window_size.y * 0.86f), 0.35f, vec4(1, 1, 0, 1)); //context
			render_text("MP : ", int(window_size.x * 0.5f), int(window_size.y * 0.86f), 0.35f, vec4(1, 1, 1, 1)); //context
			render_text(std::to_string(default_skill_info[skill_help_num].wasted_mp), int(window_size.x * 0.54f), int(window_size.y * 0.86f), 0.35f, vec4(0, 0, 1, 1)); //context
			render_text("Cooltime : ", int(window_size.x * 0.65f), int(window_size.y * 0.86f), 0.35f, vec4(1, 1, 1, 1)); //context
			render_text(std::to_string(int(default_skill_info[skill_help_num].cooldown)) + "sec", int(window_size.x * 0.75f), int(window_size.y * 0.86f), 0.35f, vec4(0, 1, 1, 1)); //context
			render_text(default_skill_info[skill_help_num].context, (int)(window_size.x * 0.18f), (int)(window_size.y * 0.9f), 0.3f, vec4(0.6f, 0.6f, 0.6f, 1)); //context
			render_text(default_skill_info[skill_help_num].context2, (int)(window_size.x * 0.18f), (int)(window_size.y * 0.93f), 0.3f, vec4(0.6f, 0.6f, 0.6f, 1)); //context
		}
		return true;
	}
	Skill_help() {
		vertex_list.clear();
		vertex_list.push_back({ vec3(-0.7f, -0.9f, 0.0f), vec3(0, 0, 1), vec2(0.0f, 0.0f) });
		vertex_list.push_back({ vec3(0.7f, -0.9f, 0.0f), vec3(0, 0, 1), vec2(1.0f, 0.0f) });
		vertex_list.push_back({ vec3(0.7f, -0.6f, 0.0f), vec3(0, 0, 1), vec2(1.0f, 1.0f) });
		vertex_list.push_back({ vec3(-0.7f, -0.6f, 0.0f), vec3(0, 0, 1), vec2(0.0f, 1.0f) });

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