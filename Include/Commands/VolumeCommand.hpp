#ifndef GALENA_VOLUMECOMMAND_HPP
#define GALENA_VOLUMECOMMAND_HPP
#include "Command.hpp"

namespace gla
{

class VolumeCommand : public Command
{
public:
    explicit VolumeCommand(float volumeChange);
    ~VolumeCommand() override = default;

    VolumeCommand(VolumeCommand const&) = delete;
    auto operator=(VolumeCommand const&) -> VolumeCommand& = delete;
    VolumeCommand(VolumeCommand&&) = delete;
    auto operator=(VolumeCommand&&) -> VolumeCommand& = delete;

    void Execute() override;

private:
    static constexpr float m_volumeMin{ 0.0f };
    static constexpr float m_volumeMax{ 1.5f };

    float m_volumeChange;
};

}  // namespace gla

#endif  // GALENA_VOLUMECOMMAND_HPP
