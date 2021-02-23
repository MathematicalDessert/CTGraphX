#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>
#include <SDL2/SDL_opengl.h>

#include "application.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"
#include "state/ct_head_state.h"
#include "state/render_settings_state.h"
#include "window/ct_head_viewer.h"
#include "window/debug_panel.h"
#include "window/render_settings.h"

bool Application::initialize() {
	spdlog::info("Initializing application");
	
	// initialize SDL
	spdlog::info("Initializing SDL");
	if (SDL_Init(SDL_INIT_VIDEO)) {
		spdlog::error("Failed to initialize SDL2. Reason: {}", SDL_GetError());
		return false;
	}

	// create SDL window
	spdlog::info("Creating Window");
	window_ = SDL_CreateWindow(
		"CTGraphX",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		720,
		640,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
	);

	SDL_SetWindowMinimumSize(window_, 720, 640);

	// initialize OpenGL
	spdlog::info("Creating OpenGL Context");
	gl_context_ = SDL_GL_CreateContext(window_);
	SDL_GL_MakeCurrent(window_, gl_context_);
	SDL_GL_SetSwapInterval(1); // vsync

	// initialize IMGUI
	spdlog::info("Creating IMGUI Context");
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	auto io = &ImGui::GetIO();
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backend
	spdlog::info("Initialize SDL and OpenGL bindings");
	ImGui_ImplSDL2_InitForOpenGL(window_, gl_context_);
	ImGui_ImplOpenGL2_Init();
	
	return true;
}

bool Application::deinitialize() {
	spdlog::info("De-initializing application");

	spdlog::info("Shutting down IMGUI...");
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	spdlog::info("Shutting down OpenGL...");
	SDL_GL_DeleteContext(gl_context_);

	spdlog::info("Shutting down Window...");
	SDL_DestroyWindow(window_);
	return false;
}


bool Application::run() {
	spdlog::info("Creating states and windows...");

	// TODO: create states and windows
	state::RenderSettingsState render_settings_state;
	state::CTHeadState ct_head_state;

	ct_head_state.source = std::make_unique<ct::CTHead>("../../../data/CThead");
	

	windows_.push_back(std::make_unique<window::RenderSettingsWindow>(render_settings_state));
	windows_.push_back(std::make_unique<window::DebugPanelWindow>());
	windows_.push_back(std::make_unique<window::CTHeadViewer>(ct_head_state));

	spdlog::info("Running application...");
	
	while (running_) {
		SDL_Event event;

		// poll events
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT ||
				(event.type == SDL_WINDOWEVENT &&
					event.window.event == SDL_WINDOWEVENT_CLOSE &&
					event.window.windowID == SDL_GetWindowID(window_))) {
				running_ = false; // kills the application
			}
		}

		// IMGUI frame
		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplSDL2_NewFrame(window_);
		ImGui::NewFrame();

		for (const auto& window : windows_) {
			window->render();
		}

		const auto io = &ImGui::GetIO();

		ImGui::Render();
		glViewport(0, 0, static_cast<GLsizei>(io->DisplaySize.x), static_cast<GLsizei>(io->DisplaySize.y));
		glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window_);
	}

	spdlog::info("Application closing...");	
	return true; // ran successfully
}
