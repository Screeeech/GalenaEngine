#include "Services/SoundService.hpp"

#include <algorithm>
#include <functional>
#include <print>
#include <stdexcept>
#include <thread>

namespace gla
{

void SoundService::QuitAudio()
{
    std::scoped_lock const lock(m_mutex);

    m_audioCommands.push({ .type = SoundCommand::Type::Quit });
    m_cv.notify_one();
}


void SoundService::PlayTrack(std::string const& tag)
{
    std::scoped_lock const lock(m_mutex);

    m_audioCommands.push({ .type = SoundCommand::Type::PlayTag, .tag = tag });
    m_cv.notify_one();
}

void SoundService::PlayAudio(uint32_t audioID)
{
    std::scoped_lock const lock(m_mutex);

    m_audioCommands.push({ .type = SoundCommand::Type::Play, .id = audioID });
    m_cv.notify_one();
}

void SoundService::ProcessAudioCommands(std::stop_token stopToken)
{
    while (true)
    {
        std::unique_lock lock(m_mutex);
        m_cv.wait(lock, [this, &stopToken]() -> bool { return not m_audioCommands.empty() or stopToken.stop_requested(); });

        if (stopToken.stop_requested() and m_audioCommands.empty())
            break;

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

}  // namespace gla