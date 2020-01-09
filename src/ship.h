#pragma once
#include "cgmath.h"			// slee's simple math library
#include "cgut.h"			// slee's OpenGL utility
#include "func.h"
#include "constant.h"
#include "assimp_loader.h"
#include "camera.h"
#include "skill_set.h"
#include "trap.h"
#include "ship2.h"
#include "terrain.h"
#include "barrel.h"
#include "woodboard.h"
#include "sniperbullet.h"
#include "zombie.h"

class Ship {
public:

	struct { bool w = false, a = false, s = false, d = false, space = false, _1 = false, _3 = false, _4 = false; operator bool() const { return w || a || s || d || space || _1 || _3 || _4; } } key;
	const char* mesh_obj = "../bin/mesh/ship2/Boat.obj";	// OBJ
	//const char* mesh_obj = "../bin/mesh/wood/plank 1 model.obj";
	mesh2* pMesh = nullptr;
	GLuint texture_buffer = 0;
	mat4 base_model_matrix = mat4::rotate(vec3(0, 0, 1), PI) * mat4::rotate(vec3(1, 0, 0), PI / 2) * mat4::scale(0.005f);
	mat4 model_matrix;


	float acc = 0.006f;
	vec2 dir = vec2(0, 1.0f);
	vec2 speed = vec2(0, 0);
	vec2 distance = vec2(0, 0);
	vec2 total_speed = vec2(0, 0);
	float distance_z = -1.0f;
	float distance_z_init = -1.0f;
	float additional_speed = 0.0f;
	float max_speed = 3.0f;
	float fraction = 0.006f;
	float friction = 0.09f;
	float angle_acc = 0.0001f;
	float angle_speed = 0.0f;
	float angle_max_speed = 0.006f;
	float angle = 0.0f;
	float trap_slow_speed = 1.0f;
	bool is_sturn = false;
	float sturn_time_end;
	vec2 crush_speed = vec2(0.0f, 0.0f);
	vec2 crush_speed2 = vec2(0.0f, 0.0f);
	float crushed = 0.0f;
	float crushed2 = 0.0f;
	float crush_damage = 50.0f;

	Trap* trap;
	float trap_damage_time_end = 0.0f;
	float r_1_damage_time_end = 0.0f;
	
	int curshot = 0;

