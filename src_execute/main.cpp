#include "cgmath.h"			// slee's simple math library
#include "cgut.h"			// slee's OpenGL utility
#include "server.h"
#include "sound.h"
//*******************************************************************
// global constants
static const char*	window_name = "cgbase - separable blur";
static const char*	vert_shader_path = "../bin_execute/shaders/blur.vert";
static const char*	frag_shader_path = "../bin_execute/shaders/blur.frag";
static const char*	image_path = "../bin_execute/images/start.jpg";
static const char* image_path_help = "../bin_execute/images/help.jpg";

//*******************************************************************
// include stb_image with the implementation preprocessor definition
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//*******************************************************************
// window objects
GLFWwindow*	window = nullptr;
ivec2		window_size = ivec2( 1024, 576 );	// initial window size

//*******************************************************************
// OpenGL objects
GLuint	program	= 0;				// ID holder for GPU program
GLuint  vertex_buffer = 0, vertex_buffer2 = 0;;
GLuint	fbo = 0;					// framebuffer objects
GLuint	SRC = 0, HELP=0;	// texture objects
void render_text(std::string, GLint, GLint, GLfloat, vec4);
void text_init();
//*******************************************************************
// global variables
int		frame=0;	// index of rendering frames
ivec2	image_size;
vec2	sigma = 0.5f;
Sound_2d* bgm;
std::string myip_show, serverIP, ip="";
bool is_server=false;
int SKILL_SET = 0;
STARTUPINFO StartupInfo = { 0 };
PROCESS_INFORMATION ProcessInfo;
bool help_print = false;
void run();
int set_round = 3;
//*******************************************************************
void update()
{
}


void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	if (help_print) {
		glUseProgram(program);
		glBindTexture(GL_TEXTURE_2D, HELP);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer2);
		cg_bind_vertex_attributes(program);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glUseProgram(program);
	glBindTexture(GL_TEXTURE_2D, SRC);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	cg_bind_vertex_attributes(program);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	render_text("My IP : " + myip_show, int(window_size.x * 0.1f), int(window_size.y * 0.7f), 0.35f, vec4(1, 1, 1, 1)); //context
	render_text("Server IP : " + serverIP, int(window_size.x * 0.1f), int(window_size.y * 0.81f), 0.35f, vec4(1, 1, 1, 1)); //context
	render_text("Round : " + std::to_string(set_round), int(window_size.x * 0.1f), int(window_size.y * 0.92f), 0.35f, vec4(1, 1, 1, 1)); //context

	glfwSwapBuffers( window );

}

void reshape( GLFWwindow* window, int width, int height )
{
	
	window_size = ivec2( width, height );
	glViewport( 0, 0, width, height );
	void update_render_target_textures( int width, int height );
	update_render_target_textures( width, height );
}

void print_help()
{
	printf( "[help]\n" );
	printf( "- press ESC or 'q' to terminate the program\n" );
	printf( "- press F1 or 'h' to see help\n" );
	printf( "- press left/right to increase/decrease horizontal blur\n" );
	printf( "- press up/down to increase/decrease vertical blur\n" );
	printf( "\n" );
}
void CharToWChar(const char* pstrSrc, wchar_t pwstrDest[])
{
	int nLen = (int)strlen(pstrSrc) + 1;
	mbstowcs(pwstrDest, pstrSrc, nLen);
}
void keyboard( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	if (action == GLFW_PRESS)
	{
		switch (key) {
		case GLFW_KEY_KP_1:
		case GLFW_KEY_1:
			serverIP += "1";
			break;
		case GLFW_KEY_KP_2:
		case GLFW_KEY_2:
			serverIP += "2";
			break;
		case GLFW_KEY_KP_3:
		case GLFW_KEY_3:
			serverIP += "3";
			break;
		case GLFW_KEY_KP_4:
		case GLFW_KEY_4:
			serverIP += "4";
			break;
		case GLFW_KEY_KP_5:
		case GLFW_KEY_5:
			serverIP += "5";
			break;
		case GLFW_KEY_KP_6:
		case GLFW_KEY_6:
			serverIP += "6";
			break;
		case GLFW_KEY_KP_7:
		case GLFW_KEY_7:
			serverIP += "7";
			break;
		case GLFW_KEY_KP_8:
		case GLFW_KEY_8:
			serverIP += "8";
			break;
		case GLFW_KEY_KP_9:
		case GLFW_KEY_9:
			serverIP += "9";
			break;
		case GLFW_KEY_KP_0:
		case GLFW_KEY_0:
			serverIP += "0";
			break;
		case GLFW_KEY_KP_DECIMAL:
		case GLFW_KEY_PERIOD:
			serverIP += ".";
			break;
		case GLFW_KEY_BACKSPACE:
			if (serverIP.length() > 0) serverIP.pop_back();
			break;
		case GLFW_KEY_EQUAL:
			if (mods && GLFW_MOD_SHIFT) {
				is_server = true;
				SKILL_SET = 0;
				bgm->stop();
				run();
			}
			break;
		case GLFW_KEY_KP_ADD:
			is_server = true;
			SKILL_SET = 0;
			bgm->stop();
			run();
			break;
		case GLFW_KEY_ENTER:
		case GLFW_KEY_KP_ENTER:
			is_server = false;
			SKILL_SET = 1;
			ip = serverIP;
			bgm->stop();
			run();
			break;
		case GLFW_KEY_H:
			help_print ^= 1;
			break;

		case GLFW_KEY_UP:
			if (set_round < 29)
				set_round += 2;
			break;
		case GLFW_KEY_DOWN:
			if(set_round>1)
				set_round -= 2;
			break;
		}
	}
}
void run() {
	wchar_t command[100];
	std::string command_ = "../bin/cgcirc.exe " + std::to_string(is_server ? 1 : 0) +" "+std::to_string(set_round) + " " + (serverIP == "" ? "0" : serverIP);
	CharToWChar(command_.c_str(), command);
	::CreateProcess(NULL, command, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo);
}
void mouse( GLFWwindow* window, int button, int action, int mods )
{
}

