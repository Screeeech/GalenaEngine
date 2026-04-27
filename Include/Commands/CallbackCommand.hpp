#ifndef GALENA_CALLBACKCOMMAND_H
#define GALENA_CALLBACKCOMMAND_H

#include <functional>

#include "Command.hpp"

namespace gla
{

class CallbackCommand : public Command
{
public:
    explicit CallbackCommand(std::function<void()> callback);
    ~CallbackCommand() noexcept override = default;

    CallbackCommand(CallbackCommand const&) = delete;
    auto operator=(CallbackCommand const&) -> CallbackCommand& = delete;
    CallbackCommand(CallbackCommand&&) = delete;
    auto operator=(CallbackCommand&&) -> CallbackCommand& = delete;

    void Execute() override;

private:
    std::function<void()> m_callback;
};

}  // namespace gla

#endif  // GALENA_CALLBACKCOMMAND_H
