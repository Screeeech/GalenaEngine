#ifndef GALENA_GAMEOBJECTCOMMAND_H
#define GALENA_GAMEOBJECTCOMMAND_H

#include "Command.hpp"

namespace gla
{

class GameObjectCommand : public Command
{
public:
    explicit GameObjectCommand(GameObject* pCaller);

    void Execute() override;

protected:
    GameObject* m_pCaller;
};

}  // namespace gla

#endif  // GALENA_GAMEOBJECTCOMMAND_H