	bool skill_set_1(float glTime) {
		if (key.space && glTime > default_skill_info[Skill_space_1_code].cooldown_end&& mp >= default_skill_info[Skill_space_1_code].wasted_mp) {
			default_skill_info[Skill_space_1_code].cooldown_end = glTime + default_skill_info[Skill_space_1_code].cooldown;
			mp -= default_skill_info[Skill_space_1_code].wasted_mp;

			space_1->duration_end = glTime + space_1->duration;
			additional_speed = space_1->add_speed;
			cam.radial_intensity = 75.0f;
			cam.radial_blur = true;

		}
		if (!key.space || glTime > space_1->duration_end) {
			additional_speed = 0.0f; cam.radial_blur = false;
		}
		if (key._1) {
			if (!q_1->turn_on && glTime > default_skill_info[Skill_Q_1_code].cooldown_end&& mp >= default_skill_info[Skill_Q_1_code].wasted_mp) {
				q_1->dir = dir;
				q_1->ship_spd = speed;
				q_1->turn_on = true;
				q_1->position = distance + dir * 30.0f;
				q_1->init_position = q_1->position;
				q_1->hit_server = q_1->hit_client = false;

				default_skill_info[Skill_Q_1_code].cooldown_end = glTime + default_skill_info[Skill_Q_1_code].cooldown;
				mp -= default_skill_info[Skill_Q_1_code].wasted_mp;
			}
			if (q_1->turn_on && q_1->hit_server) {
				distance += (q_1->position - distance) * 0.04f;
			}
		}
		if (key._3 && glTime > default_skill_info[Skill_E_1_code].cooldown_end&& mp >= default_skill_info[Skill_E_1_code].wasted_mp) {
			default_skill_info[Skill_E_1_code].cooldown_end = glTime + default_skill_info[Skill_E_1_code].cooldown;
			e_1->duration_end = glTime + e_1->duration;
			e_1->turn_on = true;
			mp -= default_skill_info[Skill_E_1_code].wasted_mp;
		}
		if (glTime > e_1->duration_end) {
			e_1->turn_on = false;
		}
		if (key._4 && glTime > default_skill_info[Skill_R_1_code].cooldown_end) {
			if (mp >= default_skill_info[Skill_R_1_code].wasted_mp) {
				r_1->turn_on = true;
				default_skill_info[Skill_R_1_code].cooldown_end = glTime + default_skill_info[Skill_R_1_code].cooldown;
				mp -= default_skill_info[Skill_R_1_code].wasted_mp;
				sound_plays[FIRE] = true;
				sound_updates[FIRE] = true;
				plays_to_enemy[2] = true;
				updates_to_enemy[2] = true;
			}
			else {
				r_1->turn_on = false;
				stop_fire = true;
				stop_client_fire = true;
			}
		}
		else if (!key._4 && r_1->turn_on) {
			r_1->turn_on = false;
			stop_fire = true;
			stop_client_fire = true;
		}

		
		return true;
	}
	bool skill_set_2(float glTime) {
		if (key.space && glTime > default_skill_info[Skill_space_2_code].cooldown_end&& mp >= default_skill_info[Skill_space_2_code].wasted_mp) {
			default_skill_info[Skill_space_2_code].cooldown_end = glTime + default_skill_info[Skill_space_2_code].cooldown;
			mp -= default_skill_info[Skill_space_2_code].wasted_mp;

			space_2->duration_end = glTime + space_2->duration;
			space_2->add_speed = 0.0f;
			space_2->turn_on = true;
		}
		if (key.space && space_2->turn_on && space_2->duration_end - glTime < space_2->duration - 1) {
			space_2->turn_on = false;
		}
		if (space_2->turn_on && glTime < space_2->duration_end) {
			if (space_2->add_speed < space_2->add_speed_max)
				space_2->add_speed += space_2->add_acc;
			additional_speed = speed.length() * space_2->add_speed;
			cam.radial_intensity = additional_speed * 120.0f;
			cam.radial_blur = true;
		}
		else {
			space_2->turn_on = false;
		}
		if (!space_2->turn_on) {
			if (additional_speed > 0)
				additional_speed -= space_2->add_acc * 1.2f;
			else
				additional_speed = 0;
			cam.radial_blur = false;
		}
		if (key._1 && glTime > default_skill_info[Skill_Q_2_code].cooldown_end&& mp >= default_skill_info[Skill_Q_2_code].wasted_mp) {
			default_skill_info[Skill_Q_2_code].cooldown_end = glTime + default_skill_info[Skill_Q_2_code].cooldown;
			mp -= default_skill_info[Skill_Q_2_code].wasted_mp;
			q_2->turn_on = true;

			trap->init(distance, angle, glTime, q_2->trap_duration, q_2->trap_size);
		}
		if (key._3 && glTime > default_skill_info[Skill_E_2_code].cooldown_end&& mp >= default_skill_info[Skill_E_2_code].wasted_mp) {
			mp -= default_skill_info[Skill_E_2_code].wasted_mp;
			default_skill_info[Skill_E_2_code].cooldown_end = glTime + default_skill_info[Skill_E_2_code].cooldown;
			e_2->duration_end = e_2->duration + glTime;
			e_2->turn_on = true;
			hp_shield = e_2->protection;
		}
		if (e_2->turn_on && glTime > e_2->duration_end) {
			hp_shield = 0;
			e_2->turn_on = false;
		}

		if (key._4 && glTime > default_skill_info[Skill_R_2_code].cooldown_end&& mp >= default_skill_info[Skill_R_2_code].wasted_mp) {
			mp -= default_skill_info[Skill_R_2_code].wasted_mp;
			default_skill_info[Skill_R_2_code].cooldown_end = glTime + default_skill_info[Skill_R_2_code].cooldown;
			r_2->turn_on = true;
			r_2->shot = r_2->initial_shot;
			r_2->damage = int(r_2->init_damage * (2.0f - r_2->damage_step * 0.05f));

		}
		if (r_2->turn_on) {
			if (!CAMERA_TRACKBALL) {
				cam.set_eye = vec3(vec2(distance) + dir * 20.0f, 10.0f);
				cam.set_at = vec3(vec2(distance) + dir * 40.0f, 10.0f) + vec3(sin(angle - cur.x ) * 110.0f, cos(angle - cur.x ) * 110.0f, cur.y * 10.0f);
				cam.set_up = vec3(0, 0, 1);
				//cam.eye = cam.set_eye = vec3(vec2(distance), 40.0f) - vec3(sin(angle - cur.x) * 80.0f, cos(angle - cur.x) * 80.0f, cur.y * 10.0f);
				//cam.at = cam.set_at = vec3(vec2(distance), 0.0f);
				
			}
			if (key._4 && default_skill_info[Skill_R_2_code].cooldown_end - glTime < default_skill_info[Skill_R_2_code].cooldown - 1) r_2->turn_on = false;
			if (glTime > default_skill_info[Skill_R_2_code].cooldown_end-1.0f) r_2->turn_on = false;
		}
		
		return true;
	}
	bool skill_detector(float glTime) {
		if (normal_left->turn_on) {
			if (normal_left->range < normal_left->max_range)
				normal_left->range += (normal_left->max_range - normal_left->init_range) * 0.005f;
			normal_left->radius = (normal_left->max_radius - normal_left->init_radius) * (normal_left->range - normal_left->init_range) / (normal_left->max_range - normal_left->init_range) + normal_left->init_radius;
		}
		if (SKILL_SET == 0) {
			skill_set_1(glTime);
		}
		else {
			skill_set_2(glTime);
		}
		return true;
	}
	bool skill_hit_detector(float glTime) {


		if (normal_left->render_time_end < glTime) {
			normal_left->render = false;
		}
		if (SKILL_SET == 1) { //skill 1 champion
			if (q_1->turn_on && (distance - q_1->position).length() < q_1->hit_range) {
				if (!q_1->hit_client) {
					is_sturn = true;
					sturn_time_end = q_1->sturn_time + glTime;
					hp -= q_1->damage;
					q_1->hit_client = true;
				}
			}
			if (!q_1->turn_on) q_1->hit_client = false;
			if (sturn_time_end < glTime) {
				is_sturn = false;
			}
			if (r_1->turn_on) {
				float angle_difference = acos((normalize(ship2->distance - distance)).dot(ship2->dir));
				if ((ship2->distance - distance).length() < r_1->range*2.0f && glTime > r_1_damage_time_end && angle_difference > 3.0f*PI/4 ) {
					hp -= r_1->damage;
					r_1_damage_time_end = glTime+0.1f;
				}
			}
		}
		else if (SKILL_SET == 0) {
			trap_slow_speed = 1;
			for (int i = 0; i < 5; i++) {
				if (trap->available[i]) {
					vec3 tmp = vec3(distance, 0) - trap->position[i];
					float tmp_angle = -trap->dir[i];
					vec3 rtmp = mat3(cos(tmp_angle), -sin(tmp_angle), 0, sin(tmp_angle), cos(tmp_angle), 0, 0, 0, 1) * tmp;
					if (rtmp.x > -trap->size[i].x && rtmp.x < trap->size[i].x && rtmp.y > -trap->size[i].y && rtmp.y < trap->size[i].y) {
						trap_slow_speed = 1.0f - q_2->slow;
					}
				}
				if (trap_slow_speed < 0.9f && trap_damage_time_end < glTime) {
					trap_damage_time_end = glTime + 1;
					hp -= q_2->damage;
				}
			}
			if (r_2->hit && !r_2->hit_init) {
				hp -= r_2->damage;
				r_2->hit_init = true;
			}
		}
		return true;
	}
	bool is_collision_island() {
		vec2 angle_points[8]{
			vec2(2.0f, 7.46f),
			vec2(2.0f, 0.0f),
			vec2(2.0f, -15.8f),
			vec2(-2.0f, 7.46f),
			vec2(-2.0f, 0.0f),
			vec2(-2.0f, -15.8f),
			vec2(0.0f, 7.46f),
			vec2(0.0f, -15.8f)
		};
		vec2* last = angle_points + 8;
		for (vec2* angle_point = angle_points; angle_point < last; angle_point += 1)
			*angle_point = vec2(angle_point->x * dir.y - angle_point->y * dir.x, angle_point->x * dir.x + angle_point->y * dir.y);
		vec2 sensors[8] = {
		   distance - angle_points[0],
		   distance - angle_points[1],
		   distance - angle_points[2],
		   distance - angle_points[3],
		   distance - angle_points[4],
		   distance - angle_points[5],
		   distance - angle_points[6],
		   distance - angle_points[7]
		};
		last = sensors + 8;
		for (vec2* sensor = sensors; sensor < last; sensor += 1)
			if (terrain->getHeight_ship(*sensor) >= 0.0f) return true;
		return false;
	}
	void collision_island(vec2 prev_distance) {
		distance = prev_distance;
		crush_speed += 2 * speed;
		vec2 velocity = speed + additional_speed * dir;
		vec2 velocity2 = ship2->speed + ship2->additional_speed * ship2->dir;
		float crush = length(velocity - velocity2);
		if (crush > 0.3f) {
			hp -= int(crush * 20.0f);
			sound_plays[HIT] = true;
			sound_updates[HIT] = true;
			plays_to_enemy[0] = true;
			updates_to_enemy[0] = true;
		}
	}

