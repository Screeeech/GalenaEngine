#include "Services/InputManager.hpp"

#include <imgui_impl_sdl3.h>
#include <SDL3/SDL.h>

#include <algorithm>
#include <print>

#include "Commands/CallbackCommand.hpp"
#include "Events.hpp"
#include "Locator.hpp"
#include "Services/EventManager.hpp"
#include "Utils.hpp"

namespace vw = std::ranges::views;

namespace gla
{

InputManager::~InputManager() noexcept
{
    for (auto const& gamepad : m_gamepadHandles | vw::values)
        SDL_CloseGamepad(gamepad);
}

auto InputManager::ProcessInput() -> bool
{
    ProcessInputHeld();

    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        ImGui_ImplSDL3_ProcessEvent(&e);

        if (e.type == SDL_EVENT_QUIT)
            return false;

        switch (e.type)
        {
            case SDL_EVENT_KEY_UP:
                if (auto const playerIndex = GetOrAssignPlayerIndex(); playerIndex.has_value())
                    HandleInputEvent(Input::Type::released, e.key.scancode, *playerIndex);
                break;

            case SDL_EVENT_KEY_DOWN:
                if (e.key.repeat)
                    break;

                if (auto const playerIndex = GetOrAssignPlayerIndex(); playerIndex.has_value())
                    HandleInputEvent(Input::Type::pressed, e.key.scancode, *playerIndex);
                break;

            case SDL_EVENT_GAMEPAD_ADDED:
                HandleGamepadConnect(e.gdevice.which);
                break;

            case SDL_EVENT_GAMEPAD_REMOVED:
                HandleGamepadDisconnect(e.gdevice.which);
                break;

            case SDL_EVENT_GAMEPAD_BUTTON_UP:

                if (auto const playerIndex = GetOrAssignPlayerIndex(e.gbutton.which); playerIndex.has_value())
                    HandleInputEvent(Input::Type::released, static_cast<SDL_GamepadButton>(e.gbutton.button), *playerIndex);
                break;

            case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
                if (e.key.repeat)
                    break;

                if (auto const playerIndex = GetOrAssignPlayerIndex(e.gbutton.which); playerIndex.has_value())
                    HandleInputEvent(Input::Type::pressed, static_cast<SDL_GamepadButton>(e.gbutton.button), *playerIndex);
                break;

            default:;
        }
    }

    return true;
}

void InputManager::ProcessInputHeld()
{
    for (auto& [action, input] : m_registeredInputs)
    {
        if (input.type != Input::Type::held)
            continue;

        if (std::holds_alternative<SDL_Scancode>(input.data))
        {
            auto const* pKeyboard = SDL_GetKeyboardState(nullptr);
            auto const key = std::get<SDL_Scancode>(input.data);
            if (input.type != Input::Type::held or not pKeyboard[key])
                continue;
        }
        else
        {
            auto* gamepad = GetGamepadForPlayer(action.playerIndex);
            if (not gamepad)
                continue;

            auto const button = std::get<SDL_GamepadButton>(input.data);
            if (not SDL_GetGamepadButton(gamepad, button))
                continue;
        }

        auto [fst, snd] = m_commands.equal_range(action);
        for (auto& [k, command] : std::ranges::subrange(fst, snd))
        {
            command->Execute();
        }
    }
}

void InputManager::UnbindAction(const ActionID& name, int playerIndex)
{
    m_commands.erase(Action{ name, playerIndex });
}

auto InputManager::GetActivePlayerIndices() -> std::set<int>
{
    return m_usedPlayerIndices;
}

void InputManager::HandleGamepadConnect(SDL_JoystickID id)
{
    SDL_Gamepad* newPad = SDL_OpenGamepad(id);
    if (not newPad)
        return;

    m_gamepadHandles[id] = newPad;
    m_registeredGamepads[id] = std::nullopt;
    std::println("Gamepad connected, Joystick ID: {}", id);
}

void InputManager::HandleGamepadDisconnect(SDL_JoystickID id)
{
    std::println("Gamepad disconnected, Joystick ID: {}", id);
    if (auto const it = m_registeredGamepads.find(id); it != m_registeredGamepads.end())
    {
        if (it->second.has_value())
        {
            int const playerIndex = *it->second;
            FreePlayerIndex(playerIndex, id);
        }
        m_registeredGamepads.erase(it);
    }
    if (auto const handleIt = m_gamepadHandles.find(id); handleIt != m_gamepadHandles.end())
    {
        if (handleIt->second)
            SDL_CloseGamepad(handleIt->second);
        m_gamepadHandles.erase(handleIt);
    }
}

auto InputManager::GetOrAssignPlayerIndex(SDL_JoystickID id) -> std::optional<int>
{
    auto& playerIndex = m_registeredGamepads.at(id);
    if (playerIndex.has_value())
        return playerIndex;

    if (m_usedPlayerIndices.size() >= maxPlayers)
    {
        std::println("Max player count ({}) reached, cannot assign Gamepad", maxPlayers);
        return std::nullopt;
    }

    int newIndex{};
    while (m_usedPlayerIndices.contains(newIndex))
        ++newIndex;

    playerIndex = newIndex;
    m_usedPlayerIndices.insert(newIndex);
    m_playerIndexToJoystick[newIndex] = id;

    std::println("Player {} connected (Joystick ID: {})", newIndex, id);
    Locator::Get<EventManager>().InvokeEvent(PlayerConnectionEvent("OnPlayerConnect"_h, newIndex, true));

    return newIndex;
}

auto InputManager::GetOrAssignPlayerIndex() -> std::optional<int>
{
    if (m_keyboardPlayerIndex.has_value())
        return m_keyboardPlayerIndex;

    if (m_usedPlayerIndices.size() >= maxPlayers)
    {
        std::println("Max player count ({}) reached, cannot assign Gamepad", maxPlayers);
        return std::nullopt;
    }

    int newIndex{};
    while (m_usedPlayerIndices.contains(newIndex))
        ++newIndex;

    m_usedPlayerIndices.insert(newIndex);
    m_keyboardPlayerIndex = newIndex;

    std::println("Player {} connected (Keyboard)", newIndex);
    Locator::Get<EventManager>().InvokeEvent(PlayerConnectionEvent("OnPlayerConnect"_h, newIndex, false));

    return newIndex;
}

void InputManager::FreePlayerIndex(int playerIndex, SDL_JoystickID id)
{
    m_usedPlayerIndices.erase(playerIndex);
    m_playerIndexToJoystick.erase(playerIndex);

    std::println("Player {} disconnected (Joystick ID: {})", playerIndex, id);
    Locator::Get<EventManager>().InvokeEvent(PlayerConnectionEvent("OnPlayerDisconnect"_h, playerIndex, true));
}

auto InputManager::GetGamepadForPlayer(int playerIndex) const -> SDL_Gamepad*
{
    auto const it = m_playerIndexToJoystick.find(playerIndex);
    if (it == m_playerIndexToJoystick.end())
        return nullptr;

    auto const handleIt = m_gamepadHandles.find(it->second);
    if (handleIt == m_gamepadHandles.end())
        return nullptr;

    return handleIt->second;
}
}  // namespace gla