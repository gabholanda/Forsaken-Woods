#include "Window.h"
#include <SDL.h>
#include <iostream> /// Umer likes this over printf() - too bad
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "gl3w.h"
#include "gl3w.c"



Window::Window(int width_, int height_) {
	screenSurface = nullptr;
	window = nullptr;
	width = width_;
	height = height_;
}

bool Window::OnCreate() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0) {
		std::cout << "SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	window = SDL_CreateWindow("Forsaken Woods",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width,
		height,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);  // Ensure SDL_WINDOW_OPENGL flag is set
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
	if (!window) {
		std::cout << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (!glContext) {
		std::cout << "SDL_GL_CreateContext failed: " << SDL_GetError() << std::endl;
		return false;
	}

	// Initialize OpenGL loader
	if (gl3wInit() != 0) {
		std::cout << "Failed to initialize OpenGL loader (gl3w)" << std::endl;
		return false;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init("#version 130");  // Specify GLSL version if necessary

	screenSurface = SDL_GetWindowSurface(window);
	if (screenSurface == nullptr) {
		std::cout << "SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	renderer = SDL_GetRenderer(window);
	if (!renderer) {
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	}
	if (!renderer) {
		std::cout << "SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}

void Window::OnDestroy() {
	/// Kill the surface first
	if (screenSurface) {
		SDL_FreeSurface(screenSurface);
	}

	// Delete the renderer
	if (renderer) {
		SDL_DestroyRenderer(renderer);
	}

	/// Now kill the window
	if (window) {
		SDL_DestroyWindow(window);
	}

	///Exit the SDL subsystems
	SDL_Quit();

}

Window::~Window() {}

SDL_Window* Window::GetSDL_Window() {
	return window;
}
