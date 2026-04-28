#if USE_STEAMWORKS
#include <steam_api.h>
#endif

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <chrono>
#include <iostream>
#include <stdexcept>

#include "EventManager.hpp"
#include "Events.hpp"
#include "Galena.hpp"
#include "InputManager.hpp"
#include "Renderer.hpp"
#include "ResourceManager.hpp"
#include "SceneManager.hpp"

SDL_Window* g_window{};

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

void LoopCallback(void* arg)
{
    static_cast<Galena*>(arg)->RunOneFrame();
}
#endif

void PrintSDLVersion()
{
    std::println("Compiled with SDL {}.{}.{}", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
    int version = SDL_GetVersion();
    std::println("Linked with SDL {}.{}.{}", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version),
                 SDL_VERSIONNUM_MICRO(version));

    std::println("Compiled with SDL_ttf {}.{}.{}", SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION, SDL_TTF_MICRO_VERSION);
    version = TTF_Version();
    std::println("Linked with SDL_ttf {}.{}.{}", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version),
                 SDL_VERSIONNUM_MICRO(version));
}

namespace gla
{

Galena::Galena(std::string const& windowName)
    : lastTime(std::chrono::high_resolution_clock::now())
{
    PrintSDLVersion();

    if(!SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        SDL_Log("Renderer error: %s", SDL_GetError());
        throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
    }

    g_window = SDL_CreateWindow(windowName.c_str(), 1024, 576, SDL_WINDOW_RESIZABLE);
    if(g_window == nullptr)
    {
        throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
    }

#if USE_STEAMWORKS
    if(not SteamAPI_Init())
        throw std::runtime_error(std::string("Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)."));
#endif

    Renderer::Get().Init(g_window);
    ResourceManager::Init();
    InputManager::Get().Init();
}

Galena::~Galena() noexcept
{
#if USE_STEAMWORKS
    SteamAPI_Shutdown();
#endif
    SceneManager::Get().Cleanup();
    Renderer::Get().Destroy();
    SDL_DestroyWindow(g_window);
    g_window = nullptr;
    SDL_Quit();
}

void Galena::Run(std::function<void()> const& load)
{
    load();

#ifndef __EMSCRIPTEN__
    while(!m_quit)
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
    m_quit = !InputManager::Get().ProcessInput();
    SceneManager::Get().Update(deltaTime);
    EventManager::Get().ExecuteQueuedEvents();
    Renderer::Get().Render();
}

}  // namespace gla