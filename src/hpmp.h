#pragma once

#include "cgmath.h"			// slee's simple math library
#include "constant.h"
#include "cgut.h"			// slee's OpenGL utility
#include "func.h"
#include "assimp_loader.h"

class HPMP {
public:
	std::vector<vertex>	vertex_list;	// host-side vertices
	std::vector<uint>	index_list;		// host-side indices
	GLuint index_buffer = 0;
	GLuint vertex_buffer = 0;
	vec2 pos;
	mat4 model_matrix;
	int hp, maxhp, mp, maxmp,exper,maxexp;
	float hp_ratio, mp_ratio,exp_ratio, hp_shield_effect;
	bool update(vec2 ship_pos,int hp_, int maxhp_, int hp_shield, int mp_, int maxmp_,int exp_, int expmax_) {
		pos = ship_pos;
		hp = hp_;
		maxhp = maxhp_;
		mp = mp_;
		maxmp = maxmp_;
		hp_ratio = 1.0f * hp / (maxhp + hp_shield);
		mp_ratio = 1.0f * mp / maxmp;
		exper = exp_;
		maxexp = expmax_;
		hp_shield_effect = 1.0f * (hp + hp_shield) / (maxhp + hp_shield);
		exp_ratio = 1.0f * exper / maxexp;
		return true;
	}
	bool render(GLuint program, float hp_ratio_effect_, float mp_ratio_effect_) {
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		cg_bind_vertex_attributes(program);
		glUniform1i(glGetUniformLocation(program, "VertexShaderMode"), BILLBOARD_SHADER);
		glUniform1i(glGetUniformLocation(program, "use_texture"), false);
		glUniform1i(glGetUniformLocation(program, "FragmentShaderMode"), BILLBOARD_SHADER);


		glUniform4fv(glGetUniformLocation(program, "diffuse"), 1, vec4(1, 0, 0, 1));
		glUniform1f(glGetUniformLocation(program, "ratio_effect"), hp_ratio_effect_);
		glUniform1f(glGetUniformLocation(program, "ratio"), hp_ratio);
		glUniform1f(glGetUniformLocation(program, "ratio_shield"), hp_shield_effect);
		mat4 model_matrix = mat4::translate(vec3(pos, 20.0f)) * mat4::scale(1, 1, 0.5f);
		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
		glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_INT, nullptr);
		glUniform1f(glGetUniformLocation(program, "ratio_shield"), 0.0f);

		glUniform4fv(glGetUniformLocation(program, "diffuse"), 1, vec4(0, 0, 1, 1));
		glUniform1f(glGetUniformLocation(program, "ratio"), mp_ratio);
		glUniform1f(glGetUniformLocation(program, "ratio_effect"), mp_ratio_effect_);
		model_matrix = mat4::translate(vec3(pos, 19.0f)) * mat4::scale(1, 1, 0.2f);
		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
		glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_INT, nullptr);

		glUniform4fv(glGetUniformLocation(program, "diffuse"), 1, vec4(0, 1, 0, 1));
		glUniform1f(glGetUniformLocation(program, "ratio"), exp_ratio);
		glUniform1f(glGetUniformLocation(program, "ratio_effect"), 0.0f);
		model_matrix = mat4::translate(vec3(pos, 18.7f)) * mat4::scale(1, 1, 0.2f);
		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
		glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_INT, nullptr);
		return true;
	}

	HPMP() {
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