#version 330

// input attributes of vertices
in vec3 position;	
in vec3 normal;
in vec2 texcoord;

//particles
in vec3 vertices;
in vec4 xyzs;
in vec4 color;
out vec4 particle_color;

out vec3 norm;	
out vec2 tc;	
out vec4 clipSpace;
out vec3 camera_water_vector;
out vec3 sun_water_vector;
out float visibility;

// uniform variables
uniform mat4	model_matrix;	// 4x4 transformation matrix: explained later in the lecture
uniform float	aspect_ratio;	// to correct a distortion of the shape
uniform mat4	view_matrix;
uniform mat4	projection_matrix;
uniform vec4	plane;
uniform int		VertexShaderMode;
uniform vec2	guiPosition;
uniform float	thick;
//WATER shader
uniform sampler2D dudvMap;
uniform float	wavexy;
uniform vec3	camera_eye;
uniform vec3	sun_position;
uniform vec2	scale;

uniform float fakelight;
const float density = 0.001f;
const float gradient = 4.0f;

void main()
{
	vec4 wpos;
	vec4 epos;
	vec3 camera_right;
	vec3 camera_up;
	vec3 billboard;
	switch(VertexShaderMode){

	//hpmp GUIstatus
	case -2:
		gl_Position = mat4(scale.x,0,0,0, 0,scale.y,0,0, 0,0,1,0, 0,0,0,1) *aspect_ratio*vec4(position,1) + vec4(guiPosition,0,0);
		tc = texcoord;
		break;

	//sniper
	case -1:
		gl_Position = aspect_ratio*vec4(position,1);
		tc = texcoord;
		break;
	case 0:
		gl_Position = aspect_ratio*vec4(position,1) + vec4(guiPosition,0,0);
		tc = texcoord;
		break;

	//3D GAME SHADER
	case 1:
		wpos = model_matrix * (vec4(position, 1)+vec4(vec3(normal*thick),0));
		epos = view_matrix * wpos;
		gl_ClipDistance[0]=dot(wpos, plane);
		gl_Position = projection_matrix * epos;
		norm = normal;
		tc = texcoord;

		float distance = length(epos.xyz);
		visibility = exp(-pow(distance*density,gradient));
		visibility = clamp(visibility, 0.0f,1.0f);

		break;
	case 2:
		wpos = model_matrix* vec4(position,1);
		epos = view_matrix * wpos;
		gl_Position = projection_matrix * epos;
		clipSpace = gl_Position;
		camera_water_vector = (camera_eye - wpos.xyz);
		sun_water_vector = (sun_position-wpos.xyz);
		tc = vec2(texcoord.x/2.0f+0.5f, texcoord.y/2.0f+0.5f ) * 40.0f;
		norm = normalize(mat3(view_matrix*model_matrix)*normal);
		visibility = exp(-pow(distance*density,gradient));
		visibility = clamp(visibility, 0.0f,1.0f);
		break;
	case 8:
		camera_right = vec3(view_matrix[0][0],view_matrix[1][0],view_matrix[2][0]);
		camera_up = vec3(view_matrix[0][1],view_matrix[1][1],view_matrix[2][1]);
		billboard = camera_right*position.x*5.0f + camera_up*position.y;
		gl_Position = projection_matrix * view_matrix* model_matrix * vec4(billboard,1);
		tc =texcoord;
		break;
	case 9:
		wpos = model_matrix * vec4(position, 1);
		mat4 v_matrix = view_matrix;
		v_matrix[3][0]=v_matrix[3][1]=v_matrix[3][2]=0;
		epos = v_matrix * wpos;
		gl_ClipDistance[0]=dot(wpos, plane);
		gl_Position = projection_matrix * epos;
		norm = normal;
		tc = texcoord;
		break;
	case 10:
		camera_right = vec3(view_matrix[0][0],view_matrix[1][0],view_matrix[2][0]);
		camera_up = vec3(view_matrix[0][1],view_matrix[1][1],view_matrix[2][1]);

		float size_ = xyzs.w;
		wpos =vec4(vertices,1);
		billboard = (camera_right*size_*wpos.x + camera_up*size_*wpos.y);

		mat4 model_view_matrix = view_matrix* mat4(1,0,0,0, 0,1,0,0, 0,0,1,0,  xyzs.x, xyzs.y, xyzs.z, 1);
		vec4 ec_pos = model_view_matrix*vec4(billboard,1);
		gl_Position= projection_matrix*ec_pos;
	
		tc = vertices.xy/2.0f+vec2(0.5f,0.5f);
		particle_color = color;
		break;
	}
}