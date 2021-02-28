#pragma once

#include <string>
#include <vector>
#include <climits>

namespace ct {
	/**
	 * Container for CT Head Scan data.
	 *
	 * @note This class is specific to format used for
	 * the original assignment (CS-255).
	 */
	class CTHead {	
		std::int16_t value_min_ = SHRT_MAX;
		std::int16_t value_max_ = SHRT_MIN;
		
		std::vector<std::int16_t> data_;

		static std::size_t coordinates_to_value(std::size_t z, std::size_t y, std::size_t x);
	public:
		static constexpr std::size_t max_z = 113;
		static constexpr std::size_t max_width = 256;
		static constexpr std::size_t max_height = 256;

		/**
		 * Default constructor
		 *
		 * @param file_path path to CT file.
		 */
		explicit CTHead(const std::string& file_path);

		/**
		 * Returns single pixel from 3D object.
		 *
		 * @param z z-value
		 * @param y y-value
		 * @param x x-value
		 * @param map maps value to range 0 - 255 (default true).
		 */
		std::int16_t get_voxel(std::size_t z, std::size_t y, std::size_t x, bool map = true);
	};
}
