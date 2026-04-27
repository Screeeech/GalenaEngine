#include "Commands/CallbackCommand.hpp"

#include <utility>

namespace gla
{

CallbackCommand::CallbackCommand(std::function<void()> callback)
    : m_callback(std::move(callback))
{
}

void CallbackCommand::Execute()
{
    m_callback();
}

}  // namespace gla