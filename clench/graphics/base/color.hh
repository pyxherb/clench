#ifndef _CLENCH_GRAPHICS_COLOR_HH_
#define _CLENCH_GRAPHICS_COLOR_HH_

#include <cstdint>

namespace clench {
	namespace graphics {
		struct ColorI;
		struct ColorF;

		struct ColorI final {
			std::uint8_t r, g, b, a;
			inline ColorI() noexcept {
				r = 0, g = 0, b = 0, a = 0;
			}
			inline ColorI(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) noexcept {
				this->r = r, this->g = g, this->b = b, this->a = a;
			}
			inline ColorI& operator=(ColorI& x) noexcept;
			inline bool operator==(ColorI& x) noexcept;
			inline ColorI operator+(ColorI& x) noexcept;
			inline ColorI operator-(ColorI& x) noexcept;
			inline ColorI& operator+=(ColorI& x) noexcept;
			inline ColorI& operator-=(ColorI& x) noexcept;
			inline operator ColorF() noexcept;
		};

		struct ColorF final {
			float r, g, b, a;
			ColorF() noexcept {
				r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;
			}
			ColorF(float r, float g, float b, float a) noexcept {
				this->r = r, this->g = g, this->b = b, this->a = a;
			}
			inline ColorF& operator=(ColorF& x) noexcept;
			inline bool operator==(ColorF& x) noexcept;
			inline ColorF operator+(ColorF& x) noexcept;
			inline ColorF operator-(ColorF& x) noexcept;
			inline ColorF& operator+=(ColorF& x) noexcept;
			inline ColorF& operator-=(ColorF& x) noexcept;
			inline operator ColorI() noexcept;
		};

		inline ColorI& ColorI::operator=(ColorI& x) noexcept {
			r = x.r, g = x.g, b = x.b, a = x.a;
			return *this;
		}
		inline bool ColorI::operator==(ColorI& x) noexcept {
			return (this->r == x.r) && (this->g == x.g) && (this->b == x.b) && (this->a == x.a);
		}
		inline ColorI ColorI::operator+(ColorI& x) noexcept {
			return ColorI(r + x.r, g + x.g, b + x.b, a + x.a);
		}
		inline ColorI ColorI::operator-(ColorI& x) noexcept {
			return ColorI(r - x.r, g - x.g, b - x.b, a - x.a);
		}
		inline ColorI& ColorI::operator+=(ColorI& x) noexcept {
			r += x.r, g += x.g, b += x.b, a += x.a;
			return *this;
		}
		inline ColorI& ColorI::operator-=(ColorI& x) noexcept {
			r -= x.r, g -= x.g, b -= x.b, a -= x.a;
			return *this;
		}
		inline ColorI::operator ColorF() noexcept {
			return ColorF(
				(float)r / 255.0f,
				(float)g / 255.0f,
				(float)b / 255.0f,
				(float)a / 255.0f);
		}

		inline ColorF& ColorF::operator=(ColorF& x) noexcept {
			r = x.r, g = x.g, b = x.b, a = x.a;
			return *this;
		}
		inline bool ColorF::operator==(ColorF& x) noexcept {
			return (this->r == x.r) && (this->g == x.g) && (this->b == x.b) && (this->a == x.a);
		}
		inline ColorF ColorF::operator+(ColorF& x) noexcept {
			return ColorF(r + x.r, g + x.g, b + x.b, a + x.a);
		}
		inline ColorF ColorF::operator-(ColorF& x) noexcept {
			return ColorF(r - x.r, g - x.g, b - x.b, a - x.a);
		}
		inline ColorF& ColorF::operator+=(ColorF& x) noexcept {
			r += x.r, g += x.g, b += x.b, a += x.a;
			return *this;
		}
		inline ColorF& ColorF::operator-=(ColorF& x) noexcept {
			r -= x.r, g -= x.g, b -= x.b, a -= x.a;
			return *this;
		}
		inline ColorF::operator ColorI() noexcept {
			return ColorI(
				(std::uint8_t)(r * 255.0f),
				(std::uint8_t)(g * 255.0f),
				(std::uint8_t)(b * 255.0f),
				(std::uint8_t)(a * 255.0f));
		}
	}
}
#endif
