#include "func.h"
#include "water.h"
#include "terrain.h"
#include "skycube.h"
#include "ship.h"
#include "sphere.h"
#include "camera.h"
#include "woodboard.h"
#include "barrel.h"
#include "Gui.h"
#include "tree.h"
#include "scene.h"
#include "chain.h"
#include "circle.h"
#include "barrier.h"
#include "server.h"
#include "ship2.h"
#include "hpmp.h"
#include "sniper.h"
#include "skill_help.h"
#include "sniperbullet.h"
#include "zombie.h"
#include "particles.h"
#include "GUIstatus.h"
#include "scoreboard.h"
#include "sound.h"
#include "sphere2.h"
#include "end_image.h"
#include <stdlib.h>
//*******************************************************************
// global constants
static const char*	window_name = "cgcirc";
static const char*	vert_shader_path = "../bin/shaders/circ.vert";
static const char*	frag_shader_path = "../bin/shaders/circ.frag";
static const char* image_path = "../bin/map/heightmap.jpg";
static const char* terrain_path = "../bin/map/heightmap.jpg";

void text_init();
void render_text(std::string, GLint, GLint, GLfloat, vec4);
//*******************************************************************
// window objects
GLFWwindow*	window = nullptr;

//*******************************************************************
// OpenGL objects
GLuint	program			= 0;	// ID holder for GPU program
//*******************************************************************
// global variables
int		frame = 0;				
trackball tb;
bool water_level = false;
//*******************************************************************
Water* water;
Skycube* skycube;
Sphere* sphere;
Sphere2* sphere2;
Woodboard* woodboard;
Barrel* barrel;
Gui* gui[4];
Gui* q1_2_hit_gui;
//Tree* tree;
Scene* scene;
Chain* chain;
Circle* circle;
Trap* trap;
Barrier* barrier;
HPMP* hpmp, *hpmp2;
Sniper* sniper;
Skill_help* skill_help;
Sniperbullet* sniperbullet;
Zombie* zombie;
Zombie* zombie_other;
Particles* particles;
GUIstatus* status;
Scoreboard* scoreboard;
End_image* end_image;
float gl_time = 0.0f;

Sound_2d* bgm;
Sound_2d* sail;

SoundManager* soundmanager;


