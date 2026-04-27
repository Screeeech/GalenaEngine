#include "Commands/GameObjectCommand.hpp"

namespace gla
{

GameObjectCommand::GameObjectCommand(GameObject* pCaller)
    : m_pCaller(pCaller)
{
}

void GameObjectCommand::Execute() {}

}  // namespace dae