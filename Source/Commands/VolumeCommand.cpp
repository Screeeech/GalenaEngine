#include "Commands/VolumeCommand.hpp"

#include <algorithm>
#include <utility>

#include "Locator.hpp"
#include "Services/ISound.hpp"

namespace gla
{

VolumeCommand::VolumeCommand(float volumeChange)
    : m_volumeChange(volumeChange)
{
}

void VolumeCommand::Execute()
{
    auto& soundService = Locator::Get<ISound>();
    float const currentVolume = soundService.GetGlobalVolume();
    float const newVolume = std::clamp(currentVolume + m_volumeChange, m_volumeMin, m_volumeMax);
    soundService.SetGlobalVolume(newVolume);

    std::println("Changing volume: {:.1f}", newVolume);
}

}  // namespace gla