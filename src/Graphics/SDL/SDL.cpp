// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDL
*/
#include "SDL/SDL.hpp"
#include <SDL2/SDL_image.h>
#include <stdexcept>
#include <memory>
#include <utility>
#include <string>

SDL::~SDL() {
    renderer.reset();
    window.reset();
    SDL_Quit();
}

void SDL::init() {
    init(800.0f, 600.0f);
}

void SDL::init(float x, float y) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("SDL could not initialize! SDL Error: "
            + std::string(SDL_GetError()));

    window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>(
        SDL_CreateWindow(
            "Arcade",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            static_cast<int>(x),
            static_cast<int>(y),
            SDL_WINDOW_SHOWN),
        SDL_DestroyWindow);
    if (!window) {
        SDL_Quit();
        throw std::runtime_error("Window could not be created! SDL Error: "
            + std::string(SDL_GetError()));
    }

    renderer = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>(
        SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED),
        SDL_DestroyRenderer);
    if (!renderer) {
        SDL_Quit();
        throw std::runtime_error("Renderer could not be created! SDL Error: "
            + std::string(SDL_GetError()));
    }

    SDL_SetRenderDrawColor(renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
    windowWidth = static_cast<int>(x);
    windowHeight = static_cast<int>(y);
}

void SDL::addSprite(const std::string &textPath) {
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> tempSurface(
        IMG_Load(textPath.c_str()), SDL_FreeSurface);
    if (!tempSurface) {
        throw std::runtime_error("Unable to load image " + textPath +
            "! SDL_image Error: " +std::string(IMG_GetError()));
    }

    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer.get(),
        tempSurface.get());
    if (!newTexture) {
        throw std::runtime_error("Unable to create texture from" + textPath +
            "! SDL Error: " + std::string(SDL_GetError()));
    }

    int w, h;
    SDL_QueryTexture(newTexture, nullptr, nullptr, &w, &h);

    auto texturePtr = std::unique_ptr<SDL_Texture,
        decltype(&SDL_DestroyTexture)>(
        newTexture,
        SDL_DestroyTexture);

    _sprite.push_back(std::make_pair(std::move(texturePtr),
        std::make_pair(w, h)));
}

void SDL::render() {
    SDL_RenderClear(renderer.get());
    for (const auto &sprt : _sprite) {
        SDL_Rect destRect;
        destRect.x = (windowWidth - sprt.second.first) / 2;
        destRect.y = (windowHeight - sprt.second.second) / 2;
        destRect.w = sprt.second.first;
        destRect.h = sprt.second.second;

        SDL_RenderCopy(renderer.get(), sprt.first.get(), nullptr, &destRect);
    }
    SDL_RenderPresent(renderer.get());
}

InputModel::Input SDL::getInput() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return InputModel::Input::WINDOW_CLOSED;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    return InputModel::Input::UP;
                case SDLK_DOWN:
                    return InputModel::DOWN;
                case SDLK_LEFT:
                    return InputModel::LEFT;
                case SDLK_RIGHT:
                    return InputModel::RIGHT;
                case SDLK_RETURN:
                    return InputModel::ENTER;
                case SDLK_ESCAPE:
                    return InputModel::ESCAPE;
                case SDLK_SPACE:
                    return InputModel::SPACE;

                case SDLK_a:
                    return InputModel::A;
                case SDLK_b:
                    return InputModel::B;
                case SDLK_c:
                    return InputModel::C;
                case SDLK_d:
                    return InputModel::D;
                case SDLK_e:
                    return InputModel::E;
                case SDLK_f:
                    return InputModel::F;
                case SDLK_g:
                    return InputModel::G;
                case SDLK_h:
                    return InputModel::H;
                case SDLK_i:
                    return InputModel::I;
                case SDLK_j:
                    return InputModel::J;
                case SDLK_k:
                    return InputModel::K;
                case SDLK_l:
                    return InputModel::L;
                case SDLK_m:
                    return InputModel::M;
                case SDLK_n:
                    return InputModel::N;
                case SDLK_o:
                    return InputModel::O;
                case SDLK_p:
                    return InputModel::P;
                case SDLK_q:
                    return InputModel::Q;
                case SDLK_r:
                    return InputModel::R;
                case SDLK_s:
                    return InputModel::S;
                case SDLK_t:
                    return InputModel::T;
                case SDLK_u:
                    return InputModel::U;
                case SDLK_v:
                    return InputModel::V;
                case SDLK_w:
                    return InputModel::W;
                case SDLK_x:
                    return InputModel::X;
                case SDLK_y:
                    return InputModel::Y;
                case SDLK_z:
                    return InputModel::Z;

                case SDLK_0:
                    return InputModel::ZERO;
                case SDLK_1:
                    return InputModel::ONE;
                case SDLK_2:
                    return InputModel::TWO;
                case SDLK_3:
                    return InputModel::THREE;
                case SDLK_4:
                    return InputModel::FOUR;
                case SDLK_5:
                    return InputModel::FIVE;
                case SDLK_6:
                    return InputModel::SIX;
                case SDLK_7:
                    return InputModel::SEVEN;
                case SDLK_8:
                    return InputModel::EIGHT;
                case SDLK_9:
                    return InputModel::NINE;

                default:
                    return InputModel::UNKNOWN;
            }
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            switch (event.button.button) {
                case SDL_BUTTON_LEFT:
                    return InputModel::LEFT_CLICK;
                case SDL_BUTTON_RIGHT:
                    return InputModel::RIGHT_CLICK;
                case SDL_BUTTON_MIDDLE:
                    return InputModel::MIDDLE_CLICK;
                default:
                    return InputModel::UNKNOWN;
            }
        }
    }
    return InputModel::Input::UNKNOWN;
}

void SDL::drawElement() {
    addSprite("./chest.png");
}

void SDL::stop() {
    renderer.reset();
    window.reset();
    SDL_Quit();
}

extern "C" {
    __attribute__((constructor))
    const char *init_sdl(void) {
        return "Lib";
    }

    __attribute__((destructor))
    void fini_sdl(void) {
    }

    Arcade::IDisplayModule* entryPoint(void) {
        return new SDL();
    }
}

