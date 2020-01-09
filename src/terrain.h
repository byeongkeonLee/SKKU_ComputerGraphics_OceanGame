#pragma once
#include "cgmath.h"			// slee's simple math library
#include "constant.h"
#include "cgut.h"			// slee's OpenGL utility
#include "func.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "bmp.h"


class Terrain{

private:

	const char* background_texture_path = "../bin/map/grass_texture.jpg";
	const char* r_texture_path = "../bin/map/sand_texture.jpg";
	const char* g_texture_path = "../bin/map/snow_texture.jpg";
	const char* b_texture_path = "../bin/map/stone_texture.jpg";
	const char* blend_texture_path = "../bin/map/blend_map_texture.jpg";

	GLuint vertex_buffer;
	GLuint index_buffer;
	GLuint texture_buffer[5] = { 0 };
	std::vector<vertex>	vertex_list;
	std::vector<uint>	index_list;

	int w;
	int l;
	float** hs;
	vec3** normals;
	bool computedNormals;
	float maxheight = 140.0f;
	const int MAX_COLOR = 256 * 256 * 256;
	float x_trans=-1024.0f;
	float y_trans = -1024.0f;
	float scale=8.0f;
	float z_scale = 5.0f;

public:
	void prepare_texture(GLuint program) {

		glUniform1i(glGetUniformLocation(program, "backgroundTexture"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_buffer[0]);

		glUniform1i(glGetUniformLocation(program, "rTexture"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture_buffer[1]);


		glUniform1i(glGetUniformLocation(program, "gTexture"), 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture_buffer[2]);


		glUniform1i(glGetUniformLocation(program, "bTexture"), 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, texture_buffer[3]);


		glUniform1i(glGetUniformLocation(program, "blendMap"), 4);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, texture_buffer[4]);

		
	}

	void render(GLuint program) {

		prepare_texture(program);
		if (vertex_buffer)	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		if (index_buffer)	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		mat4 model_matrix = mat4::translate(vec3(x_trans, y_trans, 0)) * mat4::scale(vec3(vec2(scale),1.0f)) * mat4::identity();
		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
		glUniform1i(glGetUniformLocation(program, "VertexShaderMode"), TERRAIN_SHADER);
		glUniform1i(glGetUniformLocation(program, "FragmentShaderMode"), TERRAIN_SHADER);
		cg_bind_vertex_attributes(program);
		glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_INT, nullptr);
	}
	void texture_init() {

		bind_texture(&texture_buffer[0], background_texture_path, window_size);
		bind_texture(texture_buffer + 1, r_texture_path, window_size);
		bind_texture(texture_buffer + 2, g_texture_path, window_size);
		bind_texture(texture_buffer + 3, b_texture_path, window_size);
		bind_texture(texture_buffer + 4, blend_texture_path, window_size);

	}
	Terrain(const char* filePath,int w2, int l2) {
		w = w2;
		l = l2;

		hs = new float* [l+1];
		for (int i = 0; i < l+1; i++) {
			hs[i] = new float[w+1];
		}
		normals = new vec3 * [l+1];
		for (int i = 0; i < l+1; i++) {
			normals[i] = new vec3[w+1];
		}
		computedNormals = false;

		loadTerrain(filePath, 256,256,maxheight);
		//printf("loadTerrain\n");
		computeNormals();
		//printf("Compute Normals\n");
		set_vertex_index();
		//printf("Set_Vertex_indes\n");
		texture_init();
		//printf("Texture Init\n");
	}
	void set_vertex_index() {
		if (vertex_buffer)	glDeleteBuffers(1, &vertex_buffer);	vertex_buffer = 0;
		if (index_buffer)	glDeleteBuffers(1, &index_buffer);	index_buffer = 0;

		for (int z = 0; z < l - 1; z++) {
			for (int x = 0; x < w - 1; x++) {
				int a, b;
				vertex_list.push_back({ vec3((float)x,(float)z,getHeight(x,z)),getNormal(x,z),vec2((float)x/255.0f,(float)z/255.0f) });
				a = x + 1;
				b = z ;
				vertex_list.push_back({ vec3((float)a,(float)b,getHeight(a,b)),getNormal(a,b),vec2((float)a/255.0f,(float)b/255.0f) });
				a = x + 1;
				b = z + 1;
				vertex_list.push_back({ vec3((float)a,(float)b,getHeight(a,b)),getNormal(a,b),vec2((float)a/255.0f,(float)b/255.0f) });
				a = x ;
				b = z + 1;
				vertex_list.push_back({ vec3((float)a,(float)b,getHeight(a,b)),getNormal(a,b),vec2((float)a/255.0f,(float)b/255.0f) });

			}
		}
		for (unsigned int i = 0; i < vertex_list.size(); i+=4) {
			index_list.push_back(i);
			index_list.push_back(i+1);
			index_list.push_back(i+2);
			index_list.push_back(i);
			index_list.push_back(i+2);
			index_list.push_back(i+3);
		}
		glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertex_list.size(), &vertex_list[0], GL_STATIC_DRAW);

