#ifndef BURGERTIME_SOUNDSERVICE_HPP
#define BURGERTIME_SOUNDSERVICE_HPP

#include <cstdint>
#include <string>

namespace gla
{

class Sound
{
public:
    static constexpr float defaultVolume{ 0.2f };
    static constexpr float minimumVolume{ 0.0f };
    static constexpr float maximumVolume{ 1.5f };

    explicit Sound() = default;
    virtual ~Sound() noexcept = default;

    Sound(Sound const&) = delete;
    auto operator=(Sound const&) -> Sound& = delete;
    Sound(Sound&&) noexcept = delete;
    auto operator=(Sound&&) noexcept -> Sound& = delete;

    virtual void PlayAudio(uint32_t audioID) = 0;
    virtual void PlayTrack(std::string const& tag) = 0;
    virtual void QuitAudio() = 0;
    virtual void LoadAudio(std::string const& path, uint32_t audioID) = 0;
    virtual void LoadPersistentAudioTrack(std::string const& path, std::string const& audioTag) = 0;
    virtual void SetGlobalVolume(float volume) = 0;
    [[nodiscard]] virtual auto GetGlobalVolume() const -> float = 0;

};

}  // namespace gla

#endif  // BURGERTIME_SOUNDSERVICE_HPP
