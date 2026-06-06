#include "Locator.hpp"
#include "Services/ISound.hpp"
#include "Services/SoundNull.hpp"
#include "Services/SoundService.hpp"
#include "Time.hpp"
#if USE_STEAMWORKS
#include <steam_api.h>
#endif

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <chrono>
#include <iostream>
#include <stdexcept>

#include "Services/SceneManager.hpp"
#include "Events.hpp"
#include "Galena.hpp"
#include "Services/EventManager.hpp"
#include "Services/InputManager.hpp"
#include "Services/Renderer.hpp"
#include "Services/ResourceManager.hpp"
#include "Services/SoundNull.hpp"

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

Galena::Galena(std::string const& windowName, int fixedUpdateFrameCap)
    : m_fixedTimeStep(fixedUpdateFrameCap == 0 ? 0.f : 1.f / static_cast<float>(fixedUpdateFrameCap))
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

    Locator::Provide<Renderer>(g_window);
    Locator::Provide<ResourceManager>();
    Locator::Provide<InputManager>();
    Locator::Provide<EventManager>();
    Locator::Provide<SceneManager>();

#ifndef __EMSCRIPTEN__
    // Just using a null sound system while developing so I don't get driven crazy by startup sounds
    Locator::Provide<ISound, SoundNull>();
    //ServiceLocator::Provide<ISound, SoundService>();
#else
    // temporarily use null service on emscripten until I implement a singlethreaded sound service
    Locator::Provide<ISound, SoundNull>();
#endif
}

Galena::~Galena() noexcept
{
#if USE_STEAMWORKS
    SteamAPI_Shutdown();
#endif
    Locator::Get<SceneManager>().Cleanup();

    Locator::Destroy<Renderer>();
    Locator::Destroy<ISound>();


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
    auto& time = Time::Get();

    time.Update(m_fixedTimeStep);
    m_lag += time.DeltaTime();

#if USE_STEAMWORKS
    SteamAPI_RunCallbacks();
#endif
    m_quit = not Locator::Get<InputManager>().ProcessInput();

    auto lag = m_lag;
    while (lag >= m_fixedTimeStep)
    {
        Locator::Get<SceneManager>().FixedUpdate();
        lag -= m_fixedTimeStep;
    }
    Locator::Get<SceneManager>().Update();

    while (m_lag >= m_fixedTimeStep)
    {
        Locator::Get<SceneManager>().LateFixedUpdate();
        m_lag -= m_fixedTimeStep;
    }
    Locator::Get<SceneManager>().LateUpdate();

    Locator::Get<EventManager>().ExecuteQueuedEvents();
    Locator::Get<EventManager>().EraseFlaggedEventBindings();

    Locator::Get<SceneManager>().ExecuteReparentingQueue();
    Locator::Get<Renderer>().Render();
}

}  // namespace gla