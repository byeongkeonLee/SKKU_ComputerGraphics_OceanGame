#pragma once
#include "cgmath.h"			// slee's simple math library
#include "cgut.h"			// slee's OpenGL utility
#include "func.h"
#include "constant.h"

#define max_particle 3000
class Particles {
	GLuint	vertex_buffer = 0;	// ID holder for vertex buffer
	GLuint	index_buffer = 0;	// ID holder for index buffer
	GLuint	texture_buffer = 0;
	std::vector<vertex>	vertex_list;
	std::vector<uint>	index_list;
	const char* texture_path = "../bin/particle/particle.png";
	bool first = false;
	struct particle_t {
		vec3 pos = vec3(0, 0, 0);
		vec4 init_color;
		vec4 color=vec4(0,0,0,0);
		vec3 velocity;
		float scale;
		float life = 5.0f;
		float angle = 0.0f;
		float radius = 0.0f;
		//optional
		float cameradistance = 0.0f;
		float start_time;
		float degree;
		bool draw = false;
		particle_t() { reset(vec2(0, 0), angle, vec2(0, 0), 0.0f); life = random() * 5.0f; 
		}

		void reset(vec2 ship_pos,float ship_angle,vec2 speed, float alpha)
		{
			pos = vec3(ship_pos, 5);
			init_color = vec4(1,random()*0.1f,0, alpha);
			scale = random() * 1.0f;
			angle = -ship_angle + random() * PI / 2 + PI / 4;

			radius = random()*r_1->range/4.0f;
			life = random() * 7.0f+5.0f;
			degree = 1.0f / life;
			bool dir =  random() < 0.5f;
			velocity = vec3(cos(angle) * radius , sin(angle) * radius  , -random() * 0.001f) + vec3(speed,0)*25.0f;
		}

		void update(vec3 eye, vec2 ship_pos,bool generator_particle,float angle,vec2 speed)
		{
			draw = true;
			velocity.z += 0.001f;
			pos += velocity*0.05f;
			life -= 0.05f;
			init_color.a-= 0.005f;
			
			cameradistance = distance(pos, eye);
			// dead
			if (life < 0.0f) {
				draw = false;
				cameradistance = -1.0f;
				if (r_1->turn_on) {
					reset(ship_pos, angle, speed,1.0f);
				}
			}
			color = init_color;
			color.a = draw ? init_color.a : 0.0f;
		}
		bool particle_t::operator<(const particle_t& that) const {
			return this->cameradistance > that.cameradistance;
		}
	};
	vec4 particle_position_size_arr[max_particle];
	vec4 particle_color_arr[max_particle];
	std::vector<particle_t> particles;

	GLuint particles_position_buffer;
	GLuint particles_color_buffer;

public:
	vec3 position = { 0 };
	float radius = 14.0f;
	// index of rendering frames

	bool update(vec2 ship_pos, float glTime, vec3 eye_,float angle,vec2 speed) {
		for (auto& p : particles) p.update(eye_,ship_pos,normal_left->turn_on,angle,speed);
		std::sort(&particles[0], &particles[max_particle]);
		int i = 0;
		for (auto& p : particles) {
			particle_position_size_arr[i] = vec4(vec3(p.pos), p.scale);
			particle_color_arr[i] = p.color;
			i++;
		}
		return true;
	}
	bool render(GLuint program) {
		glDepthMask(GL_FALSE);
		glUseProgram(program);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUniform1i(glGetUniformLocation(program, "VertexShaderMode"), PARTICLE_SHADER);
		glUniform1i(glGetUniformLocation(program, "FragmentShaderMode"), PARTICLE_SHADER);

		glActiveTexture(GL_TEXTURE0);		
		glBindTexture(GL_TEXTURE_2D, texture_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		GLuint vertices_loc = glGetAttribLocation(program, "vertices");
		glEnableVertexAttribArray(vertices_loc);
		glVertexAttribPointer(vertices_loc, sizeof(vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)0);


		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, max_particle * sizeof(vec4), particle_position_size_arr);

		GLuint xyzs_loc = glGetAttribLocation(program, "xyzs");
		glEnableVertexAttribArray(xyzs_loc);
		glVertexAttribPointer(xyzs_loc, sizeof(vec4) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);



		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, max_particle * sizeof(vec4), particle_color_arr);

		GLuint color_loc = glGetAttribLocation(program, "color");
		glEnableVertexAttribArray(color_loc);
		glVertexAttribPointer(color_loc, sizeof(vec4) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

		glVertexAttribDivisor(vertices_loc, 0);
		glVertexAttribDivisor(xyzs_loc, 1);
		glVertexAttribDivisor(color_loc, 1);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, max_particle);

		glDisableVertexAttribArray(vertices_loc);
		glDisableVertexAttribArray(xyzs_loc);
		glDisableVertexAttribArray(color_loc);

		glDepthMask(GL_TRUE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		return true;
	}
	Particles() {
		static vertex vertices[] = { {vec3(-1,-1,0),vec3(0,0,1),vec2(0,0)}, {vec3(1,-1,0),vec3(0,0,1),vec2(1,0)}, {vec3(-1,1,0),vec3(0,0,1),vec2(0,1)}, {vec3(1,1,0),vec3(0,0,1),vec2(1,1)} }; // strip ordering [0, 1, 3, 2]

	// generation of vertex buffer: use vertices as it is
		glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * 4, &vertices[0], GL_STATIC_DRAW);
		bind_texture_png(&texture_buffer, texture_path, window_size);

		particles.resize(max_particle);
		
		glGenBuffers(1, &particles_position_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glBufferData(GL_ARRAY_BUFFER, max_particle * sizeof(vec4), NULL, GL_STREAM_DRAW);

		glGenBuffers(1, &particles_color_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glBufferData(GL_ARRAY_BUFFER, max_particle * sizeof(vec4), NULL, GL_STREAM_DRAW);

	}
};