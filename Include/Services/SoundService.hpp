#ifndef BURGERTIME_SOUNDSERVICE_HPP
#define BURGERTIME_SOUNDSERVICE_HPP

#include <condition_variable>
#include <mutex>
#include <cstdint>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>

class MIX_Mixer;
class MIX_Audio;
class MIX_Track;

namespace gla
{


struct SoundCommand final
{
    enum class Type : uint8_t
    {
        Play,
        PlayTag,
        // TODO: Add support for other sound commands
        //Pause,
        //VolumeUp,
        //VolumeDown,
        //StopAll,
        Quit,
    } type{};

    uint32_t id{};
    std::string tag;
};

class SoundService
{
public:
    explicit SoundService();
    virtual ~SoundService() noexcept;

    SoundService(SoundService const&);
    auto operator=(SoundService const&) -> SoundService& = delete;
    SoundService(SoundService&&) noexcept = delete;
    auto operator=(SoundService&&) noexcept -> SoundService& = delete;

    void PlayAudio(uint32_t audioID);
    void PlayTrack(std::string const& tag);
    void QuitAudio();
    void LoadAudio(std::string const& path, uint32_t audioID);
    void LoadPersistentAudioTrack(std::string const& path, std::string const& audioTag);

    void ProcessAudioCommands(std::stop_token stopToken);

private:
    auto RequestTrack() -> MIX_Track*;

    void PlaySingleTimeAudio(uint32_t audioID);
    void PlayTaggedTracks(std::string const& tag) const;

    static void SetTrackAudioNull(void* pUserData, MIX_Track* track);

    MIX_Mixer* m_mixer{};

    std::vector<MIX_Audio*> m_persistentAudios;
    std::vector<MIX_Track*> m_persistentTracks;

    std::unordered_map<uint32_t, MIX_Audio*> m_audioPerID;
    std::vector<MIX_Track*> m_oneTimeUseTracks;

    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::jthread m_thread;
    std::queue<SoundCommand> m_audioCommands;
};

}  // namespace gla

#endif  // BURGERTIME_SOUNDSERVICE_HPP
