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

    Command(Command const&) = delete;
    auto operator=(Command const&) -> Command& = delete;
    Command(Command&&) = default;
    auto operator=(Command&&) -> Command& = default;

    virtual void Execute() = 0;
};

}  // namespace gla

#endif  // GALENA_COMMAND_H
