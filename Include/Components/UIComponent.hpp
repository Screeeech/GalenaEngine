#ifndef GALENA_UICOMPONENT_H
#define GALENA_UICOMPONENT_H
#include <functional>

#include "Component.hpp"

namespace gla
{

class GameObject;

class UIComponent : public Component
{

public:
    UIComponent(GameObject* pOwner, std::function<void(GameObject* pCaller)> drawFunc);
    ~UIComponent() override;

    void DrawUI() const;

protected:
    void OnActivate() override;

private:
    std::function<void(GameObject* pCaller)> m_drawFunc;
};


}  // namespace gla

#endif  // GALENA_UICOMPONENT_H
