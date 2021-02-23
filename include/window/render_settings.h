#pragma once

#include "window.h"
#include "state/render_settings_state.h"

namespace window {
	class RenderSettingsWindow final : public Window {
		state::RenderSettingsState* render_settings_state_;
	public:
		explicit RenderSettingsWindow(state::RenderSettingsState& render_settings_state);
		
		void render() override;
	};
}