void camera_update(vec3, vec3, vec3);
void camera_update();
void server_data_sendrecv() {
	//data transfer
	server_data.p1_position = ship->distance;
	server_data.p1_position_z = ship->distance_z;
	server_data.angle = ship->angle;
	server_data.crush_speed = ship->crush_speed2;
	ship->crush_speed2 = vec2(0.0f, 0.0f);
	server_data.crushed = ship->crushed2;
	ship->crushed2 = 0.0f;
	server_data.no_fire = stop_client_fire;
	stop_client_fire = false;

	for (int i = 0; i < 3; i++) {
		server_data.plays_from_enemy[i] = plays_to_enemy[i];
		server_data.updates_from_enemy[i] = updates_to_enemy[i];
		plays_to_enemy[i] = false;
		updates_to_enemy[i] = false;

	}
	for (int i = 0; i < WOODBOARD_NUM; i++) {
		server_data.woodboard.position[i] = woodboard->position[i];
		server_data.woodboard.height[i] = woodboard->height[i];
		server_data.woodboard.rotate[i] = woodboard->rotate[i];
	}
	for (int i = 0; i < BARREL_NUM; i++) {
		server_data.barrels.position[i] = barrel->position[i];
		server_data.barrels.height[i] = barrel->height[i];
		server_data.barrels.rotate[i] = barrel->rotate[i];
	}
	server_data.normal_lefts.render = normal_left->render;
	for (int i = 0; i < NUM_SPHERE; i++) {
		server_data.spheres.position[i] = sphere->position[i];
		server_data.spheres2.is_hit[i] = sphere2->is_hit[i];
	}
	server_data.spheres.radius = normal_left->radius;

	server_data.e_1s.alpha = e_1->alpha;
	server_data.e_1s.turn_on = e_1->turn_on;

	server_data.r_1s.turn_on = r_1->turn_on;
	server_data.r_1s.range = r_1->range;
	server_data.r_1s.damage = r_1->damage;

	server_data.q_1s.turn_on = q_1->turn_on;
	server_data.q_1s.position = q_1->position;
	server_data.q_1s.damage = q_1->damage;
	server_data.q_1s.sturn_time = q_1->sturn_time;
	server_data.chain_angle = chain->angle;
	server_data.chain_angle2 = chain->angle2;
	server_data.items.woodboard = woodboard->p2woodboard;
	server_data.items.barrel = barrel->p2barrel;
	server_data.death = death;
	server_data.normal_lefts.damage = normal_left->damage;

	server_data.zombies.cur = zombie->cur;
	for (int i = 0; i < zombie->cur; i++) {
		server_data.zombies.model_matrix[i] = zombie->model_matrix[i];
	}

	server_data.hp = hp;
	server_data.maxhp = maxhp;
	server_data.mp = mp;
	server_data.maxmp = maxmp;
	server_data.exper = exper1;
	server_data.maxexp = maxexp;
	server_data.lv = lv;
	server_data.round_number = round_number;
	server_data.total_speed = (ship->speed + ship->dir * ship->additional_speed) * ship->trap_slow_speed;

	e_2->turn_on = client_data.e_2s.turn_on;
	barrier->position = client_data.e_2s.position;
	barrier->radius = client_data.e_2s.radius;

	ship2->distance = client_data.p2_position;
	ship2->angle = client_data.angle;
	ship->crush_speed += client_data.crush_speed;
	ship->crushed = client_data.crushed;

	sound_plays[ENEMY_HIT] |= client_data.plays_from_enemy[0];
	sound_plays[ENEMY_CANNON] |= client_data.plays_from_enemy[1];
	sound_plays[SNIPER] |= client_data.plays_from_enemy[2];
	sound_updates[ENEMY_HIT] |= client_data.updates_from_enemy[0];
	sound_updates[ENEMY_CANNON] |= client_data.updates_from_enemy[1];
	sound_updates[SNIPER] |= client_data.updates_from_enemy[2];
	for (int i = 0; i < 3; i++) {
		client_data.plays_from_enemy[i] = false;
		client_data.updates_from_enemy[i] = false;
	}

	for (int i = 0; i < 5; i++) {
		trap->position[i] = client_data.q_2s.position[i];
		trap->dir[i] = client_data.q_2s.dir[i];
		trap->size[i] = client_data.q_2s.size[i];
		trap->available[i] = client_data.q_2s.available[i];
	}
	hp2 = client_data.hp;
	maxhp2 = client_data.maxhp;
	mp2 = client_data.mp;
	maxmp2 = client_data.maxmp;
	exper2 = client_data.exper;
	maxexp2 = client_data.maxexp;
	lv2 = client_data.lv;
	hp_shield2 = client_data.hp_shield;
	death2 = client_data.death;

	q_1->hit_client= client_data.q_1s.hit;
	q_2->slow = client_data.q_2s.trap_slow_down;
	q_2->damage = client_data.q_2s.damage;

	r_2->shot=client_data.r_2s.shot;
	r_2->damage= client_data.r_2s.damage;
	r_2->hit = client_data.r_2s.hit;
	sniperbullet->position = client_data.sniper_bullets.pos;
	sniperbullet->is_render = client_data.sniper_bullets.render;
	ship2->distance_z=client_data.p2_position_z;

	zombie_other->cur = client_data.zombies.cur;
	for (int i = 0; i < zombie_other->cur; i++) {
		zombie_other->model_matrix[i] = client_data.zombies.model_matrix[i];
	}
	sphere2->is_render = client_data.normal_lefts.render;
	for (int i = 0; i < NUM_SPHERE; i++) {
		sphere2->position[i] = client_data.spheres.position[i];
		sphere->is_hit[i] = client_data.spheres2.is_hit[i];

	}
	sphere2->damage=client_data.normal_lefts.damage;
	sphere2->radius = client_data.spheres.radius;
	server_data.game_over = game_over;
	game_start = client_data.game_start;
}
void client_data_sendrecv() {
	client_data.p2_position = ship->distance;
	client_data.p2_position_z = ship->distance_z;
	client_data.angle = ship->angle;
	client_data.crush_speed = ship->crush_speed2;
	client_data.game_start = true;
	ship->crush_speed2 = vec2(0.0f, 0.0f);
	client_data.crushed = ship->crushed2;
	ship->crushed2 = 0.0f;

	for (int i = 0; i < 3; i++) {
		client_data.plays_from_enemy[i] = plays_to_enemy[i];
		client_data.updates_from_enemy[i] = updates_to_enemy[i];
		plays_to_enemy[i] = false;
		updates_to_enemy[i] = false;

	}

	client_data.e_2s.position = barrier->position;
	client_data.e_2s.radius = barrier->radius;
	client_data.e_2s.turn_on = e_2->turn_on;

	client_data.hp = hp;
	client_data.maxhp = maxhp;
	client_data.mp = mp;
	client_data.maxmp = maxmp;
	client_data.lv = lv;
	client_data.hp_shield = hp_shield;
	client_data.exper = exper1;
	client_data.maxexp = maxexp;
	client_data.death = death;
	client_data.q_1s.hit = q_1->hit_client;
	client_data.q_2s.trap_slow_down = q_2->slow;
	client_data.q_2s.damage = q_2->damage;
	

	client_data.r_2s.shot = r_2->shot;
	client_data.r_2s.damage = r_2->damage;
	client_data.sniper_bullets.pos = sniperbullet->position;
	client_data.sniper_bullets.render = sniperbullet->is_render;
	client_data.normal_lefts.damage = normal_left->damage;
	
	client_data.zombies.cur = zombie->cur;
	for (int i = 0; i < zombie->cur; i++) {
		client_data.zombies.model_matrix[i] = zombie->model_matrix[i];
	}

	client_data.normal_lefts.render = normal_left->render;
	for (int i = 0; i < NUM_SPHERE; i++) {
		client_data.spheres.position[i] = sphere->position[i];
		client_data.spheres2.is_hit[i] = sphere2->is_hit[i];

	}
	client_data.spheres.radius = normal_left->radius;

	q_1->damage = server_data.q_1s.damage;
	e_1->alpha = server_data.e_1s.alpha;
	e_1->turn_on = server_data.e_1s.turn_on;

	for (int i = 0; i < 5; i++) {
		client_data.q_2s.position[i] = trap->position[i];
		client_data.q_2s.dir[i] = trap->dir[i];
		client_data.q_2s.size[i] = trap->size[i];
		client_data.q_2s.available[i] = trap->available[i];
	}


	ship2->distance = server_data.p1_position;
	ship2->distance_z = server_data.p1_position_z;
	ship2->angle = server_data.angle;
	ship->crush_speed += server_data.crush_speed;
	ship->crushed = server_data.crushed;
	round_number = server_data.round_number;

	sound_plays[ENEMY_HIT] |= server_data.plays_from_enemy[0];
	sound_plays[ENEMY_CANNON] |= server_data.plays_from_enemy[1];
	sound_plays[FIRE] |= server_data.plays_from_enemy[2];
	sound_updates[ENEMY_HIT] |= server_data.updates_from_enemy[0];
	sound_updates[ENEMY_CANNON] |= server_data.updates_from_enemy[1];
	sound_updates[FIRE] |= server_data.updates_from_enemy[2];
	for (int i = 0; i < 3; i++) {
		server_data.plays_from_enemy[i] = false;
		server_data.updates_from_enemy[i] = false;
	}

	stop_fire |= server_data.no_fire;

	woodboard->p2woodboard= server_data.items.woodboard;
	barrel->p2barrel=server_data.items.barrel;

	for (int i = 0; i < WOODBOARD_NUM; i++) {
		woodboard->position[i] = server_data.woodboard.position[i];
		woodboard->height[i] = server_data.woodboard.height[i];
		woodboard->rotate[i] = server_data.woodboard.rotate[i];
	}
	for (int i = 0; i < BARREL_NUM; i++) {
		barrel->position[i] = server_data.barrels.position[i];
		barrel->height[i] = server_data.barrels.height[i];
		barrel->rotate[i] = server_data.barrels.rotate[i];
	}
	sphere2->is_render = server_data.normal_lefts.render;
	for (int i = 0; i < NUM_SPHERE; i++) {
		sphere2->position[i] = server_data.spheres.position[i];
		sphere->is_hit[i] = server_data.spheres2.is_hit[i] ;
	}
	sphere2->radius = server_data.spheres.radius;

	q_1->turn_on = server_data.q_1s.turn_on;
	q_1->position = server_data.q_1s.position;
	q_1->sturn_time = server_data.q_1s.sturn_time;


	r_1->turn_on = server_data.r_1s.turn_on;
	r_1->range = server_data.r_1s.range;
	r_1->damage = server_data.r_1s.damage;

	chain->angle = server_data.chain_angle;
	chain->angle2 = server_data.chain_angle2;

	zombie_other->cur = server_data.zombies.cur;
	for (int i = 0; i < zombie_other->cur; i++) {
		zombie_other->model_matrix[i] = server_data.zombies.model_matrix[i];
	}

	hp2 = server_data.hp;
	maxhp2 = server_data.maxhp;
	mp2 = server_data.mp;
	maxmp2 = server_data.maxmp;
	exper2 = server_data.exper;
	maxexp2 = server_data.maxexp;
	death2= server_data.death;
	lv2 = server_data.lv;
	ship2->total_speed = server_data.total_speed;
	game_over = server_data.game_over;
	sphere2->damage = server_data.normal_lefts.damage;
}
void server_update()
{
	if (game_start && !game_var_init_called) {
		game_var_init_called = true;
		game_var_init();
	}
	hp_ratio_effect += (1.0f * hp / maxhp - hp_ratio_effect)*0.02f;
	mp_ratio_effect += (1.0f * mp / maxmp - mp_ratio_effect) * 0.02f;
	hp2_ratio_effect += (1.0f * hp2 / maxhp2 - hp2_ratio_effect) * 0.02f;
	mp2_ratio_effect += (1.0f * mp2 / maxmp2 - mp2_ratio_effect) * 0.02f;

	status->update(ship->distance, hp, maxhp, hp_shield,mp, maxmp, exper1, maxexp);
	// update simulation
	gl_time = (float)glfwGetTime();
	ship->update(gl_time,zombie);
	zombie->update(gl_time);
	
	particles->update(ship->distance, gl_time, cam.eye, ship->angle, ship->total_speed);
	if(!CAMERA_TRACKBALL)
		camera_update();

	woodboard->update(gl_time,ship->distance,ship2->distance);
	water->update(gl_time);
	barrel->update(gl_time,ship->distance, ship2->distance);
	for (int i = 0; i < SKILL_GUI_NUM; i++)
		gui[i]->update(gl_time);
	if(q_1->hit_server)
		q1_2_hit_gui->update(gl_time);


	if (normal_left->turn_on)
		circle->update(ship->distance, ship->angle, normal_left->range);
	if (normal_left->render) {
		sphere->update(ship->distance, ship->angle,gl_time,ship->total_speed);
	}
	sphere2->update(ship->distance, ship->angle, sphere2->is_render);
	if(q_1->turn_on)
		chain->update(gl_time);

	if (e_2->turn_on)
		barrier->update(ship->distance,gl_time);


	camera_update(cam.eye,cam.at,cam.up);
	hpmp->update(ship->distance, hp, maxhp, hp_shield, mp, maxmp, exper1,maxexp);

	if (death > round_number / 2 || death2 > round_number / 2) {
		game_over = true;
	}

	server_data_sendrecv();

	hpmp2->update(ship2->distance, hp2, maxhp2, hp_shield2, mp2, maxmp2,exper2,maxexp2);
	ship2->update(gl_time);	
}
void client_update() {
	hp_prev = hp;
	hp_ratio_effect += (1.0f * hp / maxhp - hp_ratio_effect) * 0.02f;
	mp_ratio_effect += (1.0f * mp / maxmp - mp_ratio_effect) * 0.02f;
	hp2_ratio_effect += (1.0f * hp2 / maxhp2 - hp2_ratio_effect) * 0.02f;
	mp2_ratio_effect += (1.0f * mp2 / maxmp2 - mp2_ratio_effect) * 0.02f;

	gl_time = (float)glfwGetTime();
	particles->update(ship2->distance, gl_time, cam.eye, ship2->angle, (ship2->speed + ship2->dir * ship2->additional_speed) * ship2->total_speed);
	status->update(ship->distance, hp, maxhp, hp_shield, mp, maxmp, exper1, maxexp);
	ship->update(gl_time,zombie);
	zombie->update(gl_time);
	if (!CAMERA_TRACKBALL)
		camera_update();


	if (normal_left->turn_on)
		circle->update(ship->distance, ship->angle, normal_left->range);
	if (normal_left->render)
		sphere->update(ship->distance, ship->angle,gl_time,ship->total_speed);

	barrier->update(ship->distance, gl_time);
	trap->update(gl_time);
	water->update(gl_time);
	hpmp->update(ship->distance, hp, maxhp, hp_shield, mp, maxmp,exper1,maxexp);

	sphere2->update(ship->distance, ship->angle, sphere2->is_render);

	if(sniperbullet->is_render)
		client_data.r_2s.hit = sniperbullet->update(gl_time, vec3(ship2->distance,0.0f));

	for (int i = 0; i < SKILL_GUI_NUM; i++)
		gui[i]->update(gl_time);

	client_data_sendrecv();

	camera_update(cam.eye, cam.at, cam.up);
	hpmp2->update(ship2->distance, hp2, maxhp2, hp_shield2, mp2, maxmp2,exper2,maxexp2);
	barrel->player2update();
	woodboard->player2update();
	ship2->update(gl_time);
	/*if (death > round_number / 2 || death2 > round_number / 2) {
		game_over = true;
	}*/
	if (hp_shield > 0 && hp < hp_prev) {
		if (hp_prev - hp < hp_shield) {
			hp_shield -= hp_prev - hp;
			hp += hp_prev - hp;
		}
		else {
			hp += hp_shield;
			hp_shield = 0;
		}
	}
}
void camera_update(vec3 eye, vec3 at, vec3 up) {
	cam.view_matrix = mat4::look_at(eye, at, up);
	cam.aspect_ratio = window_size.x / float(window_size.y);
	GLint uloc;
	uloc = glGetUniformLocation(program, "aspect_ratio");			if (uloc > -1) glUniform1f(uloc, cam.aspect_ratio);		
	uloc = glGetUniformLocation(program, "view_matrix");			if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.view_matrix);		// update the view matrix (covered later in viewing lecture)

	if (!r_2->turn_on) 
		cam.projection_matrix = mat4::perspective(cam.fovy, cam.aspect_ratio, cam.dNear, cam.dFar);
	else 
		cam.projection_matrix = mat4::perspective(r_2->zoom, cam.aspect_ratio, cam.dNear, cam.dFar);
	uloc = glGetUniformLocation(program, "projection_matrix");	if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.projection_matrix);	// update the projection matrix (covered later in viewing lecture)
}
void camera_update() {
	cam.eye += (cam.set_eye - cam.eye) * cam.move_spd;
	cam.at += (cam.set_at - cam.at) * cam.move_spd;
	cam.up += (cam.set_up - cam.up) * cam.move_spd;
}


