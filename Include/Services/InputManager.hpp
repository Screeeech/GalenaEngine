#ifndef GALENA_INPUTMANAGER_HPP
#define GALENA_INPUTMANAGER_HPP

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_scancode.h>

#include <memory>
#include <optional>
#include <print>
#include <ranges>
#include <set>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "Command.hpp"

namespace gla
{
using ActionID = uint32_t;

template<typename InputDataType>
concept InputConcept = std::same_as<InputDataType, SDL_Scancode> or std::same_as<InputDataType, SDL_GamepadButton>;

struct Input
{
    using InputData = std::variant<SDL_Scancode, SDL_GamepadButton>;
    InputData data;
    enum class Type : uint8_t
    {
        released,
        pressed,
        held
    } type{};

    explicit Input(SDL_Scancode _key, Type _type = Type::released)
        : data{ _key }
        , type{ _type }
    {
    }

    explicit Input(SDL_GamepadButton _key, Type _type = Type::released)
        : data{ _key }
        , type{ _type }
    {
    }

    template<InputConcept T>
    auto InputDataMatches(T otherData) const -> bool
    {
        if constexpr (std::same_as<T, SDL_Scancode>)
        {
            if (not std::holds_alternative<SDL_Scancode>(data))
                return false;

            const auto ourKey = std::get<SDL_Scancode>(data);
            return ourKey == otherData;
        }
        else
        {
            if (not std::holds_alternative<SDL_GamepadButton>(data))
                return false;

            const auto ourButton = std::get<SDL_GamepadButton>(data);
            return ourButton == otherData;
        }
    }
};

struct Action
{
    ActionID name;
    int playerIndex{};

    auto operator==(const Action& other) const -> bool { return name == other.name and playerIndex == other.playerIndex; }

    struct Hash
    {
        auto operator()(const Action& action) const noexcept -> size_t { return std::hash<ActionID>{}(action.name); }
    };
};

class InputManager final
{
public:
    InputManager();
    ~InputManager() noexcept;

    InputManager(InputManager const&) = delete;
    auto operator=(InputManager const&) -> InputManager& = delete;
    InputManager(InputManager&&) = delete;
    auto operator=(InputManager&&) -> InputManager& = delete;

    auto ProcessInput() -> bool;
    void ProcessInputHeld();

    template<InputConcept T>
    void RegisterInput(T inputData, Input::Type inputType, const ActionID& name, int playerIndex)
    {
        if (m_registeredInputs.contains(Action{ .name = name, .playerIndex = playerIndex }))
        {
            if constexpr (std::same_as<T, SDL_Scancode>)
                std::println(
                    "Warning! \tAttempting to bind Input: \'{}\' to Action: ({}, {}) which is already bound!",
                    SDL_GetScancodeName(inputData),
                    name,
                    playerIndex);
            else if constexpr (std::same_as<T, SDL_GamepadButton>)
                std::println(
                    "Warning! \tAttempting to bind Input: \'{}\' to Action: ({}, {}) which is already bound!",
                    SDL_GetGamepadStringForButton(inputData),
                    name,
                    playerIndex);
        }

        m_registeredInputs.insert(std::pair{ Action{ .name = name, .playerIndex = playerIndex }, Input{ inputData, inputType } });
    }

    template<InputConcept T>
    void UnregisterInput(T inputData, const ActionID& name, int playerIndex)
    {
        std::erase_if(
            m_registeredInputs,
            [inputData, name, playerIndex](const std::pair<Action, Input>& input) -> bool
            { return input.first.name == name and input.first.playerIndex == playerIndex and input.second.InputDataMatches(inputData); });
    }

    template<typename CommandType, typename... Args>
        requires std::derived_from<CommandType, Command>
    void BindAction(const ActionID& name, int playerIndex, Args... args)
    {
        m_commands.emplace(Action{ .name = name, .playerIndex = playerIndex }, std::make_unique<CommandType>(args...));
    }

    void UnbindAction(const ActionID& name, int playerIndex);

private:
    template<InputConcept T>
    auto HandleInputEvent(Input::Type inputType, T inputData, int playerIndex) -> bool
    {
        auto actionFilter = [&](const auto& pair) -> bool
        {
            const auto& [action, registeredInput] = pair;
            return action.playerIndex == playerIndex and registeredInput.type != inputType and registeredInput.InputDataMatches(inputData);
        };

        auto getCommands = [this](const Action& action) -> auto
        {
            auto [fst, snd] = m_commands.equal_range(action);
            return std::ranges::subrange(fst, snd) | std::views::values;
        };

        // clang-format off
        auto allCommands = m_registeredInputs
            | std::views::filter(actionFilter)
            | std::views::keys
            | std::views::transform(getCommands)
            | std::views::join;
        // clang-format on

        bool executed{};
        for (auto& command : allCommands)
        {
            command->Execute();
            executed = true;
        }

        return executed;
    }

    void HandleGamepadConnect(SDL_JoystickID id);
    void HandleGamepadDisconnect(SDL_JoystickID id);

    std::unordered_multimap<Action, Input, Action::Hash> m_registeredInputs;
    std::unordered_multimap<Action, std::unique_ptr<Command>, Action::Hash> m_commands;

    auto GetOrAssignPlayerIndex(SDL_JoystickID id) -> int;
    auto GetOrAssignPlayerIndex() -> int;

    void FreePlayerIndex(int playerIndex, SDL_JoystickID id);
    auto GetGamepadForPlayer(int playerIndex) const -> SDL_Gamepad*;

    std::unordered_map<SDL_JoystickID, std::optional<int>> m_registeredGamepads;
    std::optional<int> m_keyboardPlayerIndex;

    std::unordered_map<int, SDL_JoystickID> m_playerIndexToJoystick;
    std::set<int> m_usedPlayerIndices;

    std::unordered_map<SDL_JoystickID, SDL_Gamepad*> m_gamepadHandles;
    // SDL_JoystickID* m_joystickIDs;
};

}  // namespace gla

#endif  // GALENA_INPUTMANAGER_HPP