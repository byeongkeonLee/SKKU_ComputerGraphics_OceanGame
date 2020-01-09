#version 330

// vertex attributes
in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec2 tc;	// texture coordinate

void main()
{
	gl_Position = vec4(position,1);
	tc = texcoord;
}