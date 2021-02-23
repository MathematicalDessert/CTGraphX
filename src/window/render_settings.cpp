#include <imgui.h>

#include "window/render_settings.h"

namespace window {
	RenderSettingsWindow::RenderSettingsWindow(state::RenderSettingsState& render_settings_state)
		: render_settings_state_(&render_settings_state) {
		
	}

	void RenderSettingsWindow::render() {
		ImGui::Begin("Render Settings");

		ImGui::Checkbox("Test", &render_settings_state_->test);
		
		ImGui::End();
	}

}