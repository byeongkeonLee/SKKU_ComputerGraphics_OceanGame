#pragma once
#include "cgmath.h"			// slee's simple math library
#include "cgut.h"			// slee's OpenGL utility
#include "func.h"
#include <string.h>

#define Skill_Q_1_code 0
#define Skill_E_1_code 1
#define Skill_R_1_code 2
#define Skill_space_1_code 3

#define Skill_Q_2_code 4
#define Skill_E_2_code 5
#define Skill_R_2_code 6
#define Skill_space_2_code 7

#define Skill_Normal_Left_code 8

struct default_skill_info{
	std::string skill_name;
	std::string context;
	std::string context2;
	float cooldown = 0.0f;
	float cooldown_end = 0;
	int wasted_mp;
	int level;
}default_skill_info[10];
class Skill_space_1  {
public:
	float duration = 0.7f;
	float duration_end ;
	float add_speed = 2.1f;
	bool  turn_on = false;
	 
	Skill_space_1() {
		default_skill_info[Skill_space_1_code].skill_name = "Dash";
		default_skill_info[Skill_space_1_code].context = "Increase speed by " + std::to_string(add_speed) + " for " + std::to_string(duration) + " second";
		default_skill_info[Skill_space_1_code].context2 = "(Key SPACE)";
		default_skill_info[Skill_space_1_code].wasted_mp = 4;
		default_skill_info[Skill_space_1_code].cooldown = 10.0f;
		default_skill_info[Skill_space_1_code].level = 1;
	}
	int level_up() {
		if (default_skill_info[Skill_space_1_code].level == 8) return 0;
		default_skill_info[Skill_space_1_code].level += 1;
		default_skill_info[Skill_space_1_code].wasted_mp += 2;
		default_skill_info[Skill_space_1_code].cooldown -= 0.5f;
		duration += 0.2f;
		add_speed += 0.2f;
		default_skill_info[Skill_space_1_code].context = "Increase speed by " + std::to_string(add_speed) + " for " + std::to_string(duration) + " second";
		return -1;
	}
};
class Skill_Q_1 {
public:
	vec2 dir;
	vec2 ship_spd;
	vec2 position;
	vec2 init_position;
	float launch_spd = 2.5f;
	float range = 540.0f;
	bool  turn_on = false;
	int damage = 40;
	float sturn_time = 2.0f;
	float hit_range = 18.0f;
	bool hit_client = false;
	bool hit_server = false;
	float hit_run_time = 3.0f;
	float hit_run_time_end = 3.0f;
	Skill_Q_1() {
		default_skill_info[Skill_Q_1_code].skill_name = "Anchor";
		default_skill_info[Skill_Q_1_code].context = "Throw anchor at " + std::to_string(launch_spd) + " speed, for " + std::to_string(int(range)) + " range, dealing " +std::to_string(damage)+" damage.";
		default_skill_info[Skill_Q_1_code].context2 = "If enemy or terrain hit, enmey sturn "+std::to_string(sturn_time)+" sec, and you can dash to anchor (Key 1)";
		default_skill_info[Skill_Q_1_code].wasted_mp = 10;
		default_skill_info[Skill_Q_1_code].cooldown = 8.0f;
		default_skill_info[Skill_Q_1_code].level = 1;
	}
	int level_up() {
		if (default_skill_info[Skill_Q_1_code].level == 8) return 0; 
		default_skill_info[Skill_Q_1_code].level += 1;
		default_skill_info[Skill_Q_1_code].wasted_mp += 2;
		default_skill_info[Skill_Q_1_code].cooldown -= 1.0f;
		range += 80.0f;
		launch_spd += 0.1f;
		damage += 2;
		sturn_time += 0.4f;
		default_skill_info[Skill_Q_1_code].context = "Throw anchor at " + std::to_string(launch_spd) + " speed, for " + std::to_string(int(range)) + " range, dealing " + std::to_string(damage) + " damage.";
		return -1;
	}
};
class Skill_E_1 {
public:
	bool  turn_on = false;
	float duration = 3.0f;
	float duration_end;
	float alpha = 0.0f;
	Skill_E_1() {
		default_skill_info[Skill_E_1_code].skill_name = "Stealth";
		default_skill_info[Skill_E_1_code].context = "Hide our ship for " + std::to_string(duration) + " sec. Enemy can see our ship " + std::to_string(int(alpha)) + "%.";
		default_skill_info[Skill_E_1_code].context2 = "(Key 3)";
		default_skill_info[Skill_E_1_code].wasted_mp = 6;
		default_skill_info[Skill_E_1_code].cooldown = 8.0f;
		default_skill_info[Skill_E_1_code].level = 1;
	}
	int level_up() {
		if (default_skill_info[Skill_E_1_code].level == 8) return 0;
		default_skill_info[Skill_E_1_code].level += 1;
		default_skill_info[Skill_E_1_code].wasted_mp -= 1;
		duration += 1.0f;
		default_skill_info[Skill_E_1_code].context = "Hide our ship for " + std::to_string(duration) + " sec. Enemy can see our ship " + std::to_string(int(alpha)) + "%.";
		return -1;
	}
};
class Skill_R_1 {
public:
	bool turn_on = false;
	float duration = 5.0f;
	float duration_end;
	float range = 16.0f;
	int damage = 2;
	Skill_R_1() {
		default_skill_info[Skill_R_1_code].skill_name = "FlameThrower";
		default_skill_info[Skill_R_1_code].context = "Fire Flame for " + std::to_string(duration) + " sec, dealing "+std::to_string(damage)+"/0.1s. The range is" + std::to_string(int(range)) + ".";
		default_skill_info[Skill_R_1_code].context2 = "(Key 4)";
		default_skill_info[Skill_R_1_code].wasted_mp = 15;
		default_skill_info[Skill_R_1_code].cooldown = 1.0f;
		default_skill_info[Skill_R_1_code].level = 1;
	}
	int level_up() {
		if (default_skill_info[Skill_R_1_code].level == 5) return 0;
		default_skill_info[Skill_R_1_code].level += 1;
		default_skill_info[Skill_R_1_code].wasted_mp += 4;
		duration += 2.0f;
		damage += 1;
		range += 10;
		default_skill_info[Skill_R_1_code].context = "Fire Flame for " + std::to_string(duration) + " sec, dealing " + std::to_string(damage) + "/0.1s. The range is" + std::to_string(int(range)) + ".";
		return -1;
	}
};
class Normal_Left {
public:
	bool  turn_on = false;
	bool  attack = false;
	bool  render = false;
	float init_range = 40.0f;
	float range = init_range;
	float max_range = 100.0f;
	int damage = 6;

