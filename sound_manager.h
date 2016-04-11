#pragma once

#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <map>
#include <string>

#include <irrklang/irrKlang.h>

class SoundManager final {
public:
	static void LoadSound(std::string name, std::string file);

	static void PlaySound(std::string name, float volume);
	static void Clear();

private:
	SoundManager() {}
};

#endif
