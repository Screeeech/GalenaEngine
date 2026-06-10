#include "Commands/VolumeCommand.hpp"

#include <algorithm>
#include <utility>

#include "Locator.hpp"
#include "Services/Sound.hpp"

namespace gla
{

VolumeCommand::VolumeCommand(float volumeChange)
    : m_volumeChange(volumeChange)
{
}

void VolumeCommand::Execute()
{
    auto& soundService = Locator::Get<Sound>();
    float const currentVolume = soundService.GetGlobalVolume();
    float const newVolume = std::clamp(currentVolume + m_volumeChange, Sound::minimumVolume, Sound::maximumVolume);
    soundService.SetGlobalVolume(newVolume);

    std::println("Changing volume: {:.1f}", newVolume);
}

}  // namespace gla