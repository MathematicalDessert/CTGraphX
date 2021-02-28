#include <imgui.h>
#include <SDL2/SDL_opengl.h>
#ifdef _WIN32
#include <ppl.h>
#endif

#include "window/ct_head_viewer.h"

#include <iostream>

#include "state/ct_head_state.h"

namespace window {
	std::array<double, 4> CTHeadViewer::transfer_function(const std::int16_t value) const {
		if (value < -300) {
			return { 0, 0, 0, 0 };
		}

		if (value >= -300 && value <= 49) {
			return { 1, 0.79, 0.6, transparency_ / 100.0 };
		}

		if (value >= 50 && value <= 299) {
			return { 0, 0, 0, 0 };
		}

		return { 1, 1, 1, 0.8 };
	}

	void CTHeadViewer::update_all() {
		update_top_view();
		update_front_view();
		update_side_view();
	}
	
	void CTHeadViewer::update_top_view() {
		update_view(textures_[0], 256, 256, 113, false, false);
	}

	void CTHeadViewer::update_front_view() {
		update_view(textures_[1], 256, 113, 256, true, false);
	}

	void CTHeadViewer::update_side_view() {
		update_view(textures_[2], 256, 113, 256, false, true);
	}

	void CTHeadViewer::update_view(GLuint texture_id, const int width, const int height, const int depth, const bool back, const bool side) {
		glBindTexture(GL_TEXTURE_2D, texture_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		const auto data = std::make_unique<GLuint[]>(width * height);

		switch (static_cast<RenderStyle>(render_style_)) {
		case RenderStyle::slice: {
#ifdef _WIN32
			Concurrency::parallel_for(0, width, [&](const int j) {
				Concurrency::parallel_for(0, height, [&](const int k) {
#else
			for (auto j = 0; j < width; j++) {
				for (auto k = 0; k < height; k++) {
#endif
					std::int16_t z, x, y;

					if (back) {
						z = k;
						y = front_back_slider_;
						x = j;
					}
					else if (side) {
						z = k;
						y = j;
						x = side_slider_;
					}
					else {
						z = top_down_slider_;
						y = k;
						x = j;
					}

					const auto voxel = ct_head_state_->source->get_voxel(z, y, x);
					data[k * width + j] = 255 << 24 | voxel << 16 | voxel << 8 | voxel;
#ifdef _WIN32
				});
			});
#else
				}
			}
#endif
			break;
		}
		case RenderStyle::volume: {
#ifdef _WIN32
			Concurrency::parallel_for(0, width, [&](int j) {
				Concurrency::parallel_for(0, height, [&](int k) {
#else
			for (auto j = 0; j < width; j++) {
				for (auto k = 0; k < height; k++) {
#endif
					const auto light = lighting_ / 100.0;
					std::array<double, 4> voxel{ 0, 0, 0, light };

					// loop for data
					for (auto slice = 0; slice < depth; slice++) {
						std::int16_t z, x, y;

						if (back) {
							z = k;
							y = slice;
							x = j;
						}
						else if (side) {
							z = k;
							y = j;
							x = slice;
						}
						else {
							z = slice;
							y = k;
							x = j;
						}

						const auto source_voxel = ct_head_state_->source->get_voxel(z, y, x, false);

						auto transformed_value = transfer_function(source_voxel);
						const auto transparency = voxel[3];
						const auto transformed_transparency = transformed_value[3];

						
						voxel[0] += transparency * transformed_transparency * light * transformed_value[0];
						voxel[1] += transparency * transformed_transparency * light * transformed_value[1];
						voxel[2] += transparency * transformed_transparency * light * transformed_value[2];
						voxel[3] *= light - transformed_transparency;
					}

					data[k * width + j] =
						255 << 24 |
						static_cast<int>(voxel[2] * 255) << 16 |
						static_cast<int>(voxel[1] * 255) << 8 |
						static_cast<int>(voxel[0] * 255);
#ifdef _WIN32
				});
			});
#else
				}
			}
#endif
			break;
		}
		case RenderStyle::raycast: {
			// Raycasted model.

			
		}
		default: break;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.get());
	}
	
	CTHeadViewer::CTHeadViewer(state::CTHeadState& ct_head_state)
		: ct_head_state_(&ct_head_state) {
		textures_ = new GLuint[3];
		glGenTextures(3, textures_);

		update_all();
	}

	CTHeadViewer::~CTHeadViewer() {		
		glDeleteTextures(3, textures_);
		delete textures_;
	}

	void CTHeadViewer::render() {
		ImGui::Begin("CT Head", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

		if (ImGui::RadioButton("Slice Render", &render_style_, static_cast<int>(RenderStyle::slice))) {
			update_all();
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Volume Render", &render_style_, static_cast<int>(RenderStyle::volume))) {
			update_all();
		}
		
		ImGui::Image(reinterpret_cast<GLuint*>(textures_[0]), ImVec2(ct::CTHead::max_width, ct::CTHead::max_height));
		ImGui::SameLine();
		ImGui::Image(reinterpret_cast<GLuint*>(textures_[1]), ImVec2(ct::CTHead::max_width, ct::CTHead::max_z));
		ImGui::SameLine();
		ImGui::Image(reinterpret_cast<GLuint*>(textures_[2]), ImVec2(ct::CTHead::max_height, ct::CTHead::max_z));

		if (render_style_ == static_cast<int>(RenderStyle::slice)) {
			if (ImGui::SliderInt("Top", &top_down_slider_, 0, ct::CTHead::max_z - 1)) update_top_view();
			ImGui::SameLine();
			if (ImGui::SliderInt("Front", &front_back_slider_, 0, ct::CTHead::max_width - 1)) update_front_view();
			ImGui::SameLine();
			if (ImGui::SliderInt("Side", &side_slider_, 0, ct::CTHead::max_height - 1)) update_side_view();
		} else if (render_style_ == static_cast<int>(RenderStyle::volume)) {
			if (ImGui::SliderInt("Skin Opacity", &transparency_, 0, 100)) {
				update_all();
			}

			if (ImGui::SliderInt("Source Light Brightness", &lighting_, 0, 100)) {
				update_all();
			}
		}
		ImGui::End();
	}
}
