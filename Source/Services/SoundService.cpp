#include "Services/SoundService.hpp"

#include <SDL3_mixer/SDL_mixer.h>

#include <algorithm>
#include <functional>
#include <print>
#include <stdexcept>

namespace gla
{

SoundService::SoundService()
{
    if (not MIX_Init())
        throw std::runtime_error("MIX_Init() failed");

    m_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);

    // We have to use bind_front here since stop_token always has to be the first parameter
    m_thread = std::jthread(std::bind_front(&SoundService::ProcessAudioCommands, this));
}

SoundService::~SoundService() noexcept
{
    // I know a jthread already joins automatically
    // but I need the thread to shut down before I destroy all the mixer resources
    if (m_thread.joinable())
        m_thread.join();

    std::ranges::for_each(m_persistentAudios, MIX_DestroyAudio);
    std::ranges::for_each(m_persistentTracks, MIX_DestroyTrack);
    std::ranges::for_each(m_oneTimeUseTracks, MIX_DestroyTrack);
    std::ranges::for_each(m_audioPerID, MIX_DestroyAudio, [](auto& pair) -> auto { return pair.second; });

    MIX_DestroyMixer(m_mixer);
}


SoundService::SoundService(SoundService const& /*other*/)
{
    // TODO: Again this is very bad, I need to figure out how to store services in a way that allows them to not have a copy constructor
    throw std::runtime_error("Cannot copy construct InputManager, no clue how to fix this yet");
}

void SoundService::PlayAudio(uint32_t audioID)
{
    std::scoped_lock const lock(m_mutex);

    m_audioCommands.push({ .type = SoundCommand::Type::Play, .id = audioID });
    m_cv.notify_one();
}

void SoundService::PlayTrack(std::string const& tag)
{
    std::scoped_lock const lock(m_mutex);

    m_audioCommands.push({ .type = SoundCommand::Type::PlayTag, .tag = tag });
    m_cv.notify_one();
}

void SoundService::QuitAudio()
{
    std::scoped_lock const lock(m_mutex);

    m_audioCommands.push({ .type = SoundCommand::Type::Quit });
    m_cv.notify_one();
}

void SoundService::LoadAudio(std::string const& path, uint32_t audioID)
{
    std::scoped_lock const lock(m_mutex);

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

void SoundService::LoadPersistentAudioTrack(std::string const& path, std::string const& audioTag)
{
    std::scoped_lock const lock(m_mutex);

    MIX_Audio* audio = MIX_LoadAudio(m_mixer, path.c_str(), true);
    MIX_Track* track = MIX_CreateTrack(m_mixer);

    MIX_SetTrackAudio(track, audio);
    MIX_TagTrack(track, audioTag.c_str());

    m_persistentAudios.emplace_back(audio);
    m_persistentTracks.emplace_back(track);
}

void SoundService::ProcessAudioCommands(std::stop_token stopToken)
{
    while (true)
    {
        std::unique_lock lock(m_mutex);
        m_cv.wait(lock, [this, &stopToken]() -> bool { return not m_audioCommands.empty() or stopToken.stop_requested(); });

        if (stopToken.stop_requested() and m_audioCommands.empty())
            break;

        if (m_audioCommands.empty())
            continue;  // spurious wake


        auto const [type, id, tag] = m_audioCommands.front();
        m_audioCommands.pop();

        switch (type)
        {
            case SoundCommand::Type::Play:
                PlaySingleTimeAudio(id);
                break;
            case SoundCommand::Type::PlayTag:
                PlayTaggedTracks(tag);
                break;
            case SoundCommand::Type::Quit:
                return;
            default:;
        }
    }
}

auto SoundService::RequestTrack() -> MIX_Track*
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

void SoundService::PlaySingleTimeAudio(uint32_t audioID)
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

void SoundService::PlayTaggedTracks(std::string const& tag) const
{
    // Doesn't need a lock, is only called from ProcessCommands which already locks the mutex
    MIX_PlayTag(m_mixer, tag.c_str(), 0);
}

void SoundService::SetTrackAudioNull([[maybe_unused]] void* pUserData, MIX_Track* track)
{
    MIX_SetTrackAudio(track, nullptr);
}


}  // namespace gla