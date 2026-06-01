#ifndef GALENA_GAMEOBJECTCOMMAND_H
#define GALENA_GAMEOBJECTCOMMAND_H

#include "Command.hpp"

namespace gla
{

class GameObjectCommand : public Command
{
public:
    explicit GameObjectCommand(GameObject* pCaller)
        : m_pCaller(pCaller)
    {
    }

    void Execute() override = 0;

protected:
    GameObject* m_pCaller;
};

}  // namespace gla

#endif  // GALENA_GAMEOBJECTCOMMAND_H
