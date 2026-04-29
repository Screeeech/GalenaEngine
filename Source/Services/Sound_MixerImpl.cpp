#include <SDL3_mixer/SDL_mixer.h>

#include <algorithm>
#include <functional>
#include <print>
#include <Services/SoundService.hpp>

namespace gla
{

class SoundService::Impl final
{
public:
    Impl(Impl const&) = delete;
    auto operator=(Impl const&) -> Impl& = delete;
    Impl(Impl&&) = delete;
    auto operator=(Impl&&) -> Impl& = delete;

    explicit Impl()
    {
        if (not MIX_Init())
            throw std::runtime_error("MIX_Init() failed");

        m_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    }

    ~Impl()
    {
        MIX_StopAllTracks(m_mixer, 0);
        // std::ranges::for_each(m_persistentTracks, );
        // std::ranges::for_each(m_oneTimeUseTracks, MIX_DestroyTrack);
        std::ranges::for_each(m_persistentAudios, MIX_DestroyAudio);
        std::ranges::for_each(m_audioPerID, MIX_DestroyAudio, [](auto& pair) -> auto { return pair.second; });

        MIX_DestroyMixer(m_mixer);
    }

    void LoadAudio(std::string const& path, uint32_t audioID)
    {
        auto it = m_audioPerID.find(audioID);
        if (it != m_audioPerID.end())
        {
            std::println("Audio of ID {} already exists. Overwriting that audio...", audioID);
            MIX_DestroyAudio(it->second);
            m_audioPerID.erase(it);
        }

        MIX_Audio* audio = MIX_LoadAudio(m_mixer, path.c_str(), true);
        m_audioPerID[audioID] = audio;
    }

    void LoadPersistentAudioTrack(std::string const& path, std::string const& audioTag)
    {
        MIX_Audio* audio = MIX_LoadAudio(m_mixer, path.c_str(), true);
        MIX_Track* track = MIX_CreateTrack(m_mixer);

        MIX_SetTrackAudio(track, audio);
        MIX_TagTrack(track, audioTag.c_str());

        m_persistentAudios.emplace_back(audio);
        m_persistentTracks.emplace_back(track);
    }

    auto RequestTrack() -> MIX_Track*
    {
        // Doesn't need a lock, is only called from ProcessCommands which already locks the mutex
        for (auto* track : m_oneTimeUseTracks)
        {
            if (MIX_TrackPlaying(track))
                continue;

            // Found valid track
            return track;
        }

        auto* track = MIX_CreateTrack(m_mixer);
        MIX_SetTrackStoppedCallback(track, &SetTrackAudioNull, nullptr);

        m_oneTimeUseTracks.emplace_back(track);

        return track;
    }


    void PlaySingleTimeAudio(uint32_t audioID)
    {
        // Doesn't need a lock, is only called from ProcessCommands which already locks the mutex
        auto* track = RequestTrack();
        auto* audio = m_audioPerID.at(audioID);

        if (not MIX_SetTrackAudio(track, audio))
        {
            std::println("Error setting track audio: {}", SDL_GetError());
            return;
        }

        MIX_PlayTrack(track, 0);
    }

    void PlayTaggedTracks(std::string const& tag) const
    {
        // Doesn't need a lock, is only called from ProcessCommands which already locks the mutex
        MIX_PlayTag(m_mixer, tag.c_str(), 0);
    }

    void SetGlobalVolume(float volume) { MIX_SetMixerGain(m_mixer, volume); }

    auto GetGlobalVolume() -> float { return MIX_GetMixerGain(m_mixer); }

    // clang-format off
    static void SetTrackAudioNull([[maybe_unused]] void* pUserData, MIX_Track* track)
    {
        MIX_SetTrackAudio(track, nullptr);
    }

    std::vector<MIX_Audio*> m_persistentAudios;
    std::vector<MIX_Track*> m_persistentTracks;

    std::unordered_map<uint32_t, MIX_Audio*> m_audioPerID;
    std::vector<MIX_Track*> m_oneTimeUseTracks;

    MIX_Mixer* m_mixer{};
};

SoundService::SoundService()
    : m_pImpl(std::make_unique<Impl>())
{
    // We have to use bind_front here since stop_token always has to be the first parameter
    m_thread = std::jthread(std::bind_front(&SoundService::ProcessAudioCommands, this));
}

SoundService::~SoundService() noexcept
{
    SoundService::QuitAudio();

    // I know a jthread already joins automatically
    // but I need the thread to shut down before I destroy all the mixer resources
    if (m_thread.joinable())
        m_thread.join();
}

void SoundService::LoadAudio(std::string const& path, uint32_t audioID)
{
    std::scoped_lock const lock(m_mutex);

    m_pImpl->LoadAudio(path, audioID);
}

void SoundService::LoadPersistentAudioTrack(std::string const& path, std::string const& audioTag)
{
    std::scoped_lock const lock(m_mutex);

    m_pImpl->LoadPersistentAudioTrack(path, audioTag);

}

void SoundService::SetGlobalVolume(float volume)
{
    m_pImpl->SetGlobalVolume(volume);
}

auto SoundService::GetGlobalVolume()const -> float
{
    return m_pImpl->GetGlobalVolume();
}

void SoundService::PlaySingleTimeAudio(uint32_t audioID)
{
    m_pImpl->PlaySingleTimeAudio(audioID);
}

void SoundService::PlayTaggedTracks(std::string const& tag)
{
    m_pImpl->PlayTaggedTracks(tag);
}

}  // namespace gla