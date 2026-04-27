#ifndef GALENA_COMMAND_H
#define GALENA_COMMAND_H

namespace gla
{
class GameObject;

class Command
{
public:
    Command() = default;
    virtual ~Command() noexcept = default;
    virtual void Execute() = 0;
};

}  // namespace gla

#endif  // GALENA_COMMAND_H
