#include "sound_manager.h"

#ifdef OS_WINDOWS
irrklang::ISoundEngine *SoundEngine = irrklang::createIrrKlangDevice();
#endif

void SoundManager::LoadSound(std::string name, std::string file) {
#ifdef OS_WINDOWS
	irrklang::ISoundSource *sound = SoundEngine->addSoundSourceFromFile(file.c_str());
	if (sound) {
		SoundEngine->addSoundSourceAlias(sound, name.c_str());
	}
#endif
}

void SoundManager::PlaySound(std::string name, float volume) {
#ifdef OS_WINDOWS
	irrklang::ISoundSource *sound = SoundEngine->getSoundSource(name.c_str());
	if (sound) {
		sound->setDefaultVolume(volume);
		SoundEngine->play2D(sound);
	}
#endif
}

void SoundManager::Clear() {
#ifdef OS_WINDOWS
	SoundEngine->removeAllSoundSources();
	SoundEngine->drop();
#endif
}
