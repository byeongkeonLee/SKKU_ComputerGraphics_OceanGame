#version 330

// input from vertex shader
in vec2 tc;

// the only output variable
out vec4 fragColor;

uniform sampler2D	TEX;

void main()
{	
	fragColor = texture2D(TEX, tc);
}
