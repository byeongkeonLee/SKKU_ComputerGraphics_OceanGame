#pragma once
#ifndef __SOUND_H__
#define __SOUND_H__
#include "irrKlang\irrKlang.h"

#pragma comment(lib, "irrKlang.lib")
class Sound_2d {
	irrklang::ISoundEngine* engine = nullptr;
	irrklang::ISoundSource* src = nullptr;

public:
	Sound_2d(char *path) {
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
#endif