#ifndef _SLAKE_UTIL_SCOPE_GUARD_H_
#define _SLAKE_UTIL_SCOPE_GUARD_H_

#include <functional>

namespace clench {
	namespace utils {
		using ScopeGuardCallback = std::function<void()>;

		struct ScopeGuard {
			ScopeGuardCallback callback;

			ScopeGuard() = default;
			FORCEINLINE ScopeGuard(ScopeGuardCallback &&callback) : callback(callback) {}
			FORCEINLINE ~ScopeGuard() {
				if (callback)
					callback();
			}

			FORCEINLINE void release() {
				callback = {};
			}

			FORCEINLINE ScopeGuard& operator=(ScopeGuardCallback&& callback) {
				this->callback = callback;
				return *this;
			}
		};
	}
}

#endif