void render()
{
	glUseProgram(program);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glUniform1f(glGetUniformLocation(program, "thick"), 0);
	glUniform1i(glGetUniformLocation(program, "grayscale"), hp<=0);
	terrain->render(program);
	skycube->render(program);
	ship->render(program);

	woodboard->render(program);
	barrel->render(program);
	//tree->render(program);
	zombie->render(program);
	zombie_other->render(program);
	if(normal_left->turn_on)
		circle->render(program);
	
	if (sphere2->is_render)
		sphere2->render(program);
	
	if (normal_left->render) {
		sphere->render(program);
	}
	if(q_1->turn_on)
		chain->render(program);
	
	trap->render(program);
	ship2->render(program);

	if (!water_level ) {
		hpmp->render(program, hp_ratio_effect, mp_ratio_effect);
		if(SKILL_SET == 0 || (SKILL_SET == 1 && !e_1->turn_on)) hpmp2->render(program,hp2_ratio_effect,mp2_ratio_effect);
	}
	if(sniperbullet->is_render)
		sniperbullet->render(program);
	if (r_2->start_switch != sniperbullet->is_render) {
		if (r_2->start_switch == false) r_2->hit_init = false;
		r_2->start_switch = sniperbullet->is_render;
	}
	if (e_2->turn_on)
		barrier->render(program);
	if(water_level==true)
		particles->render(program);
}
void render_gui() {
	if (SKILL_SET == 0) {
		if (!q_1->hit_server) {
			gui[0]->render(program);
		}
		else {
			q1_2_hit_gui->render(program);
		}
	}
	else {
		gui[0]->render(program);
	}
	for(int i=1;i<SKILL_GUI_NUM;i++)
		gui[i]->render(program);
}