	bool point_in_triangle(vec2 p, vec2 p1, vec2 p2, vec2 p3) {
		float alpha = ((p2.y - p3.y) * (p.x - p3.x) + (p3.x - p2.x) * (p.y - p3.y)) /
			((p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));
		float beta = ((p3.y - p1.y) * (p.x - p3.x) + (p1.x - p3.x) * (p.y - p3.y)) /
			((p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));
		float gamma = 1.0f - alpha - beta;
		if (alpha > 0.0f && beta > 0.0f && gamma > 0.0f) return true;
		else return false;
	}
		

	bool is_collision() {
		vec2 angle_points[8]{
			vec2(2.0f, 7.46f),
			vec2(2.0f, 0.0f),
			vec2(2.0f, -15.8f),
		    vec2(-2.0f, 7.46f),
		    vec2(-2.0f, 0.0f),
		    vec2(-2.0f, -15.8f),
		    vec2(0.0f, 7.46f),
		    vec2(0.0f, -15.8f)
		};
		vec2* last = angle_points + 8;
		for (vec2* angle_point = angle_points; angle_point < last; angle_point += 1)
			*angle_point = vec2(angle_point->x * dir.y - angle_point->y * dir.x, angle_point->x * dir.x + angle_point->y * dir.y);

		vec2 angle_points2[8]{
			vec2(2.0f, 7.46f),
			vec2(2.0f, 0.0f),
			vec2(2.0f, -15.8f),
			vec2(-2.0f, 7.46f),
			vec2(-2.0f, 0.0f),
			vec2(-2.0f, -15.8f),
			vec2(0.0f, 7.46f),
			vec2(0.0f, -15.8f)
		};
		last = angle_points2 + 8;
		for(vec2* angle_point = angle_points2; angle_point < last; angle_point += 1)
			*angle_point = vec2(angle_point->x * ship2->dir.y - angle_point->y * ship2->dir.x, angle_point->x * ship2->dir.x + angle_point->y * ship2->dir.y);
		
		vec2 sensors[8] = {
		   distance - angle_points[0],
		   distance - angle_points[1],
		   distance - angle_points[2],
		   distance - angle_points[3],
		   distance - angle_points[4],
		   distance - angle_points[5],
		   distance - angle_points[6],
		   distance - angle_points[7]
		};

		vec2 sensors2[8] = {
			ship2->distance - angle_points2[0],
			ship2->distance - angle_points2[1],
			ship2->distance - angle_points2[2],
			ship2->distance - angle_points2[3],
			ship2->distance - angle_points2[4],
			ship2->distance - angle_points2[5],
			ship2->distance - angle_points2[6],
			ship2->distance - angle_points2[7]
		};
		last = sensors + 8;
		for (vec2* sensor = sensors; sensor < last; sensor += 1)
			if (point_in_triangle(*sensor, sensors2[0], sensors2[2], sensors2[3])
				|| point_in_triangle(*sensor, sensors2[2], sensors2[3], sensors2[5]))
				return true;

		last = sensors2 + 8;
		for (vec2* sensor = sensors2; sensor < last; sensor += 1)
			if (point_in_triangle(*sensor, sensors[0], sensors[2], sensors[3])
				|| point_in_triangle(*sensor, sensors[2], sensors[3], sensors[5]))
				return true;

		return false;
	}
	void collision(vec2 prev_distance)
	{
		distance = prev_distance;
		vec2 gap = distance - ship2->distance;
		vec2 velocity = speed + additional_speed * dir;
		vec2 velocity2 = ship2->speed + ship2->additional_speed * ship2->dir;
		float crush = length(velocity - velocity2);
		crush_speed += dot(speed - ship2->speed, gap) / length2(gap) * gap;
		crush_speed2 = -crush_speed;
		if (crush > 0.3f) {
			hp -= int(crush * 20.0f);
			crushed2 = crush * crush_damage;
			sound_plays[HIT] = true;
			sound_updates[HIT] = true;
			plays_to_enemy[0] = true;
			updates_to_enemy[0] = true;
		}
	}

