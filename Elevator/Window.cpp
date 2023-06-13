#pragma once
#include "Window.h"

SDL_Window* Window::window = nullptr;
SDL_Renderer* Window::renderer = nullptr;
const int Window::WIDTH = 600;
const int Window::HEIGHT = 400;

SDL_Texture* Window::Text::charset = nullptr;
const int Window::Text::BMP_LETTER_SIZE = 8;
const int Window::Text::LETTER_SIZE = 16;