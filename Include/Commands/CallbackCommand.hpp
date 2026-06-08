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

    void Execute() override;

private:
    std::function<void()> m_callback;
};

}  // namespace gla

#endif  // GALENA_CALLBACKCOMMAND_H
