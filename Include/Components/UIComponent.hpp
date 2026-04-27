#ifndef GALENA_UICOMPONENT_H
#define GALENA_UICOMPONENT_H
#include <functional>

#include "Component.hpp"

namespace gla
{

class UIComponent : public Component
{
public:
    UIComponent(GameObject* pOwner, std::function<void(GameObject* pCaller)> drawFunc);
    ~UIComponent() noexcept override;

    UIComponent(UIComponent const&) = delete;
    auto operator=(UIComponent const&) -> UIComponent = delete;
    UIComponent(UIComponent &&) = delete;
    auto operator=(UIComponent &&) -> UIComponent = delete;

    void Update(float deltaTime) override;
    void DrawUI() const;

private:
    std::function<void(GameObject* pCaller)> m_drawFunc;
};


}  // namespace gla

#endif  // GALENA_UICOMPONENT_H
