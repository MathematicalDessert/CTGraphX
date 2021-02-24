#pragma once

// SSE helpers
#include <nmmintrin.h>
#include <immintrin.h>

namespace helpers {
	class Vector4 {
		union {
			__m128 f_intrin_;
			float f_[4];
		};
	public:
		Vector4();
		Vector4(__m128 v);
		Vector4(float v1, float v2, float v3, float v4);

		Vector4 operator*(const Vector4& rhs) {
			return Vector4(_mm_mul_ps(f_intrin_, rhs.f_intrin_));
		}

		Vector4 operator*=(const Vector4& rhs) {
			f_intrin_ = _mm_mul_ps(f_intrin_, rhs.f_intrin_);
			return *this;
		}
		
		Vector4 operator+=(const Vector4& rhs) {
			f_intrin_ = _mm_add_ps(f_intrin_, rhs.f_intrin_);
			return *this;
		}

		Vector4 operator-=(const Vector4& rhs) {
			f_intrin_ = _mm_sub_ps(f_intrin_, rhs.f_intrin_);
			return *this;
		}

		float operator[](const int i) const {
			return f_[i];
		}

		float& operator[](const int i) {
			return f_[i];
		}
	};

	typedef Vector4 Vec4;
}