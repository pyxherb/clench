#ifndef _CLENCH_WSAL_KEYBOARD_H_
#define _CLENCH_WSAL_KEYBOARD_H_

#include <cstdint>

#ifdef None
#define _None None
#undef None
#endif

namespace clench {
	namespace wsal {
		enum class KeyboardKeyCode : uint32_t {
			None = 0,

			// ASCII control characters
			Backspace = 8,	// \b
			Tab = 9,		// \t
			Clear = 12,		// \f
			Return = 13,	// \r
			Pause = 19,		// Pause
			Esc = 27,		// Esc
			Del = 127,		// Delete

			NumPad0 = 256,	// 0 on numpad
			NumPad1,		// 1 on numpad
			NumPad2,		// 2 on numpad
			NumPad3,		// 3 on numpad
			NumPad4,		// 4 on numpad
			NumPad5,		// 5 on numpad
			NumPad6,		// 6 on numpad
			NumPad7,		// 7 on numpad
			NumPad8,		// 8 on numpad
			NumPad9,		// 9 on numpad
			NumPadAdd,		// `+` on numpad
			NumPadSub,		// `-` on numpad
			NumPadMul,		// `*` on numpad
			NumPadDiv,		// `/` on numpad
			NumPadDot,		// `.` on numpad
			NumPadEnter,	// Enter on numpad
			NumPadEq,		// `=` on numpad

			Up,
			Down,
			Right,
			Left,
			Insert,
			Home,
			End,
			PageUp,
			PageDown,
			Select,
			Print,
			Execute,
			PrintScreen,
			Help,

			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
			F13,
			F14,
			F15,
			F16,
			F17,
			F18,
			F19,
			F20,

			NumLock,
			CapsLock,
			ScrollLock,
			RShift,
			LShift,
			RCtrl,
			LCtrl,
			RAlt,
			LAlt,
			RMeta,
			LMeta,
			LSuper,
			RSuper,
			Menu,
			AltGr,
			Apps
		};
	}
}

#ifdef _None
#define None _None
#undef _None
#endif

#endif
