#include <fstream>

#include "ct/ct_head.h"

namespace {
	short map_range(const std::int16_t value,
		const std::int16_t input_min,
		const std::int16_t input_max,
		const std::int16_t output_min,
		const std::int16_t output_max) {
		const auto intermediate = (value - input_min) / static_cast<double>(input_max - input_min);
		return static_cast<short>(output_max * intermediate + output_min * (1.0 - intermediate));
	}
}

namespace ct {
	std::size_t CTHead::coordinates_to_value(const std::size_t z, const std::size_t y, const std::size_t x) {
		return z * max_width * max_height + y * max_height + x;
	}
	
	CTHead::CTHead(const std::string& file_path) {
		// initialize data vector with zeros
		data_ = std::vector<std::int16_t>(max_z * max_width * max_height, 0);

		// open file
		std::ifstream source_stream(file_path, std::ios::binary | std::ios::in);

		if (!source_stream) {
			throw std::runtime_error("file not found!");
		}

		for (std::size_t pos = 0; pos < max_z * max_width * max_height; pos++) {
			std::int16_t pixel = 0;
			source_stream.read(reinterpret_cast<char*>(&pixel), sizeof(std::int16_t));

			/*
			auto hibyte = (pixel & 0xff00) >> 8;
			auto lowbyte = (pixel & 0xff);
			pixel = lowbyte << 8 | hibyte;
			*/
			
			if (pixel < value_min_) value_min_ = pixel;
			if (pixel > value_max_) value_max_ = pixel;

			data_[pos] = pixel;
		}

		// TODO: If necessary we could have a better way of doing this
		// such as checking some header in the file.
		if (value_min_ != -1117 || value_max_ != 2248) {
			//throw std::runtime_error("File is incorrectly formatted!");
		}
	}

	std::int16_t CTHead::get_voxel(const std::size_t z, const std::size_t y, const std::size_t x, const bool map) {
		const auto value = data_[coordinates_to_value(z, y, x)];

		if (map) {
			return map_range(value, value_min_, value_max_, 0, 255);
		}
		return value;
	}

}