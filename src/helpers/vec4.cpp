#include "helpers/vec4.h"

namespace helpers {
	Vector4::Vector4()
		: f_intrin_(_mm_setzero_ps()) {}
	
	Vector4::Vector4(const __m128 v)
		: f_intrin_(v) {}
	
	Vector4::Vector4(const float v1, const float v2, const float v3, const float v4)
		: f_intrin_(_mm_set_ps(v1, v2, v3, v4)) { }
}
