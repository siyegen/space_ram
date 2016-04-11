#include "sound_manager.h"


irrklang::ISoundEngine *SoundEngine = irrklang::createIrrKlangDevice();

void SoundManager::LoadSound(std::string name, std::string file) {
	irrklang::ISoundSource *sound = SoundEngine->addSoundSourceFromFile(file.c_str());
	if (sound) {
		SoundEngine->addSoundSourceAlias(sound, name.c_str());
	}
}

void SoundManager::PlaySound(std::string name, float volume) {
	irrklang::ISoundSource *sound = SoundEngine->getSoundSource(name.c_str());
	if (sound) {
		sound->setDefaultVolume(volume);
		SoundEngine->play2D(sound);
	}
}

void SoundManager::Clear() {
	SoundEngine->removeAllSoundSources();
	SoundEngine->drop();
}