	float init_radius = 0.6f;
	float radius = 0.6f;
	float max_radius = 1.5f;
	float render_time_end = 0.0f;
	float render_time = 4.0f;
	Normal_Left() {
		default_skill_info[Skill_Normal_Left_code].cooldown = 6.0f;
		default_skill_info[Skill_Normal_Left_code].level = 1;
	}
	int level_up() {
		default_skill_info[Skill_Normal_Left_code].level += 1;
		damage += 2;
		max_range += 10.0f;
		max_radius += 0.15f;
		return -1;
	}
};

class Skill_space_2 {
public:
	float duration = 10.0f;
	float duration_end;
	float add_speed_max = 0.8f;
	float add_speed;
	float add_acc = 0.004f;
	bool  turn_on = false;
	Skill_space_2() {
		default_skill_info[Skill_space_2_code].skill_name = "Accelerator";
		default_skill_info[Skill_space_2_code].context = "Gradually increase speed from 0 to " + std::to_string(add_speed_max) + "x our speed, for " + std::to_string(duration) + " second";
		default_skill_info[Skill_space_2_code].context2 = "(Key SPACE)";
		default_skill_info[Skill_space_2_code].wasted_mp = 4;
		default_skill_info[Skill_space_2_code].cooldown = 14.0f;
		default_skill_info[Skill_space_2_code].level = 1;
	}

