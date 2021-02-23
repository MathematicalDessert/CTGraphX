#pragma once

namespace window {
	/**
	 * IMGUI window wrapper.
	 */
	class Window {
	public:
		virtual ~Window() = default;
		virtual void render() = 0;
	};
}