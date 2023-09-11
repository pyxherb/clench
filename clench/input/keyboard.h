#ifndef _CLENCH_INPUT_KEYBOARD_H_
#define _CLENCH_INPUT_KEYBOARD_H_

#include <cstdint>

namespace clench {
	namespace input {
		/// @brief Key codes for keyboard inputs, keys that represent ASCII printable characters are not included.
		enum KeyboardKeyCode : uint32_t {
			// ASCII control characters
			KBKEY_BACKSPACE = 8, // \b
			KBKEY_TAB = 9, // \t
			KBKEY_CLEAR = 12, // \f
			KBKEY_RETURN =13, // \r
			KBKEY_PAUSE = 19, // Pause
			KBKEY_ESC = 27, // Esc
			KBKEY_DEL = 127, // Delete

			KBKEY_NUMPAD0 = 256, // 0 on numpad
			KBKEY_NUMPAD1, // 1 on numpad
			KBKEY_NUMPAD2, // 2 on numpad
			KBKEY_NUMPAD3, // 3 on numpad
			KBKEY_NUMPAD4, // 4 on numpad
			KBKEY_NUMPAD5, // 5 on numpad
			KBKEY_NUMPAD6, // 6 on numpad
			KBKEY_NUMPAD7, // 7 on numpad
			KBKEY_NUMPAD8, // 8 on numpad
			KBKEY_NUMPAD9, // 9 on numpad
			KBKEY_NUMPAD_ADD, // `+` on numpad
			KBKEY_NUMPAD_SUB, // `-` on numpad
			KBKEY_NUMPAD_MUL, // `*` on numpad
			KBKEY_NUMPAD_DIV, // `/` on numpad
			KBKEY_NUMPAD_DOT, // `.` on numpad
			KBKEY_NUMPAD_ENTER, // Enter on numpad
			KBKEY_NUMPAD_EQ, // `=` on numpad

			KBKEY_UP,
			KBKEY_DOWN,
			KBKEY_RIGHT,
			KBKEY_LEFT,
			KBKEY_INSERT,
			KBKEY_HOME,
			KBKEY_END,
			KBKEY_PAGEUP,
			KBKEY_PAGEDOWN,

			KBKEY_F1,
			KBKEY_F2,
			KBKEY_F3,
			KBKEY_F4,
			KBKEY_F5,
			KBKEY_F6,
			KBKEY_F7,
			KBKEY_F8,
			KBKEY_F9,
			KBKEY_F10,
			KBKEY_F11,
			KBKEY_F12,

			KBKEY_NUM_LOCK,
			KBKEY_CAPS_LOCK,
			KBKEY_SCROLL_LOCK,
			KBKEY_RSHIFT,
			KBKEY_LSHIFT,
			KBKEY_RCTRL,
			KBKEY_LCTRL,
			KBKEY_RALT,
			KBKEY_LALT,
			KBKEY_RMETA,
			KBKEY_LMETA,
			KBKEY_LSUPER,
			KBKEY_RSUPER,
			KBKEY_ALTGR
		};
	}
}

#endif
