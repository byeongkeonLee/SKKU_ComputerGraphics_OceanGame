#pragma once
#ifndef __SOUND_H__
#define __SOUND_H__
#include "irrKlang\irrKlang.h"
#include "camera.h"
#include "ship.h"

#pragma comment(lib, "irrKlang.lib")

class Sound_2d {
	irrklang::ISoundEngine* engine = nullptr;
	irrklang::ISoundSource* src = nullptr;

public:
	Sound_2d(char* path) {
		engine = irrklang::createIrrKlangDevice();
		src = engine->addSoundSourceFromFile(path);
		src->setDefaultVolume(0.5f);
		engine->play2D(src, true);
	}
	void play() {
		engine->play2D(src, true);
	}
	void stop() {
		engine->stopAllSounds();
	}
	~Sound_2d() {
		engine->drop();
	}
};

class Sound_3d {
	irrklang::ISoundEngine* engine = nullptr;
	irrklang::ISound* sound = nullptr;
	irrklang::vec3df sound_pos;
	irrklang::vec3df sound_velocity;
	char* sound_path;

public:
	Sound_3d(char* path, vec3 pos, vec3 velocity) {
		sound_path = path;
		// create the engine
		sound_pos = irrklang::vec3df(pos.x, pos.y, pos.z);
		sound_velocity = irrklang::vec3df(velocity.x, velocity.y, velocity.z);
		engine = irrklang::createIrrKlangDevice();
		// play the sound source in 3D

		//set Dopper effect parameters
		// two times than the real world Doppler effect
		engine->setDopplerEffectParameters(2.0f, 1.0f);

		//set listener's attributes using camera attributes
		irrklang::vec3df listener_pos = reinterpret_cast<irrklang::vec3df&> (cam.eye);			// listener's position
		irrklang::vec3df listener_up = reinterpret_cast<irrklang::vec3df&> (cam.up);		// listener's up vector
		irrklang::vec3df look_direction = reinterpret_cast<irrklang::vec3df&> (cam.at);		// the direction the listener looks into
		vec2 v = ship->speed + ship->additional_speed * ship->dir;
		irrklang::vec3df listener_velocity(v.x, v.y, 0);

		//change OpenGL coordinates to irrKlang coordinates
		listener_pos.Z = -listener_pos.Z;
		look_direction.Z = -look_direction.Z;
		listener_up.Z = -listener_up.Z;
		engine->setListenerPosition(listener_pos, look_direction, listener_velocity, listener_up);
	}
	void update(vec3 pos, vec3 velocity) {
		sound_pos = irrklang::vec3df(pos.x, pos.y, pos.z);
		sound_velocity = irrklang::vec3df(velocity.x, velocity.y, velocity.z);
		if (sound) {
			sound->setVelocity(sound_velocity);
			sound->setPosition(sound_pos);
		}
		irrklang::vec3df listener_pos = reinterpret_cast<irrklang::vec3df&> (cam.eye);			// listener's position
		irrklang::vec3df listener_up = reinterpret_cast<irrklang::vec3df&> (cam.up);		// listener's up vector
		irrklang::vec3df look_direction = reinterpret_cast<irrklang::vec3df&> (cam.at);		// the direction the listener looks into
		vec2 v = ship->speed + ship->additional_speed * ship->dir;
		irrklang::vec3df listener_velocity(v.x, v.y, 0);
		//change OpenGL coordinates to irrKlang coordinates
		listener_pos.Z = -listener_pos.Z;
		look_direction.Z = -look_direction.Z;
		listener_up.Z = -listener_up.Z;
		engine->setListenerPosition(listener_pos, look_direction, listener_velocity, listener_up);
	}
	void stop() {
		engine->stopAllSounds();
	}
	void play(bool re) {
		sound = engine->play3D(sound_path, sound_pos, re, false, true);
		if (sound) {
			sound->setVelocity(sound_velocity);
			sound->setPosition(sound_pos);
			sound->setMinDistance(100.0f);
		}
	}
	~Sound_3d() {
		if (sound) sound->drop();
		engine->drop();
	}

};


class SoundManager {
	Sound_3d* sounds[6];
public:
	SoundManager() {
		sounds[HIT] = new Sound_3d("../bin/sound/hit.mp3", vec3(ship->distance, 0), vec3(ship->dir, 0));
		sounds[ENEMY_HIT] = new Sound_3d("../bin/sound/hit.mp3", vec3(ship2->distance, 0), vec3(ship2->dir, 0));
		sounds[CANNON] = new Sound_3d("../bin/sound/cannon.mp3", vec3(ship->distance, 0), vec3(ship->dir, 0));
		sounds[ENEMY_CANNON] = new Sound_3d("../bin/sound/cannon.mp3", vec3(ship2->distance, 0), vec3(ship2->dir, 0));
		if (SKILL_SET) {
			sounds[SNIPER] = new Sound_3d("../bin/sound/sniper.mp3", vec3(ship->distance, 0), vec3(ship->dir, 0));
			sounds[FIRE] = new Sound_3d("../bin/sound/fire.mp3", vec3(ship2->distance, 0), vec3(ship2->dir, 0));
		}
		else {
			sounds[SNIPER] = new Sound_3d("../bin/sound/sniper.mp3", vec3(ship2->distance, 0), vec3(ship2->dir, 0));
			sounds[FIRE] = new Sound_3d("../bin/sound/fire.mp3", vec3(ship->distance, 0), vec3(ship->dir, 0));
		}
	}

	void update(bool* updates) {
		if (updates[HIT]) {
			sounds[HIT]->update(vec3(ship->distance, 0), vec3(ship->dir, 0));
			updates[HIT] = false;
		}
		if (updates[ENEMY_HIT]) {
			sounds[ENEMY_HIT]->update(vec3(ship2->distance, 0), vec3(ship2->dir, 0));
			updates[ENEMY_HIT] = false;
		}
		if (updates[CANNON]) {
			sounds[CANNON]->update(vec3(ship->distance, 0), vec3(ship->dir, 0));
			updates[CANNON] = false;
		}
		if (updates[ENEMY_CANNON]) {
			sounds[ENEMY_CANNON]->update(vec3(ship2->distance, 0), vec3(ship2->dir, 0));
			updates[ENEMY_CANNON] = false;
		}
		if (updates[SNIPER]) {
			if (SKILL_SET) sounds[SNIPER]->update(vec3(ship->distance, 0), vec3(ship->dir, 0));
			else sounds[SNIPER]->update(vec3(ship2->distance, 0), vec3(ship2->dir, 0));
			updates[SNIPER] = false;
		}
		if (true || updates[FIRE]) {
			if (SKILL_SET) sounds[FIRE]->update(vec3(ship2->distance, 0), vec3(ship2->dir, 0));
			else sounds[FIRE]->update(vec3(ship->distance, 0), vec3(ship->dir, 0));
			updates[FIRE] = false;
		}
	}

	void stop_fire() {
		sounds[FIRE]->stop();
	}

	void play(bool* plays) {
		for (int i = 0; i < 5; i++) {
			if (plays[i]) {
				sounds[i]->play(false);
				plays[i] = false;
			}
		}
		if (plays[5]) {
			sounds[5]->play(true);
			plays[5] = false;
		}
	}
};

#endif