	int level_up() {
		if (default_skill_info[Skill_space_2_code].level == 8) return 0;
		default_skill_info[Skill_space_2_code].level += 1;
		default_skill_info[Skill_space_2_code].wasted_mp += 1;
		duration += 0.8f;
		add_speed_max += 0.1f;
		add_acc += 0.001f;
		default_skill_info[Skill_space_2_code].context = "Gradually increase speed from 0 to " + std::to_string(add_speed_max) + "x our speed, for " + std::to_string(duration) + " second";
		return -1;
	}
};
class Skill_Q_2 {
public:
	bool  turn_on = false;
	int maximum_trap = 6;
	float trap_duration = 5.0f;
	vec2 trap_size = vec2(100.0f,30.0f);
	int damage = 8;
	float slow = 0.4f;
	Skill_Q_2() {
		default_skill_info[Skill_Q_2_code].skill_name = "Trap";
		default_skill_info[Skill_Q_2_code].context = "Install trap. If enemy step on, dealing" + std::to_string(damage) + "per seconds, slow down " + std::to_string(int(slow*100)) + "%. Trap duration is "+ std::to_string(int(trap_duration))+"sec.";
		default_skill_info[Skill_Q_2_code].context2 = "(Key 1)";
		default_skill_info[Skill_Q_2_code].wasted_mp = 12;
		default_skill_info[Skill_Q_2_code].cooldown = 8.0f;
		default_skill_info[Skill_Q_2_code].level = 1;
	}
	int level_up() {
		if (default_skill_info[Skill_Q_2_code].level == 8)return 0;
		default_skill_info[Skill_Q_2_code].wasted_mp += 2;
		default_skill_info[Skill_Q_2_code].cooldown -= 0.6f;
		default_skill_info[Skill_Q_2_code].level += 1;
		trap_size += vec2(10.0f,5.0f);
		damage += 2;
		slow += 0.08f;
		trap_duration += 5.0f;
		default_skill_info[Skill_Q_2_code].context = "Install trap. If enemy step on, dealing" + std::to_string(damage) + "per seconds,, slow down " + std::to_string(int(slow * 100)) + "%. Trap duration is " + std::to_string(int(trap_duration)) + "sec.";
		return -1;
	}
};

class Skill_E_2 {
public:
	bool  turn_on = false;
	float duration = 3.0f;
	float duration_end;
	int protection = 20;
	Skill_E_2() {
		default_skill_info[Skill_E_2_code].skill_name = "Barrier";
		default_skill_info[Skill_E_2_code].context = "Protect "+std::to_string(protection)+" damage, for "+std::to_string(duration)+" second.";
		default_skill_info[Skill_E_2_code].context2 = "(Key 3)";
		default_skill_info[Skill_E_2_code].wasted_mp = 10;
		default_skill_info[Skill_E_2_code].cooldown = 8.0f;
		default_skill_info[Skill_E_2_code].level = 1;
	}
	int level_up() {
		if (default_skill_info[Skill_E_2_code].level == 8)return 0;
		default_skill_info[Skill_E_2_code].wasted_mp += 3;
		default_skill_info[Skill_E_2_code].cooldown -= 0.6f;
		default_skill_info[Skill_E_2_code].level += 1;
		duration += 0.2f;
		protection += 10;
		default_skill_info[Skill_E_2_code].context = "Protect " + std::to_string(protection) + " damage, for " + std::to_string(duration) + " second.";
		return -1;
	}
};
class Skill_R_2 {
public:
	bool  turn_on = false;
	float duration = 3.0f;
	float duration_end;
	float zoom = PI/4;
	int damage = 18;
	int init_damage = 18;
	int initial_shot = 4;
	int shot = 4;
	float reshot_end=0.0f;
	float reshot_time = 0.9f;
	vec3 cameye;
	vec3 camat;
	bool hit=false;
	bool hit_init=false;
	bool start_switch = false;
	int damage_step = 0;
	Skill_R_2() {
		default_skill_info[Skill_R_2_code].skill_name = "Sniper";
		default_skill_info[Skill_R_2_code].context = "Sniping " + std::to_string(initial_shot) + " times, dealing " + std::to_string(init_damage) + "~" + std::to_string(init_damage * 2);
		default_skill_info[Skill_R_2_code].context2 = "The damage is proportional to zoom out. (Key 4)";
		default_skill_info[Skill_R_2_code].wasted_mp = 20;
		default_skill_info[Skill_R_2_code].cooldown = 8.0f;
		default_skill_info[Skill_R_2_code].level = 1;
	}
	int level_up() {
		if (default_skill_info[Skill_R_2_code].level == 5)return 0;
		default_skill_info[Skill_R_2_code].wasted_mp += 6;
		default_skill_info[Skill_R_2_code].cooldown -= 0.6f;
		default_skill_info[Skill_R_2_code].level += 1;
		initial_shot += 1;
		init_damage += 8;
		default_skill_info[Skill_R_2_code].context = "Sniping " + std::to_string(initial_shot) + " times, dealing " + std::to_string(init_damage) + "~" + std::to_string(init_damage * 2);
		return -1;
	}
};
Skill_space_1* space_1 = new Skill_space_1();
Skill_Q_1* q_1 = new Skill_Q_1();
Skill_E_1* e_1 = new Skill_E_1();
Skill_R_1* r_1 = new Skill_R_1();
Normal_Left* normal_left = new Normal_Left();

Skill_space_2* space_2 = new Skill_space_2();
Skill_Q_2* q_2 = new Skill_Q_2();
Skill_E_2* e_2 = new Skill_E_2();
Skill_R_2* r_2 = new Skill_R_2();