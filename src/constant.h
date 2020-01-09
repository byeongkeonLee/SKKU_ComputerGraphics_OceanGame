#pragma once
#define TERRAIN_SHADER 1
#define WATER_SHADER 2
#define OUTLINER_SHADER 3
#define TREE_SHADER 4
#define SCENE_RADIAL_BLUR 5
#define SKILL_GUI_SHADER 6
#define SHIP_SHADER 7
#define BILLBOARD_SHADER 8
#define SKYBOX_SHADE 9
#define PARTICLE_SHADER 10

#define CAMERA_TRACKBALL false
#define CAMERA_INIT_EFFECT false

ivec2		window_size = ivec2(1024, 576);	// initial window size

#define WOODBOARD_NUM 20
#define BARREL_NUM 20 
#define TREE_NUM 2
#define SKILL_GUI_NUM 4
#define CHAIN_NUM 20
//----------------TERRAIN----------------------
#define WORLD_WIDTH 1024
#define WORLD_HEIGHT 1024

#define ZOMBIE_NUM 35
//----------------CRITICAL VARIABLE------------


#define HIT				0
#define ENEMY_HIT		1
#define CANNON			2
#define ENEMY_CANNON	3
#define SNIPER			4
#define FIRE			5

//-----------------------------------------------------------

int SKILL_SET = 1;
bool game_over = false;
bool game_closed = false;
int hp = 100;

//shield
int hp_shield = 0;
int hp_shield2 = 0;
int hp_prev = 100;


int mp = 100;
int maxhp = 100;
int maxmp = 100;

float hp_ratio_effect = 1.0f;
float mp_ratio_effect = 1.0f;
float hp2_ratio_effect = 1.0f;
float mp2_ratio_effect = 1.0f;

int lv = 1;
int exper1 = 1;
int maxexp = 3;
int sp = 2;
int round_number = 3;
bool is_dead = false;
bool is_dead2 = false;
float rebirth_time = 5.0f;
float rebirth_time_end =0.0f;

float hp_freq = 10.0f;
float hp_freq_end = hp_freq;
int hp_recovery = 2;

float mp_freq = 1.0f;
float mp_freq_end = mp_freq;
int mp_recovery = 1;

int hp2 = 100;
int mp2 = 100;
int maxhp2 = 100;
int maxmp2 = 100;
int lv2 = 1;
int exper2 = 0;
int maxexp2 = 3;

int death = 0;
int death2 = 0;

bool game_start = false;
float game_true_time = 0.0f;
bool game_var_init_called = false;
void game_var_init() {
	hp = 100;
	mp = 100;
	maxhp = 100;
	maxmp = 100;

	hp_shield = 0;
	hp_prev = 100;

	lv = 1;
	exper1 = 1;
	maxexp = 3;
	sp = 3;
	is_dead = false;
	is_dead2 = false;
	rebirth_time = 5.0f;
	rebirth_time_end = 0.0f;

	hp_freq = 4.0f;
	hp_freq_end = hp_freq;
	hp_recovery = 4;

	mp_freq = 4.0f;
	mp_freq_end = mp_freq;
	mp_recovery = 4;

	hp2 = 100;
	mp2 = 100;
	maxhp2 = 100;
	maxmp2 = 100;
	lv2 = 1;
	exper2 = 0;
	maxexp2 = 3;

	death = 0;
	death2 = 0;
	printf("Initialized\n\n");
}


bool sound_plays[6] = { false, };
bool sound_updates[6] = { false, };
bool plays_to_enemy[3] = { false, };
bool updates_to_enemy[3] = { false, };
bool stop_fire = false;
bool stop_client_fire = false;