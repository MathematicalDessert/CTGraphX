#pragma once

#include <vector>
#include <memory>
#include <SDL2/SDL_video.h>

#include "window/window.h"

/**
 * Main application class.
 */
class Application {
	// running boolean
	bool running_ = true;

	// SDL window instance
	SDL_Window* window_ = nullptr;

	// OpenGL context
	SDL_GLContext gl_context_ = nullptr;

	// container of window objects
	std::vector<std::unique_ptr<window::Window>> windows_;
public:
	/**
	 * Initializes the application.
	 *
	 * @returns false if initialization fails, otherwise true.
	 */
	bool initialize();

	/**
	 * De-initializes the application.
	 *
	 * @returns false if de-initialization fails, otherwise true.
	 */
	bool deinitialize();

	/**
	 * Runs the application.
	 *
	 * @returns true if run was successful, otherwise false.
	 */
	bool run();
};