void reshape( GLFWwindow* window, int width, int height )
{
	window_size = ivec2(width,height);
	glViewport( 0, 0, width, height );
}

void print_help()
{
	printf( "[help]\n" );
}

void keyboard( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	vec3 myship_shifted = vec3(ship->distance, 0.0f) - r_2->cameye;
	vec3 yourship_shifted = vec3(ship2->distance, 0.0f) - r_2->cameye;
	vec3 aim = r_2->camat - r_2->cameye;
	if(action==GLFW_PRESS)
	{
		switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE); break;
		case GLFW_KEY_F10:
			print_help(); break;
		case GLFW_KEY_A: ship->key.a = true; break;
		case GLFW_KEY_S: ship->key.s = true; break;
		case GLFW_KEY_D: ship->key.d = true; break;
		case GLFW_KEY_W: ship->key.w = true; break;
		case GLFW_KEY_L: ship->level_up(); break;
		case GLFW_KEY_B: ship->angle += PI; break;
		case GLFW_KEY_C:
			/*printf("[eye : %lf %lf %lf]\n", cam.eye.x, cam.eye.y, cam.eye.z);
			printf("[at : %lf %lf %lf]\n", cam.at.x, cam.at.y, cam.at.z);
			printf("[up : %lf %lf %lf]\n", cam.up.x, cam.up.y, cam.up.z);*/
			printf("[eye : %lf %lf %lf]\n", r_2->cameye.x, r_2->cameye.y, r_2->cameye.z);
			printf("[at : %lf %lf %lf]\n", r_2->camat.x, r_2->camat.y, r_2->camat.z);
			printf("[ship_pos : %lf %lf]\n", ship->distance.x,ship->distance.y);
			printf("[ship2_pos : %lf %lf]\n", ship2->distance.x, ship2->distance.y);

			printf("[aim : %lf %lf %lf]\n", aim.x, aim.y, aim.z);
			break;
		case GLFW_KEY_SPACE:
			if (mods && GLFW_MOD_CONTROL) {
				if (sp <= 0) break;
				if (SKILL_SET == 0)
					sp+=space_1->level_up();
				else
					sp+=space_2->level_up();
			}
			else {
				ship->key.space = true;
			}break;
		case GLFW_KEY_1:
			if (mods && GLFW_MOD_CONTROL) {
				if (sp <= 0) break;
				if (SKILL_SET == 0)
					sp += q_1->level_up();
				else
					sp += q_2->level_up();
			}
			else {
				ship->key._1 = true;
			}
			break;
		case GLFW_KEY_3: 
			if (mods && GLFW_MOD_CONTROL) {
				if (sp <= 0) break;
				if (SKILL_SET == 0)
					sp+= e_1->level_up();
				else
					sp += e_2->level_up();
			}
			else {
				ship->key._3 = true;
			}
			 break;
		case GLFW_KEY_4:
		if (mods && GLFW_MOD_CONTROL) {
			if (sp <= 0) break;
			if (SKILL_SET == 0)
				sp +=r_1->level_up();
			else
				sp+=r_2->level_up();
		}else {
			ship->key._4 = true;
		}
					   break;
		case GLFW_KEY_F1:
			if (skill_help->skill_help_num == Skill_Q_1_code || skill_help->skill_help_num == Skill_Q_2_code)
				skill_help->skill_help_num = -1;
			else
				skill_help->skill_help_num = SKILL_SET == 0 ? Skill_Q_1_code : Skill_Q_2_code;
			break;
		case GLFW_KEY_F2:
			if (skill_help->skill_help_num == Skill_E_1_code || skill_help->skill_help_num == Skill_E_2_code)
				skill_help->skill_help_num = -1;
			else
				skill_help->skill_help_num = SKILL_SET == 0 ? Skill_E_1_code : Skill_E_2_code;
			break;
		case GLFW_KEY_F3:
			if (skill_help->skill_help_num == Skill_R_1_code || skill_help->skill_help_num == Skill_R_2_code)
				skill_help->skill_help_num = -1;
			else
				skill_help->skill_help_num = SKILL_SET == 0 ? Skill_R_1_code : Skill_R_2_code;
			break;
		case GLFW_KEY_F4:
			if (skill_help->skill_help_num == Skill_space_1_code || skill_help->skill_help_num == Skill_space_2_code)
				skill_help->skill_help_num = -1;
			else
				skill_help->skill_help_num = SKILL_SET == 0 ? Skill_space_1_code : Skill_space_2_code;
			break;
		case GLFW_KEY_F11:
			hp = -1;
			break;
		case GLFW_KEY_PAUSE:
			game_closed = true;
			break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		switch (key) {
		case GLFW_KEY_A: ship->key.a = false; break;
		case GLFW_KEY_S: ship->key.s = false; break;
		case GLFW_KEY_D: ship->key.d = false; break;
		case GLFW_KEY_W: ship->key.w = false; break;
		case GLFW_KEY_SPACE: ship->key.space = false; break;
		case GLFW_KEY_1: ship->key._1 = false; break;
		case GLFW_KEY_3: ship->key._3 = false; break;
		case GLFW_KEY_4:ship->key._4 = false; break;
		}
	}
}