void motion( GLFWwindow* window, double x, double y )
{
}
void update_render_target_textures(int a, int b) {};
void texture_load(const char* image_path, GLuint* vertex_buffer, GLuint* texture_buffer,vec2 start_point, vec2 size) {
	int comp; int& width = image_size.x, & height = image_size.y;
	unsigned char* pimage0 = stbi_load(image_path, &width, &height, &comp, 3); if (comp == 1) comp = 3; /* convert 1-channel to 3-channel image */
	int stride0 = width * comp, stride1 = (stride0 + 3) & (~3);	// 4-byte aligned stride
	unsigned char* pimage = (unsigned char*)malloc(sizeof(unsigned char) * stride1 * height);
	for (int y = 0; y < height; y++) memcpy(pimage + (height - 1 - y) * stride1, pimage0 + y * stride0, stride0); // vertical flip
	stbi_image_free(pimage0); // release the original image

	// create corners and vertices
	vertex corners[4];
	corners[0].pos = vec3(start_point.x, start_point.y, 0.0f);	corners[0].tex = vec2(0.0f, 0.0f);
	corners[1].pos = vec3(start_point.x+size.x, start_point.y, 0.0f);	corners[1].tex = vec2(1.0f, 0.0f);
	corners[2].pos = vec3(start_point.x + size.x, start_point.y + size.y, 0.0f);	corners[2].tex = vec2(1.0f, 1.0f);
	corners[3].pos = vec3(start_point.x, start_point.y + size.y, 0.0f);	corners[3].tex = vec2(0.0f, 1.0f);
	vertex vertices[6] = { corners[0], corners[1], corners[2], corners[0], corners[2], corners[3] };

	// generation of vertex buffer is the same, but use vertices instead of corners
	glGenBuffers(1, vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, *vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// create a src texture (lena texture)
	glGenTextures(1, texture_buffer);
	glBindTexture(GL_TEXTURE_2D, *texture_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8 /* GL_RGB for legacy GL */, image_size.x, image_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, pimage);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	free(pimage);
}
bool user_init()
{
	// log hotkeys
	print_help();

	// init GL states
	glClearColor( 39/255.0f, 40/255.0f, 34/255.0f, 1.0f );	// set clear color
	glEnable( GL_CULL_FACE );								// turn on backface culling
	glEnable( GL_DEPTH_TEST );								// turn on depth tests
	glEnable( GL_TEXTURE_2D );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture( GL_TEXTURE0 );
	bgm = new Sound_2d("../bin_execute/sounds/lobby.mp3");
	// load and flip an image
	texture_load(image_path, &vertex_buffer, &SRC,vec2(-1,-1),vec2(2,2));
	texture_load(image_path_help, &vertex_buffer2, &HELP,vec2(-0.8f,-0.2f),vec2(1.1f*1.19f,1.1f));
	
	// release the new image
	
	text_init();
	
	char myIP[50];
	get_ip(myIP);
	myip_show = myIP;

	StartupInfo.cb = sizeof(STARTUPINFO);
	
	
	return true;
}

void user_finalize()
{
}

int main( int argc, char* argv[] )
{
	// initialization
	if(!glfwInit()){ printf( "[error] failed in glfwInit()\n" ); return 1; }

	// create window and initialize OpenGL extensions
	if(!(window = cg_create_window( window_name, window_size.x, window_size.y ))){ glfwTerminate(); return 1; }
	if(!cg_init_extensions( window )){ glfwTerminate(); return 1; }	// version and extensions

	// initializations and validations
	if(!(program=cg_create_program( vert_shader_path, frag_shader_path ))){ glfwTerminate(); return 1; }	// create and compile shaders/program
	if(!user_init()){ printf( "Failed to user_init()\n" ); glfwTerminate(); return 1; }					// user initialization

	// register event callbacks
	glfwSetWindowSizeCallback( window, reshape );	// callback for window resizing events
    glfwSetKeyCallback( window, keyboard );			// callback for keyboard events
	glfwSetMouseButtonCallback( window, mouse );	// callback for mouse click inputs
	glfwSetCursorPosCallback( window, motion );		// callback for mouse movement

	// enters rendering/event loop
	float prev_time = 0.0f;
	for( frame=0; !glfwWindowShouldClose(window); frame++ )
	{
		while ((float)glfwGetTime() - prev_time < 1.0f / 60.0f) {
			Sleep(2);
		}
		prev_time = (float)glfwGetTime();
		glfwPollEvents();	// polling and processing of events
		update();			// per-frame update
		render();			// per-frame render
	}
	
	// normal termination
	user_finalize();
	cg_destroy_window(window);

	return 0;
}
