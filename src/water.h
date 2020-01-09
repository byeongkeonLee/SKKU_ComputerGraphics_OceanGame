#pragma once
#include "cgmath.h"			// slee's simple math library
#include "cgut.h"			// slee's OpenGL utility
#include "func.h"
#include "constant.h"
#include "camera.h"

class Water {
public:
	std::vector<vertex>	vertex_list;	// host-side vertices
	std::vector<uint>	index_list;		// host-side indices
	GLuint	vertex_buffer = 0;	// ID holder for vertex buffer
	GLuint	index_buffer = 0;	// ID holder for index buffer
	GLuint	dudvTexture = 0;
	GLuint	normalTexture = 0;
	const char* dudvmap = "../bin/water/dudvmap.jpg";
	const char* normalmap = "../bin/water/normalmap.jpg";
	mat4 model_matrix;
	int wavedistance = 0;
	vec3 position = vec3(0, 0, 0);

	int REFLECTION_WIDTH = 1280;
	int REFLECTION_HEIGHT = 720;
	int REFRACTION_WIDTH = 1280;
	int REFRACTION_HEIGHT = 720;

	GLuint reflectionFramebuffer;
	GLuint reflectionTexture;
	GLuint reflectionDepthBuffer;

	GLuint refractionFramebuffer;
	GLuint refractionTexture;
	GLuint refractionDepthTexture;

	void initial_water_reflection(int width, int height) {

		//createFramebuffer
		glGenFramebuffers(1, &reflectionFramebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, reflectionFramebuffer);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		//createTextureAttachment
		glGenTextures(1, &reflectionTexture);
		glBindTexture(GL_TEXTURE_2D, reflectionTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, reflectionTexture, 0);

		//createDepthbufferAttachment
		glGenRenderbuffers(1, &reflectionDepthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, reflectionDepthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, reflectionDepthBuffer);
		unbindCurrentFrameBuffer();
	}
	void initial_water_refraction(int width, int height) {

		//createFramebuffer
		glGenFramebuffers(1, &refractionFramebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, refractionFramebuffer);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		//createTextureAttachment
		glGenTextures(1, &refractionTexture);
		glBindTexture(GL_TEXTURE_2D, refractionTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, refractionTexture, 0);

		//createTextureAttachment
		glGenTextures(1, &refractionDepthTexture);
		glBindTexture(GL_TEXTURE_2D, refractionDepthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, refractionDepthTexture, 0);
		unbindCurrentFrameBuffer();
	}

	Water(int width, int height) {


		initial_water_reflection(width,height);
		initial_water_refraction(width, height);

		vertex_list.clear();
		vertex_list.push_back({ vec3(-WORLD_HEIGHT, -WORLD_HEIGHT, 0.0f), vec3(0, 0, 1), vec2(0.0f, 0.0f) });
		vertex_list.push_back({ vec3(WORLD_HEIGHT, -WORLD_HEIGHT, 0.0f), vec3(0, 0, 1), vec2(1.0f, 0.0f) });
		vertex_list.push_back({ vec3(WORLD_HEIGHT, WORLD_HEIGHT, 0.0f), vec3(0, 0, 1), vec2(1.0f, 1.0f) });
		vertex_list.push_back({ vec3(-WORLD_HEIGHT, WORLD_HEIGHT, 0.0f), vec3(0, 0, 1), vec2(0.0f, 1.0f) });

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

		bind_texture(&dudvTexture, dudvmap, window_size);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		bind_texture(&normalTexture, normalmap, window_size);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	void update(float gl_time) {
		wavedistance++;
	}
	void render(GLuint program) {
		//mat4 model_matrix = mat4::identity(); // mat4(1, 0, 0, 10.0f, 0, 1, 0, 10.0f, 0, 0, 1, 50.0f, 0, 0, 0, 1);
		//model_matrix = mat4(1, 0, 0, 0, 0, 1, 0, 10.0f, 0, 0, 1, 0, 0, 0, 0, 1);
		glUniform1f(glGetUniformLocation(program, "wavedistance"), wavedistance/1500.0f);
		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
		glUniform3fv(glGetUniformLocation(program, "camera_eye"),1, cam.eye);
		glUniform1i(glGetUniformLocation(program, "VertexShaderMode"), WATER_SHADER);
		glUniform1i(glGetUniformLocation(program, "FragmentShaderMode"), WATER_SHADER);

		glUniform1i(glGetUniformLocation(program, "reflectionTexture"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, reflectionTexture);

		glUniform1i(glGetUniformLocation(program, "refractionTexture"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, refractionTexture);

		glUniform1i(glGetUniformLocation(program, "dudvMap"), 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, dudvTexture);

		glUniform1i(glGetUniformLocation(program, "normalMap"), 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, normalTexture);

		glUniform3fv(glGetUniformLocation(program, "light_position"), 1,sun.position);
		if (vertex_buffer)	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		if (index_buffer)	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		cg_bind_vertex_attributes(program);
		glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_INT, nullptr);

		
	}

	void finalize() {
		vertex_list.clear();
		index_list.clear();
	}
};