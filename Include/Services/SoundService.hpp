#ifndef BURGERTIME_SOUNDSERVICE_HPP
#define BURGERTIME_SOUNDSERVICE_HPP

class MIX_Mixer;

namespace gla
{

class SoundService
{
public:
    explicit SoundService();
    ~SoundService() noexcept;

    SoundService(SoundService const&);
    auto operator=(SoundService const&) -> SoundService& = delete;
    SoundService(SoundService&&) noexcept = delete;
    auto operator=(SoundService&&) noexcept -> SoundService& = delete;

    void LoadAudio();


private:
    MIX_Mixer* m_mixer{};

};

}  // namespace gla

#endif  // BURGERTIME_SOUNDSERVICE_HPP
