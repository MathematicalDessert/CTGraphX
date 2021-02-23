#pragma once

#include "window.h"

namespace window {
	class DebugPanelWindow final : public Window {
	public:
		explicit DebugPanelWindow();

		void render() override;
	};
}