		// geneation of index buffer
		glGenBuffers(1, &index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * index_list.size(), &index_list[0], GL_STATIC_DRAW);
	}
	void setHeight(int x, int y, float z) {
		hs[y][x] = z;
		computedNormals = false;
	}
	inline float getHeight_ship(vec2 pos) {
		float x = (pos.x - x_trans) / scale;
		float y = (pos.y - y_trans) / scale;
		//float interpolx = x-int(x)/1;
		//float interpoly = y - int(y) / 1;
		return getHeight (int(x), int(y));
	}
	float getHeight(int x, int y) {
		if (x >= w) x = w - 1;
		if (x < 0) x = 0;
		if (y < 0) y = 0;
		if (y >= l) y = l - 1;
		return hs[y][x];
	}
	vec3 getNormal(int x, int y) {
		if (x >= w) x = w - 1;
		if (x < 0) x = 0;
		if (y < 0) y = 0;
		if (y >= l) y = l - 1;
		return normals[y][x];
	}
	bool loadTerrain(const char* image_, int width_, int height_, float maxval) {
		printf("Image loaded : %s\n", image_);
		int comp; int& width = width_, & height = height_;
		unsigned char* pimage0 = stbi_load(image_, &width, &height, &comp, 3); if (comp == 1) comp = 3; /* convert 1-channel to 3-channel image */
		int stride0 = width * comp, stride1 = (stride0 + 3) & (~3);	// 4-byte aligned stride
		unsigned char* pimage = (unsigned char*)malloc(sizeof(unsigned char) * stride1 * height);
		for (int y = 0; y < height; y++) memcpy(pimage + (height - 1 - y) * stride1, pimage0 + y * stride0, stride0); // vertical flip
		stbi_image_free(pimage0); // release the original image

		for (int y = 0; y < width_; y++) {
			for (int x = 0; x < height_; x++) {
				int R = *(pimage + y * stride1 + x * 3 + 0);
				int G = *(pimage + y * stride1 + x * 3 + 1);
				int B = *(pimage + y * stride1 + x * 3 + 2);
				float h = (maxval * (1.0f*(R*256*256 + G*256+B)/MAX_COLOR-0.5f)+30.0f)*z_scale;
				setHeight(x, y, h);
			}
		}

		/*
printf("%d ", *(pimage + 0 * stride1 + 0));
printf("%d ", *(pimage + 0 * stride1 + 1));
printf("%d ", *(pimage + 0 * stride1 + 2));*/

		for (int y = 0; y < width_ ; y++) {
			setHeight(0, y, -maxval);
			setHeight(height_ - 1, y, -maxval);
		}
		for (int x = 0; x < height_; x++) {
			setHeight(x, 0, -maxval);
			setHeight(x, width_ - 1, -maxval);
		}
		free(pimage);
		return true;
	}
	void computeNormals() {
		if (computedNormals) {
			return;
		}
		computedNormals = TRUE;

		for (int z = 0; z < l; z++) {
			for (int x = 0; x < w; x++) {
				float heightL = getHeight(x - 1, z);
				float heightR = getHeight(x + 1, z);
				float heightD = getHeight(x , z-1);
				float heightU = getHeight(x , z+1);

				vec3 normalvec = vec3(heightL - heightR, heightU - heightD, 2.0f);

				normals[z][x] = normalvec.normalize();
			}
		}
	}
};


Terrain* terrain;
