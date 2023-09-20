#pragma once
#include <miniaudio/miniaudio.h>
#include <cstdint>

struct SoundData
{
	ma_decoder Decoder;
	float Volume = 1.0f;
};

class Sound
{
public:
	Sound(const char* filepath, bool looping);
	~Sound();
	void Play();
	void Stop();
	void SetVolume(float volume) { m_Data.Volume = volume; }
	float Volume() { return m_Data.Volume; }
	bool IsPlaying() { return (ma_device_get_state(&m_Device) == ma_device_state_started); }
private:
	ma_device m_Device;
	SoundData m_Data;
};