void mouse( GLFWwindow* window, int button, int action, int mods )
{
	dvec2 pos; glfwGetCursorPos(window, &pos.x, &pos.y);
	vec2 npos = vec2(float(pos.x) / float(window_size.x - 1), float(pos.y) / float(window_size.y - 1));
	//cam.view_matrix = tb.update_trackball(npos.x, npos.y);

	if (CAMERA_TRACKBALL) {
		if (action == GLFW_PRESS)			tb.begin(cam.view_matrix, npos.x, npos.y, button, mods);
		else if (action == GLFW_RELEASE)	tb.end();
	}

	if (action == GLFW_PRESS) {
		if (!r_2->turn_on && button == GLFW_MOUSE_BUTTON_LEFT) {
			if ((float)glfwGetTime() > default_skill_info[Skill_Normal_Left_code].cooldown_end&& mp >= default_skill_info[Skill_Normal_Left_code].wasted_mp) {
				normal_left->turn_on = true;
				normal_left->range = normal_left->init_range;
				normal_left->radius = normal_left->init_radius;
				default_skill_info[Skill_Normal_Left_code].cooldown_end = (float)glfwGetTime() + default_skill_info[Skill_Normal_Left_code].cooldown;
				mp -= default_skill_info[Skill_Normal_Left_code].wasted_mp;
			}
		}

		if (r_2->turn_on &&  button == GLFW_MOUSE_BUTTON_LEFT && tb.mods == 0 && r_2->reshot_end < (float)glfwGetTime()) {
			sound_plays[SNIPER] = true;
			sound_updates[SNIPER] = true;
			plays_to_enemy[2] = true;
			updates_to_enemy[2] = true;
			sniperbullet->init(cam.eye, cam.at);
			sniperbullet->is_render = true;
			cam.at += cam.up * 30.0f;
			r_2->shot--;
			client_data.r_2s.hit = false;
			r_2->reshot_end = r_2->reshot_time + (float)glfwGetTime();
			if (r_2->shot == 0) {
				r_2->turn_on = false;
			}
		}
	}else if(action == GLFW_RELEASE){
		if (normal_left->turn_on) {
			normal_left->attack = true;
			normal_left->render = true;
			normal_left->turn_on = false;
			normal_left->render_time_end = (float)glfwGetTime() + normal_left->render_time;
			sound_plays[CANNON] = true;
			sound_updates[CANNON] = true;
			plays_to_enemy[1] = true;
			updates_to_enemy[1] = true;
		}
	}
}

