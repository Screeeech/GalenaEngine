#ifndef GALENA_VOLUMECOMMAND_HPP
#define GALENA_VOLUMECOMMAND_HPP
#include "Command.hpp"

namespace gla
{

class VolumeCommand : public Command
{
public:
    explicit VolumeCommand(float volumeChange);

    void Execute() override;

private:

    float m_volumeChange;
};

}  // namespace gla

#endif  // GALENA_VOLUMECOMMAND_HPP
