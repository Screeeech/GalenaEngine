
#include "Commands/VolumeCommand.hpp"

#include "ServiceLocator.hpp"
#include "Services/ISound.hpp"

namespace gla
{

VolumeCommand::VolumeCommand(float volumeChange)
    : m_volumeChange(volumeChange)
{
}

void VolumeCommand::Execute()
{
    if (auto const soundService = ServiceLocator::Request<ISound>(); soundService.has_value())
    {
        float const currentVolume = soundService.value()->GetGlobalVolume();
        float const newVolume = std::clamp(currentVolume + m_volumeChange, m_volumeMin, m_volumeMax);
        soundService.value()->SetGlobalVolume(newVolume);

        std::println("Changing volume: {:.1f}", newVolume);
    }
    else
    {
        std::println("SoundService not found in VolumeCommand!");
    }

}

}  // namespace gla