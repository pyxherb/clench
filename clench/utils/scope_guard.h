#ifndef _SLAKE_UTIL_SCOPE_GUARD_H_
#define _SLAKE_UTIL_SCOPE_GUARD_H_

#include <functional>

namespace clench {
	namespace utils {
		using ScopeGuardCallback = std::function<void()>;

		struct ScopeGuard {
			ScopeGuardCallback callback;

			ScopeGuard() = default;
			CLENCH_FORCEINLINE ScopeGuard(ScopeGuardCallback &&callback) : callback(callback) {}
			CLENCH_FORCEINLINE ~ScopeGuard() {
				if (callback)
					callback();
			}

			CLENCH_FORCEINLINE void release() {
				callback = {};
			}

			CLENCH_FORCEINLINE ScopeGuard& operator=(ScopeGuardCallback&& callback) {
				this->callback = callback;
				return *this;
			}
		};
	}
}

#endif