void motion( GLFWwindow* window, double x, double y )
{
	cur = vec2(3 - float(x * 6) / float(window_size.x - 1), 3 - float(y * 6) / float(window_size.y - 1));
	if (!tb.is_tracking()) return;
	vec2 npos = vec2(float(x) / float(window_size.x - 1), float(y) / float(window_size.y - 1));
	//cam.view_matrix = tb.update_trackball(npos.x, npos.y);

	if (!r_2->turn_on && CAMERA_TRACKBALL) {
		if (tb.button == GLFW_MOUSE_BUTTON_LEFT && tb.mods == 0) {
			cam.view_matrix = tb.update_trackball(npos.x, npos.y);
		}
		else if (tb.button == GLFW_MOUSE_BUTTON_RIGHT || (tb.button == GLFW_MOUSE_BUTTON_LEFT && (tb.mods & GLFW_MOD_SHIFT))) {
			cam.view_matrix = tb.update_zoom(npos.x, npos.y);
		}
		else if (tb.button == GLFW_MOUSE_BUTTON_MIDDLE || (tb.button == GLFW_MOUSE_BUTTON_LEFT && (tb.mods & GLFW_MOD_CONTROL))) {
			if (r_2->turn_on) r_2->zoom = 30.0f;
			cam.view_matrix = tb.update_panning(npos.x, npos.y);
		}
	}
	//glfwSetCursorPos(window, window_size.x / 2, window_size.y / 2);
}
void scroll(GLFWwindow* window, double x, double y) {
	if (r_2->turn_on ) {
		if (y > 0 && r_2->zoom > PI / 32) {
			r_2->zoom -= (PI/4 - PI/32)*0.05f;
			r_2->damage_step++;
		}
		else if(y<0 && r_2->zoom < PI / 4) { 
			r_2->zoom += (PI / 4 - PI / 32) * 0.05f;
			r_2->damage_step--;
		}
		r_2->damage = int(r_2->init_damage *(2.0f-r_2->damage_step*0.05f));
	}
}
bool user_init()
{
	// log hotkeys
	print_help();

	// init GL states
	glLineWidth( 1.0f );
	glClearColor( 39/255.0f, 40/255.0f, 34/255.0f, 1.0f );	// set clear color
	glEnable( GL_CULL_FACE );								// turn on backface culling
	glEnable( GL_DEPTH_TEST );								// turn on depth tests
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_STENCIL_TEST);
	glEnable(GL_CLIP_DISTANCE0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// define the position of four corner vertices
	terrain = new Terrain(terrain_path, 256, 256);
	skycube = new Skycube();
	trap = new Trap();
	ship = new Ship(trap);
	sphere = new Sphere();
	sphere2 = new Sphere2();
	woodboard = new Woodboard();
	barrel = new Barrel();
	//tree = new Tree();
	chain = new Chain();
	circle = new Circle();
	barrier = new Barrier();
	ship2 = new Ship2();
	hpmp = new HPMP();
	hpmp2 = new HPMP();
	sniper = new Sniper();
	sniperbullet = new Sniperbullet();
	skill_help = new Skill_help();
	zombie = new Zombie();
	zombie_other = new Zombie();
	particles = new Particles();
	status = new GUIstatus();
	scoreboard = new Scoreboard();
	end_image = new End_image();
	if (SKILL_SET == 1) {
		ship->distance = vec2(0.0f, 150.0f);
		ship->angle = -PI;
	}else {
		ship->distance = vec2(0.0f, -150.0f);
	}
	for (int i = 0; i < SKILL_GUI_NUM; i++) 
		gui[i] = new Gui(vec2(-1.4f+i*0.2f, -1.0f),i);
	q1_2_hit_gui = new Gui(vec2(-1.4f, -1.0f), 9);

	scene = new Scene();
	text_init();
	soundmanager = new SoundManager();
	bgm = new Sound_2d("../bin/sound/bgm.mp3");
	sail = new Sound_2d("../bin/sound/sail.mp3");
	water = new Water(window_size.x, window_size.y);
	return true;
}

