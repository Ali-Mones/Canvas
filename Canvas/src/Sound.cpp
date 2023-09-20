#include "Sound.h"
#include <miniaudio/miniaudio.h>
#include <cstdint>
#include <iostream>
#include <filesystem>

static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    SoundData* data = (SoundData*)pDevice->pUserData;
    ma_decoder* pDecoder = &data->Decoder;
    if (pDecoder == NULL)
        return;

    ma_device_set_master_volume(pDevice, data->Volume);
    ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);
    (void)pInput;
}

Sound::Sound(const char* filepath, bool looping)
{
    if (!std::filesystem::is_regular_file(filepath))
    {
        std::cout << "(ERROR): Couldn't load sound file located at " << std::filesystem::current_path().string() + '\\' + std::filesystem::path(filepath).make_preferred().string() << std::endl;
        return;
    }

    ma_decoder_init_file(filepath, NULL, &m_Data.Decoder);
    ma_data_source_set_looping(&m_Data.Decoder, looping);

	ma_device_config deviceConfig;
	deviceConfig = ma_device_config_init(ma_device_type_playback);
	deviceConfig.playback.format = m_Data.Decoder.outputFormat;
	deviceConfig.playback.channels = m_Data.Decoder.outputChannels;
	deviceConfig.sampleRate = m_Data.Decoder.outputSampleRate;
	deviceConfig.dataCallback = data_callback;
	deviceConfig.pUserData = &m_Data.Decoder;

    ma_device_init(NULL, &deviceConfig, &m_Device);
    ma_device_start(&m_Device);
}

Sound::~Sound()
{
    ma_device_uninit(&m_Device);
    ma_decoder_uninit(&m_Data.Decoder);
}

void Sound::Play()
{
    ma_device_start(&m_Device);
}

void Sound::Stop()
{
    ma_device_stop(&m_Device);
}
