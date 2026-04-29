#ifndef BURGERTIME_SOUNDSERVICE_HPP
#define BURGERTIME_SOUNDSERVICE_HPP

namespace gla
{

class ISound
{
public:
    explicit ISound() = default;
    virtual ~ISound() noexcept = default;

    ISound(ISound const&) = delete;
    auto operator=(ISound const&) -> ISound& = delete;
    ISound(ISound&&) noexcept = delete;
    auto operator=(ISound&&) noexcept -> ISound& = delete;

    virtual void PlayAudio(uint32_t audioID) = 0;
    virtual void PlayTrack(std::string const& tag) = 0;
    virtual void QuitAudio() = 0;
    virtual void LoadAudio(std::string const& path, uint32_t audioID) = 0;
    virtual void LoadPersistentAudioTrack(std::string const& path, std::string const& audioTag) = 0;
    virtual void SetGlobalVolume(float volume) = 0;
    virtual auto GetGlobalVolume() const -> float = 0;

};

}  // namespace gla

#endif  // BURGERTIME_SOUNDSERVICE_HPP