void user_finalize()
{
	water->finalize();
}

int main( int argc, char* argv[] )
{

	srand((unsigned int)time(NULL));
	// initialization
	int sevcle;
	printf("<%d>", argc);
	//printf("((%s %s))", argv[1], argv[2]);
	if (argc != 4) {
		printf("Select the mode : 1. Server, 2. Client, 3. Client with Server IP");
		scanf("%d", &sevcle);
		if (sevcle == 3) {
			printf("Enter server IP");
			scanf("%s", ip);
		}
	}
	else {
		sevcle = atoi(argv[1]);
		round_number = atoi(argv[2]);
		if (sevcle == 0)
			strcpy(ip, argv[3]);
	}
	is_server = sevcle == 1 ? true : false;
	SKILL_SET = sevcle == 1 ? 0 : 1;

	thread server_thread(server_init);
	//server_thread.detach();

	if(!glfwInit()){ printf( "[error] failed in glfwInit()\n" ); return 1; }

	// create window and initialize OpenGL extensions
	if(!(window = cg_create_window( window_name, window_size.x, window_size.y ))){ glfwTerminate(); return 1; }
	if(!cg_init_extensions( window )){ glfwTerminate(); return 1; }	// init OpenGL extensions

	// initializations and validations of GLSL program
	if(!(program=cg_create_program( vert_shader_path, frag_shader_path ))){ glfwTerminate(); return 1; }	// create and compile shaders/program
	if(!user_init()){ printf( "Failed to user_init()\n" ); glfwTerminate(); return 1; }					// user initialization

	// register event callbacks
	glfwSetWindowSizeCallback( window, reshape );	// callback for window resizing events
    glfwSetKeyCallback( window, keyboard );			// callback for keyboard events
	glfwSetMouseButtonCallback( window, mouse );	// callback for mouse click inputs
	glfwSetCursorPosCallback( window, motion );		// callback for mouse movements
	glfwSetScrollCallback(window,scroll);
	// enters rendering/event loop
	float prev_time = 0.0f;
	for( frame=0; !glfwWindowShouldClose(window) && !game_closed; frame++ )
	{
		glfwPollEvents();	// polling and processing of events
		if (!game_over) {
			if (is_server)
				server_update();			// per-frame update
			else
				client_update();
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CLIP_DISTANCE0);
		glUseProgram(program);
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);
		
		while ((float)glfwGetTime() - prev_time < 1.0f / 60.0f) {
			Sleep(2);
		}
		prev_time = (float)glfwGetTime();
		//PART1
		water_level = true;
		camera_update(vec3(cam.eye.x, cam.eye.y, -cam.eye.z), vec3(cam.at.x,cam.at.y, -cam.at.z), vec3(-cam.up.x,-cam.up.y, cam.up.z));
		bindFrameBuffer(water->reflectionFramebuffer, window_size.x, window_size.y);
		glUniform4fv(glGetUniformLocation(program, "plane"), 1,vec4(0,0,1,0));
		render();			
		unbindCurrentFrameBuffer();
		


		//PART2

		camera_update(cam.eye, cam.at, cam.up);
		bindFrameBuffer(water->refractionFramebuffer, window_size.x, window_size.y);
		glUniform4fv(glGetUniformLocation(program, "plane"), 1, vec4(0, 0, -1, 5));
		render();
		
		unbindCurrentFrameBuffer();

		glDisable(GL_CLIP_DISTANCE0);
		
		//PART3
		water_level = false;
		if (cam.radial_blur && !game_over) {
			bindFrameBuffer(scene->Framebuffer, window_size.x, window_size.y);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, scene->texture_buffer, 0);
			render();
			water->render(program);
			particles->render(program);
			unbindCurrentFrameBuffer();
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 0, 0);
			scene->render(program);
			unbindCurrentFrameBuffer();
		}
		else {
			render();
			water->render(program);
			particles->render(program);
		}

		soundmanager->update(sound_updates);
		soundmanager->play(sound_plays);
		if (stop_fire) {
			soundmanager->stop_fire();
			stop_fire = false;
		}
		if (game_over) {
			if (death2 > death) {//win
				end_image->render(program, true);
			}
			else {//lose
				end_image->render(program, false);
			}
		}
		else {

			
			glUseProgram(program);
			render_gui();


			if (r_2->turn_on) {
				glUseProgram(program);
				sniper->render(program);
			}

			glUseProgram(program);
			scoreboard->render(program);

			glUseProgram(program);
			status->render(program, render_text, hp_ratio_effect, mp_ratio_effect);
			glUseProgram(program);
			if (skill_help->explain && !cam.radial_blur) {
				skill_help->render(program, render_text);
			}

			render_text("Level : " + std::to_string(lv), int(window_size.x * 0.05f), int(window_size.y * 0.07f), 0.4f, vec4(0, 0, 0, 1)); //context
			render_text("SP : " + std::to_string(sp), int(window_size.x * 0.05f), int(window_size.y * 0.2f), 0.25f, vec4(0, 0, 0, 1)); //contex
			render_text(std::to_string(death2), int(window_size.x * 0.45f), int(window_size.y * 0.06f), 0.7f, vec4(0, 0, 1, 1)); //context
			render_text(" vs ", int(window_size.x * 0.49f), int(window_size.y * 0.06f), 0.25f, vec4(0, 0, 0, 1)); //context
			render_text(std::to_string(death), int(window_size.x * 0.52f), int(window_size.y * 0.06f), 0.5f, vec4(1, 0, 0, 1)); //context
			render_text(" / " + std::to_string(round_number / 2 + 1) + "R", int(window_size.x * 0.54f), int(window_size.y * 0.06f), 0.3f, vec4(0, 0, 0, 1)); //context

		}
		glUseProgram(program);

		
		glfwSwapBuffers(window);
		
	}
	if (game_start && server_thread.joinable()) {
		printf("[Server thread try to join]\n");
		server_thread.join();
		printf("[Server thread is joined]\n");
	}
	server_thread.~thread();
	// normal termination
	user_finalize();
	cg_destroy_window(window);
	return 0;
}