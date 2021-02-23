#pragma once

#include <string>
#include <vector>
#include <climits>

namespace ct {
	class CTHead {	
		std::int16_t value_min_ = SHRT_MAX;
		std::int16_t value_max_ = SHRT_MIN;
		
		std::vector<std::int16_t> data_;

		static std::size_t coordinates_to_value(std::size_t z, std::size_t y, std::size_t x);
	public:
		static constexpr std::size_t max_z = 113;
		static constexpr std::size_t max_width = 256;
		static constexpr std::size_t max_height = 256;
		
		explicit CTHead(const std::string& file_path);

		std::int16_t get_voxel(std::size_t z, std::size_t y, std::size_t x, bool map = true);
	};
}
