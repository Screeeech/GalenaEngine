#ifndef BURGERTIME_SOUNDNULL_HPP
#define BURGERTIME_SOUNDNULL_HPP

#include "Services/ISound.hpp"

namespace gla
{

class SoundNull : public ISound
{
public:
    void PlayAudio(uint32_t /*audioID*/) override {};
    void QuitAudio() override {};
    void SetGlobalVolume(float /*volume*/) override {}
    [[nodiscard]] auto GetGlobalVolume() const -> float override { return 0.f; }
    void PlayTrack(const std::string& /*tag*/) override {};
    void LoadAudio(const std::string& /*path*/, uint32_t /*audioID*/) override {};
    void LoadPersistentAudioTrack(const std::string& /*path*/, const std::string& /*audioTag*/) override {};
};

}  // namespace gla

#endif  // BURGERTIME_SOUNDNULL_HPP
