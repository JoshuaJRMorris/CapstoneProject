#include "MusicPlayer.h"


MusicPlayer::MusicPlayer()
	: mMusic()
	, mFilenames()
	, mVolume(0.f)
{
	mFilenames[Music::MenuTheme] = "Media/Music/DONLV.ogg";
	mFilenames[Music::MissionTheme] = "Media/Music/Monster.ogg";
	mFilenames[Music::SurfinBird] = "Media/Music/SurfinBirdTrashmen.ogg";
	mFilenames[Music::SurfinBirdMetal] = "Media/Music/SurfinBird.ogg";
	mFilenames[Music::FreeBird] = "Media/Music/FreeBird.ogg";
	mFilenames[Music::Pigeons] = "Media/Music/Pigeons.ogg";
}

void MusicPlayer::play(Music::ID theme)
{
	std::string filename = mFilenames[theme];

	if (!mMusic.openFromFile(filename))
		throw std::runtime_error("Music " + filename + " could not be loaded.");

	mMusic.setVolume(mVolume);
	mMusic.setLoop(true);
	mMusic.play();
}

void MusicPlayer::stop()
{
	mMusic.stop();
}

void MusicPlayer::setVolume(float volume)
{
	mVolume = volume;
}

void MusicPlayer::setPaused(bool paused)
{
	if (paused)
		mMusic.pause();
	else
		mMusic.play();
}