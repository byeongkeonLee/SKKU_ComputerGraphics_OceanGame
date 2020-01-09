#pragma once

#include "cgmath.h"			// slee's simple math library
#include "constant.h"
#include "cgut.h"			// slee's OpenGL utility
#include "func.h"
#include "assimp_loader.h"

class Scene {
public:
	std::vector<vertex>	vertex_list;	// host-side vertices
	std::vector<uint>	index_list;		// host-side indices
	GLuint texture_buffer = 0;
	GLuint index_buffer = 0;
	GLuint vertex_buffer = 0;

	GLuint Framebuffer;
	GLuint FrameTexture;
	GLuint FrameDepthBuffer;

	bool render(GLuint program) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cg_bind_vertex_attributes(program);

		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_buffer);
		cg_bind_vertex_attributes(program);
		glUniform2fv(glGetUniformLocation(program, "guiPosition"), 1, vec2(0, 0));
		glUniform2fv(glGetUniformLocation(program, "texel_offset"), 1, vec2(1.0f/window_size.x, 1.0f/window_size.y));
		glUniform1i(glGetUniformLocation(program, "VertexShaderMode"), 0);
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);
		glUniform1i(glGetUniformLocation(program, "FragmentShaderMode"), SCENE_RADIAL_BLUR);
		glUniform1f(glGetUniformLocation(program, "blur_intensity"), cam.radial_intensity);
		
		glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_INT, nullptr);

		return true;
	}

	void initial_scene_framebuffer(int width, int height) {

		//createFramebuffer
		glGenFramebuffers(1, &Framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		//createTextureAttachment
		glGenTextures(1, &FrameTexture);
		glBindTexture(GL_TEXTURE_2D, FrameTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, FrameTexture, 0);

		//createDepthbufferAttachment
		glGenRenderbuffers(1, &FrameDepthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, FrameDepthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, FrameDepthBuffer);
		unbindCurrentFrameBuffer();
	}

	Scene() {
		initial_scene_framebuffer(window_size.x, window_size.y);

		vertex_list.clear();
		vertex_list.push_back({ vec3(-1.0f, -1.0f, 0.5f), vec3(0, 0, 1), vec2(0.0f, 0.0f) });
		vertex_list.push_back({ vec3(1.0f, -1.0f, 0.5f), vec3(0, 0, 1), vec2(1.0f, 0.0f) });
		vertex_list.push_back({ vec3(1.0f, 1.0f, 0.5f), vec3(0, 0, 1), vec2(1.0f, 1.0f) });
		vertex_list.push_back({ vec3(-1.0f, 1.0f, 0.5f), vec3(0, 0, 1), vec2(0.0f, 1.0f) });

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

		glGenTextures(1, &texture_buffer);
		glBindTexture(GL_TEXTURE_2D, texture_buffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, window_size.x, window_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
};