#ifndef BURGERTIME_SOUND_MIXER_HPP
#define BURGERTIME_SOUND_MIXER_HPP
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>

#include "Sound.hpp"

namespace gla
{

// Change to variant
struct SoundCommand final
{
    enum class Type : uint8_t
    {
        Play,
        PlayTag,
        Stop,
        StopAll,
        Quit,
    } type{};

    uint32_t id{};
    std::string tag;
    bool looping{};
};

class SoundService final : public Sound
{
public:
    explicit SoundService();
    ~SoundService() noexcept override;

    SoundService(SoundService const&) = delete;
    SoundService(SoundService&&) = delete;
    auto operator=(SoundService const&) -> SoundService& = delete;
    auto operator=(SoundService&&) -> SoundService& = delete;

    void PlayAudio(uint32_t audioID) override;
    void PlayTrack(std::string const& tag, bool looping = false) override;
    void StopTrack(std::string const& tag) override;
    void QuitAudio() override;
    void LoadAudio(std::string const& path, uint32_t audioID) override;
    void LoadPersistentAudioTrack(std::string const& path, std::string const& audioTag) override;
    void SetGlobalVolume(float volume) override;
    [[nodiscard]] auto GetGlobalVolume() const -> float override;

private:
    void ProcessAudioCommands(std::stop_token stopToken);

    void PlaySingleTimeAudio(uint32_t audioID) const;
    void PlayTaggedTracks(std::string const& tag, bool looping) const;
    void StopTaggedTrack(std::string const& tag) const;

    class Impl;
    std::unique_ptr<Impl> m_pImpl;

    std::jthread m_thread;
    std::condition_variable m_cv;

    std::mutex m_commandQueueMutex;
    std::queue<SoundCommand> m_audioCommands;
};

}  // namespace gla

#endif  // BURGERTIME_SOUND_MIXER_HPP
