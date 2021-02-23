#include "window/debug_panel.h"

#include "imgui.h"

namespace window {
	DebugPanelWindow::DebugPanelWindow() {
		
	}

	void DebugPanelWindow::render() {
		ImGui::Begin("Debug Panel", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

		const auto io = ImGui::GetIO();
		ImGui::Text("FPS: %.1f (%.3f ms/frame)", io.Framerate, 1000.0f / io.Framerate);
		
		ImGui::End();
	}
}
