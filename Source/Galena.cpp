#include "ServiceLocator.hpp"
#include "Services/Sound.hpp"
#include "Services/Sound_Mixer.hpp"
#if USE_STEAMWORKS
#include <steam_api.h>
#endif

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <chrono>
#include <iostream>
#include <stdexcept>

#include "Events.hpp"
#include "Galena.hpp"
#include "SceneManager.hpp"
#include "Services/EventManager.hpp"
#include "Services/InputManager.hpp"
#include "Services/Renderer.hpp"
#include "Services/ResourceManager.hpp"

SDL_Window* g_window{};

namespace
{

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

void LoopCallback(void* arg)
{
    static_cast<gla::Galena*>(arg)->RunOneFrame();
}
#endif

void PrintSDLVersion()
{
    std::println("Compiled with SDL {}.{}.{}", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
    int version = SDL_GetVersion();
    std::println("Linked with SDL {}.{}.{}", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));

    std::println("Compiled with SDL_ttf {}.{}.{}", SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION, SDL_TTF_MICRO_VERSION);
    version = TTF_Version();
    std::println("Linked with SDL_ttf {}.{}.{}", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
}
}  // namespace

namespace gla
{

Galena::Galena(std::string const& windowName)
    : lastTime(std::chrono::high_resolution_clock::now())
{
    PrintSDLVersion();

    if (!SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        SDL_Log("Renderer error: %s", SDL_GetError());
        throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
    }

    g_window = SDL_CreateWindow(windowName.c_str(), 1024, 576, SDL_WINDOW_RESIZABLE);
    if (g_window == nullptr)
    {
        throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
    }

#if USE_STEAMWORKS
    if (not SteamAPI_Init())
        throw std::runtime_error(std::string("Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)."));
#endif

    ServiceLocator::Provide<Renderer>(g_window);
    ServiceLocator::Provide<ResourceManager>();
    ServiceLocator::Provide<InputManager>();
    ServiceLocator::Provide<EventManager>();

    // This way we can provide a specialised type for the sound locator
    ServiceLocator::Provide<Sound, Sound_Mixer>();
}

Galena::~Galena() noexcept
{
#if USE_STEAMWORKS
    SteamAPI_Shutdown();
#endif
    SceneManager::Get().Cleanup();

    ServiceLocator::Destroy<Renderer>();

    SDL_DestroyWindow(g_window);
    g_window = nullptr;
    SDL_Quit();
}

void Galena::Run(std::function<void()> const& load)
{
    load();

#ifndef __EMSCRIPTEN__
    while (!m_quit)
        RunOneFrame();
#else
    emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
#endif
}

void Galena::RunOneFrame()
{
    auto const now{ std::chrono::high_resolution_clock::now() };
    float const deltaTime{ std::chrono::duration<float>(now - lastTime).count() };
    lastTime = now;

#if USE_STEAMWORKS
    SteamAPI_RunCallbacks();
#endif
    m_quit = !ServiceLocator::Request<InputManager>().value()->ProcessInput();

    SceneManager::Get().Update(deltaTime);

    if (const auto em = ServiceLocator::Request<EventManager>(); em.has_value())
        em.value()->ExecuteQueuedEvents();

    if (const auto renderer = ServiceLocator::Request<Renderer>(); renderer.has_value())
        renderer.value()->Render();
}

}  // namespace gla