	void level_up()
	{
		lv += 1;
		exper1 -= maxexp;
		maxexp += 1;
		acc += 0.001f;
		angle_acc += 0.00001f;
		angle_max_speed += 0.001f;
		max_speed += 0.3f;
		hp += 20;
		mp += 10;
		sp += 1;
		maxhp += 20;
		maxmp += 10;
		hp_recovery += 1;
		mp_recovery += 1;
		crush_damage += 10.0f;
		normal_left->level_up();
	}

	bool update(float glTime, Zombie* zombie) {
		if (is_sturn) {
			speed = 0;
			angle_speed = 0;
		}
		if (hp > 0) {
			if (glTime > 4 || !CAMERA_INIT_EFFECT && !is_sturn) {
				skill_detector(glTime);
				skill_hit_detector(glTime);
				if (key.a && angle_speed > -angle_max_speed)
					angle_speed -= angle_acc * (length(speed) + additional_speed);
				if (key.d && angle_speed < angle_max_speed)
					angle_speed += angle_acc * (length(speed) + additional_speed);
				if (!key.a && !key.d) angle_speed *= 0.992f;
				angle += angle_speed;
				dir = vec2(sin(angle), cos(angle));
				if (key.w && speed.length() < max_speed)
					speed += dir * acc;
				if (key.s && speed.length() > -max_speed / 2.0f)
					speed -= dir * acc / 2.0f;
				vec2 cir = vec2(cos(angle), -sin(angle));
				speed -= dot(speed, dir) * fraction * dir + dot(speed, cir) * friction * cir;
				//cam.radial_intensity *= 0.992f;
				//if (cam.radial_intensity < 0.001f) cam.radial_blur = false;
				vec2 prev_distance = distance;
				total_speed = (speed + dir * additional_speed) * trap_slow_speed;
				distance += total_speed;
				//if (length(distance - ship2->distance) < 20) collision(prev_distance);
				if (is_collision()) collision(prev_distance);
				if (is_collision_island()) collision_island(prev_distance);
				if (crush_speed != vec2(0.0f, 0.0f)) {
					speed -= crush_speed;
					crush_speed = vec2(0.0f, 0.0f);
					speed *= 0.5f;
				}
				if (crushed != 0.0f) {
					hp -= int(crushed);
					crushed = 0.0f;
				}
				//printf("%f\n", terrain->getHeight((int)distance.x, (int)distance.y));
				if (!r_2->turn_on && !CAMERA_TRACKBALL) {
					cam.eye = cam.set_eye = vec3(vec2(distance), 40.0f) - vec3(sin(angle - cur.x) * 80.0f, cos(angle - cur.x) * 80.0f, cur.y * 10.0f);
					cam.at = cam.set_at = vec3(vec2(distance), 0.0f);
					cam.up = cam.set_up = vec3(0, 0, 1);
				}
			}
			else if (!CAMERA_TRACKBALL) {
				//cam.set_eye = vec3(vec2(distance), 20.0f) - vec3(dir * 80.0f, 0.0f);
				//cam.set_at = vec3(vec2(distance), 0.0f) + vec3(dir * 15.0f, 0.0f);
			}

			if (glTime > mp_freq_end) {
				mp += mp_recovery;
				mp_freq_end = glTime + mp_freq;
			}
			if (glTime > hp_freq_end) {
				hp += hp_recovery;
				hp_freq_end = glTime + hp_freq;
			}
			mp = mp > maxmp ? maxmp : mp;
			hp = hp > maxhp ? maxhp : hp;
			if (exper1 >= maxexp) level_up();
		}

		model_matrix = mat4::translate(vec3(distance.x, distance.y, distance_z)) * mat4::rotate(vec3(0, 0, 1), -angle) * base_model_matrix;
		if (hp <= 0 && !is_dead) {
			death += 1;
			rebirth_time_end = glTime + rebirth_time;
			is_dead = true;
		}
		if (is_dead) {
			if (rebirth_time_end < glTime) {
				hp = maxhp;
				mp = maxmp;
				zombie->add_zombie(vec3(this->distance, distance_z), angle);
				distance_z = distance_z_init;

				float rand_x = ((random() - 0.5f) * (SKILL_SET==1? 1:-1)) * WORLD_WIDTH*0.9f;
				float rand_y = (random() - 0.5f) * WORLD_HEIGHT * 0.9f;
				while (terrain->getHeight_ship(vec2(rand_x, rand_y)) > -7.0f) {
					rand_x = ((random() - 0.5f) * (SKILL_SET == 1 ? 1 : -1)) * WORLD_WIDTH * 0.9f;
					rand_y = (random() - 0.5f) * WORLD_HEIGHT * 0.9f;
				}
				distance = vec2(rand_x, rand_y);
				is_dead = false;
			}
			else {
				if (terrain->getHeight_ship(distance) < distance_z) distance_z -= 0.07f;
			}
		}
		if (hp2 <= 0 && !is_dead2) {
			exper1 += 5 + (lv2-lv> -2? lv2-lv : 0);
			is_dead2 = true;
		}
		if (is_dead2 && hp2>0) {
			is_dead2 = false;
		}
		return true;
	}
	bool render(GLuint program) {
		glUniform1i(glGetUniformLocation(program, "VertexShaderMode"), 1);
		glUniform1i(glGetUniformLocation(program, "FragmentShaderMode"), SHIP_SHADER);
		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
		glUniform1f(glGetUniformLocation(program, "use_texture"), false);
		glUniform1f(glGetUniformLocation(program, "ship_alpha"), (SKILL_SET ==0 && e_1->turn_on)? 0.6f:1.0f);
		cg_bind_vertex_attributes_mesh(program, pMesh);
		for (size_t k = 0, kn = pMesh->geometry_list.size(); k < kn; k++) {
			geometry& g = pMesh->geometry_list[k];
			glUniform4fv(glGetUniformLocation(program, "diffuse"), 1, (const float*)(&g.mat->diffuse));
			// render vertices: trigger shader programs to process vertex data
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->index_buffer);
			glDrawElements(GL_TRIANGLES, g.index_count, GL_UNSIGNED_INT, (GLvoid*)(g.index_start * sizeof(GLuint)));
		}
		return true;
	}
	Ship(Trap* trap_) {
		pMesh = load_model(mesh_obj);
		if (pMesh == nullptr) { printf("Unable to load mesh\n"); }
		trap = trap_;
	}
};


Ship* ship;