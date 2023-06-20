#pragma once
#include "Window.h"

SDL_Window* Window::window = nullptr;
SDL_Renderer* Window::renderer = nullptr;
const int Window::WIDTH = 1280;
const int Window::HEIGHT = 720;

SDL_Texture* Window::Text::charset = nullptr;
const int Window::Text::BMP_LETTER_SIZE = 8;
const int Window::Text::LETTER_SIZE = 16;