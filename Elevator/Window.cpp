#pragma once
#include "Window.h"

SDL_Window* Window::window = nullptr;
SDL_Renderer* Window::renderer = nullptr;
const int Window::WIDTH = 600;
const int Window::HEIGHT = 400;