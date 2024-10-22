#pragma once

#include <array>
#include "window.h"
#include "state/ct_head_state.h"

namespace window {	
	class CTHeadViewer final : public Window {
		enum class RenderStyle {
			slice,
			volume,
			raycast,
		};
		
		state::CTHeadState* ct_head_state_;
		GLuint* textures_;
		
		int top_down_slider_ = 76;
		int front_back_slider_ = 76;
		int side_slider_ = 76;

		int lighting_ = 99;
		int transparency_= 12;

		int render_style_ = static_cast<int>(RenderStyle::slice);

		[[nodiscard]] std::array<double, 4> transfer_function(std::int16_t value) const;

		void update_all();
		void update_top_view();
		void update_front_view();
		void update_side_view();
		void update_view(GLuint texture_id, int width, int height, int depth, bool back = false, bool side = false);
	public:
		explicit CTHeadViewer(state::CTHeadState& ct_head_state);
		~CTHeadViewer();

		void render() override;
	};
}
