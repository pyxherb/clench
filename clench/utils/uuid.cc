#include "uuid.h"
#include "assert.h"
#include <clench/base/attribs.h>
#include <random>
#include <ctime>

using namespace clench;
using namespace clench::utils;

FORCEINLINE uint_fast8_t _parseSingleDigit(char c) {
	if (c >= '0' && c <= '9') {
		return c - '0';
	} else if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	} else if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	}

	throw UINT_FAST8_MAX;
}

CLCUTILS_API bool UUID::from(const char *s, UUID &uuidOut) {
	CLENCH_ASSERT(strlen(s) >= 36, "Input string is too short");

	if (s[8] != '-' ||
		(s[13] != '-') ||
		(s[18] != '-') ||
		(s[23] != '-'))
		return false;

	memset(&uuidOut, 0, sizeof(UUID));

	for (size_t i = 0; i < 8; ++i) {
		uuidOut.a *= 16;

		uint_fast8_t curDigit = _parseSingleDigit(s[i]);
		if (curDigit == UINT_FAST8_MAX)
			return false;

		uuidOut.a += curDigit;
	}

	for (size_t i = 9; i < 13; ++i) {
		uuidOut.b *= 16;

		uint_fast8_t curDigit = _parseSingleDigit(s[i]);
		if (curDigit == UINT_FAST8_MAX)
			return false;

		uuidOut.b += curDigit;
	}

	for (size_t i = 14; i < 18; ++i) {
		uuidOut.c *= 16;
		uuidOut.c += _parseSingleDigit(s[i]);

		uint_fast8_t curDigit = _parseSingleDigit(s[i]);
		if (curDigit == UINT_FAST8_MAX)
			return false;

		uuidOut.c += curDigit;
	}

	for (size_t i = 19; i < 23; ++i) {
		uuidOut.d *= 16;
		uuidOut.d += _parseSingleDigit(s[i]);

		uint_fast8_t curDigit = _parseSingleDigit(s[i]);
		if (curDigit == UINT_FAST8_MAX)
			return false;

		uuidOut.d += curDigit;
	}

	for (size_t i = 24; i < 36; ++i) {
		uuidOut.e *= 16;
		uuidOut.e += _parseSingleDigit(s[i]);

		uint_fast8_t curDigit = _parseSingleDigit(s[i]);
		if (curDigit == UINT_FAST8_MAX)
			return false;

		uuidOut.e += curDigit;
	}

	return true;
}

FORCEINLINE char _hexDigitToChar(uint_fast8_t digit) {
	return digit > 9 ? digit - 10 + 'a' : digit + '0';
}

std::string std::to_string(const clench::utils::UUID &uuid) {
	char s[37];

	s[0] = _hexDigitToChar((uuid.a >> 28) & 0xf);
	s[1] = _hexDigitToChar((uuid.a >> 24) & 0xf);
	s[2] = _hexDigitToChar((uuid.a >> 20) & 0xf);
	s[3] = _hexDigitToChar((uuid.a >> 16) & 0xf);
	s[4] = _hexDigitToChar((uuid.a >> 12) & 0xf);
	s[5] = _hexDigitToChar((uuid.a >> 8) & 0xf);
	s[6] = _hexDigitToChar((uuid.a >> 4) & 0xf);
	s[7] = _hexDigitToChar(uuid.a & 0xf);

	s[8] = '-';

	s[9] = _hexDigitToChar((uuid.b >> 12) & 0xf);
	s[10] = _hexDigitToChar((uuid.b >> 8) & 0xf);
	s[11] = _hexDigitToChar((uuid.b >> 4) & 0xf);
	s[12] = _hexDigitToChar(uuid.b & 0xf);

	s[13] = '-';

	s[14] = _hexDigitToChar((uuid.c >> 12) & 0xf);
	s[15] = _hexDigitToChar((uuid.c >> 8) & 0xf);
	s[16] = _hexDigitToChar((uuid.c >> 4) & 0xf);
	s[17] = _hexDigitToChar(uuid.c & 0xf);

	s[18] = '-';

	s[19] = _hexDigitToChar((uuid.d >> 12) & 0xf);
	s[20] = _hexDigitToChar((uuid.d >> 8) & 0xf);
	s[21] = _hexDigitToChar((uuid.d >> 4) & 0xf);
	s[22] = _hexDigitToChar(uuid.d & 0xf);

	s[23] = '-';

	s[24] = _hexDigitToChar((uuid.e >> 44) & 0xf);
	s[25] = _hexDigitToChar((uuid.e >> 40) & 0xf);
	s[26] = _hexDigitToChar((uuid.e >> 36) & 0xf);
	s[27] = _hexDigitToChar((uuid.e >> 32) & 0xf);
	s[28] = _hexDigitToChar((uuid.e >> 28) & 0xf);
	s[29] = _hexDigitToChar((uuid.e >> 24) & 0xf);
	s[30] = _hexDigitToChar((uuid.e >> 20) & 0xf);
	s[31] = _hexDigitToChar((uuid.e >> 16) & 0xf);
	s[32] = _hexDigitToChar((uuid.e >> 12) & 0xf);
	s[33] = _hexDigitToChar((uuid.e >> 8) & 0xf);
	s[34] = _hexDigitToChar((uuid.e >> 4) & 0xf);
	s[35] = _hexDigitToChar(uuid.e & 0xf);

	s[36] = '\0';

	return s;
}

CLCUTILS_API void clench::utils::generateRandomUUID(UUID &uuidOut) {
	memset(&uuidOut, 0, sizeof(UUID));

	std::mt19937_64 g_uuidRng(time(nullptr));

	uint64_t a = g_uuidRng(),
			 b = g_uuidRng();

	uuidOut.a = (uint32_t)a;
	uuidOut.b = (uint16_t)(a >> 32);
	uuidOut.c = 0b0100 | ((b & 0xfff) << 4);
	uuidOut.d = 0b10 | (uint16_t)(b << 2);
	uuidOut.e = b >> 14;
}
