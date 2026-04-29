#include "Services/SoundService.hpp"

#include <SDL3_mixer/SDL_mixer.h>

#include <stdexcept>

namespace gla
{

SoundService::SoundService()
{
    if (not MIX_Init())
        throw std::runtime_error("MIX_Init() failed");

    m_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);

}

}  // namespace gla