#include <clench/plat/uuid.h>
#include <ctime>
#include <memory>
#include <WinSock2.h>
#include <iphlpapi.h>

using namespace clench;
using namespace clench::plat;

static uint64_t _g_lastGenTime = 0,
				_g_timeSequence = 0;

static uint64_t _fetchSystemTime() {
	uint64_t t = 0;

	FILETIME ft;

	GetSystemTimeAsFileTime(&ft);

	t = ft.dwLowDateTime;
	t |= ((uint64_t)ft.dwHighDateTime) << 32;

	// Convert to microseconds.
	t /= 10;

	// Convert to UNIX epoch time from file time.
	t -= 11644473600000000ULL;

	return t;
}

static uint64_t _fetchSystemNode() {
	ULONG outBufLen = sizeof(IP_ADAPTER_ADDRESSES);
	std::unique_ptr<IP_ADAPTER_ADDRESSES> pAddress((IP_ADAPTER_ADDRESSES*)new char[outBufLen]);

	if (GetAdaptersAddresses(AF_UNSPEC, 0, nullptr, pAddress.get(), &outBufLen)) {
		pAddress.reset();
		pAddress = std::unique_ptr<IP_ADAPTER_ADDRESSES>((IP_ADAPTER_ADDRESSES *) new char[outBufLen]);
	}

	if (GetAdaptersAddresses(AF_UNSPEC, 0, nullptr, pAddress.get(), &outBufLen) == NO_ERROR) {
		for (PIP_ADAPTER_ADDRESSES i = pAddress.get();; i = i->Next) {
			if (i->PhysicalAddressLength != 6)
				continue;

			uint64_t result = 0;
			for (uint_fast8_t j = 0; j < 6; ++j) {
				result *= 256;
				result |= i->PhysicalAddress[j];
			}

			return result;
		}
	}

	return UINT64_MAX;
}

CLCPLAT_API void clench::plat::generateUUID(utils::UUID &uuidOut) {
	uint64_t t = _fetchSystemTime();

	if (t <= _g_lastGenTime) {
		++_g_timeSequence;
	}

	_g_lastGenTime = t;

	uuidOut.a = t & 0xffffffff;
	uuidOut.b = (uint16_t)(t >> 32);
	uuidOut.c = 0b0001 | ((t >> 44) & 0xfff0);
	uuidOut.d = 0b10 | (_g_timeSequence << 2);
	uuidOut.e = _fetchSystemNode();
}
