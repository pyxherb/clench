#include "keymap.h"

using namespace clench;
using namespace clench::wsi;

std::map<unsigned int, KeyboardKeyCode> wsi::x11KeyMap = {
	{ XK_space, (KeyboardKeyCode)' ' },
	{ XK_exclam, (KeyboardKeyCode)'!' },
	{ XK_quotedbl, (KeyboardKeyCode)'"' },
	{ XK_numbersign, (KeyboardKeyCode)'#' },
	{ XK_dollar, (KeyboardKeyCode)'$' },
	{ XK_percent, (KeyboardKeyCode)'%' },
	{ XK_ampersand, (KeyboardKeyCode)'&' },
	{ XK_quoteright, (KeyboardKeyCode)'\'' },
	{ XK_parenleft, (KeyboardKeyCode)'(' },
	{ XK_parenright, (KeyboardKeyCode)')' },
	{ XK_asterisk, (KeyboardKeyCode)'*' },
	{ XK_plus, (KeyboardKeyCode)'+' },
	{ XK_comma, (KeyboardKeyCode)',' },
	{ XK_minus, (KeyboardKeyCode)'-' },
	{ XK_period, (KeyboardKeyCode)'.' },
	{ XK_slash, (KeyboardKeyCode)'/' },

	{ XK_0, (KeyboardKeyCode)'0' },
	{ XK_1, (KeyboardKeyCode)'1' },
	{ XK_2, (KeyboardKeyCode)'2' },
	{ XK_3, (KeyboardKeyCode)'3' },
	{ XK_4, (KeyboardKeyCode)'4' },
	{ XK_5, (KeyboardKeyCode)'5' },
	{ XK_6, (KeyboardKeyCode)'6' },
	{ XK_7, (KeyboardKeyCode)'7' },
	{ XK_8, (KeyboardKeyCode)'8' },
	{ XK_9, (KeyboardKeyCode)'9' },

	{ XK_colon, (KeyboardKeyCode)':' },
	{ XK_semicolon, (KeyboardKeyCode)';' },
	{ XK_less, (KeyboardKeyCode)'<' },
	{ XK_equal, (KeyboardKeyCode)'=' },
	{ XK_greater, (KeyboardKeyCode)'>' },
	{ XK_question, (KeyboardKeyCode)'?' },
	{ XK_at, (KeyboardKeyCode)'@' },

	{ XK_A, (KeyboardKeyCode)'A' },
	{ XK_B, (KeyboardKeyCode)'B' },
	{ XK_C, (KeyboardKeyCode)'C' },
	{ XK_D, (KeyboardKeyCode)'D' },
	{ XK_E, (KeyboardKeyCode)'E' },
	{ XK_F, (KeyboardKeyCode)'F' },
	{ XK_G, (KeyboardKeyCode)'G' },
	{ XK_H, (KeyboardKeyCode)'H' },
	{ XK_I, (KeyboardKeyCode)'I' },
	{ XK_J, (KeyboardKeyCode)'J' },
	{ XK_K, (KeyboardKeyCode)'K' },
	{ XK_L, (KeyboardKeyCode)'L' },
	{ XK_M, (KeyboardKeyCode)'M' },
	{ XK_N, (KeyboardKeyCode)'N' },
	{ XK_O, (KeyboardKeyCode)'O' },
	{ XK_P, (KeyboardKeyCode)'P' },
	{ XK_Q, (KeyboardKeyCode)'Q' },
	{ XK_R, (KeyboardKeyCode)'R' },
	{ XK_S, (KeyboardKeyCode)'S' },
	{ XK_T, (KeyboardKeyCode)'T' },
	{ XK_U, (KeyboardKeyCode)'U' },
	{ XK_V, (KeyboardKeyCode)'V' },
	{ XK_W, (KeyboardKeyCode)'W' },
	{ XK_X, (KeyboardKeyCode)'X' },
	{ XK_Y, (KeyboardKeyCode)'Y' },
	{ XK_Z, (KeyboardKeyCode)'Z' },

	{ XK_bracketleft, (KeyboardKeyCode)'[' },
	{ XK_backslash, (KeyboardKeyCode)'\\' },
	{ XK_bracketright, (KeyboardKeyCode)']' },
	{ XK_asciicircum, (KeyboardKeyCode)'^' },
	{ XK_underscore, (KeyboardKeyCode)'_' },
	{ XK_quoteleft, (KeyboardKeyCode)'`' },

	{ XK_a, (KeyboardKeyCode)'a' },
	{ XK_b, (KeyboardKeyCode)'b' },
	{ XK_c, (KeyboardKeyCode)'c' },
	{ XK_d, (KeyboardKeyCode)'d' },
	{ XK_e, (KeyboardKeyCode)'e' },
	{ XK_f, (KeyboardKeyCode)'f' },
	{ XK_g, (KeyboardKeyCode)'g' },
	{ XK_h, (KeyboardKeyCode)'h' },
	{ XK_i, (KeyboardKeyCode)'i' },
	{ XK_j, (KeyboardKeyCode)'j' },
	{ XK_k, (KeyboardKeyCode)'k' },
	{ XK_l, (KeyboardKeyCode)'l' },
	{ XK_m, (KeyboardKeyCode)'m' },
	{ XK_n, (KeyboardKeyCode)'n' },
	{ XK_o, (KeyboardKeyCode)'o' },
	{ XK_p, (KeyboardKeyCode)'p' },
	{ XK_q, (KeyboardKeyCode)'q' },
	{ XK_r, (KeyboardKeyCode)'r' },
	{ XK_s, (KeyboardKeyCode)'s' },
	{ XK_t, (KeyboardKeyCode)'t' },
	{ XK_u, (KeyboardKeyCode)'u' },
	{ XK_v, (KeyboardKeyCode)'v' },
	{ XK_w, (KeyboardKeyCode)'w' },
	{ XK_x, (KeyboardKeyCode)'x' },
	{ XK_y, (KeyboardKeyCode)'y' },
	{ XK_z, (KeyboardKeyCode)'z' },

	{ XK_braceleft, (KeyboardKeyCode)'{' },
	{ XK_bar, (KeyboardKeyCode)'|' },
	{ XK_braceright, (KeyboardKeyCode)'}' },
	{ XK_asciitilde, (KeyboardKeyCode)'~' },

	{ XK_BackSpace, KeyboardKeyCode::Backspace },
	{ XK_Tab, KeyboardKeyCode::Tab },
	{ XK_Clear, KeyboardKeyCode::Clear },
	{ XK_Pause, KeyboardKeyCode::Pause },
	{ XK_Escape, KeyboardKeyCode::Esc },
	{ XK_Delete, KeyboardKeyCode::Del },

	{ XK_Up, KeyboardKeyCode::Up },
	{ XK_Down, KeyboardKeyCode::Down },
	{ XK_Left, KeyboardKeyCode::Left },
	{ XK_Right, KeyboardKeyCode::Right },
	{ XK_Insert, KeyboardKeyCode::Insert },
	{ XK_Home, KeyboardKeyCode::Home },
	{ XK_End, KeyboardKeyCode::End },
	{ XK_Page_Up, KeyboardKeyCode::PageUp },
	{ XK_Page_Down, KeyboardKeyCode::PageDown },

	{ XK_KP_0, KeyboardKeyCode::NumPad0 },
	{ XK_KP_1, KeyboardKeyCode::NumPad1 },
	{ XK_KP_2, KeyboardKeyCode::NumPad2 },
	{ XK_KP_3, KeyboardKeyCode::NumPad3 },
	{ XK_KP_4, KeyboardKeyCode::NumPad4 },
	{ XK_KP_5, KeyboardKeyCode::NumPad5 },
	{ XK_KP_6, KeyboardKeyCode::NumPad6 },
	{ XK_KP_7, KeyboardKeyCode::NumPad7 },
	{ XK_KP_8, KeyboardKeyCode::NumPad8 },
	{ XK_KP_9, KeyboardKeyCode::NumPad9 },
	{ XK_KP_Add, KeyboardKeyCode::NumPadAdd },
	{ XK_KP_Subtract, KeyboardKeyCode::NumPadSub },
	{ XK_KP_Multiply, KeyboardKeyCode::NumPadMul },
	{ XK_KP_Divide, KeyboardKeyCode::NumPadDiv },
	{ XK_KP_Prior, KeyboardKeyCode::NumPadDot },
	{ XK_KP_Enter, KeyboardKeyCode::NumPadEnter },
	{ XK_KP_Equal, KeyboardKeyCode::NumPadEq },

	{ XK_F1, KeyboardKeyCode::F1 },
	{ XK_F2, KeyboardKeyCode::F2 },
	{ XK_F3, KeyboardKeyCode::F3 },
	{ XK_F4, KeyboardKeyCode::F4 },
	{ XK_F5, KeyboardKeyCode::F5 },
	{ XK_F6, KeyboardKeyCode::F6 },
	{ XK_F7, KeyboardKeyCode::F7 },
	{ XK_F8, KeyboardKeyCode::F8 },
	{ XK_F9, KeyboardKeyCode::F9 },
	{ XK_F10, KeyboardKeyCode::F10 },
	{ XK_F11, KeyboardKeyCode::F11 },
	{ XK_F12, KeyboardKeyCode::F12 },

	{ XK_Num_Lock, KeyboardKeyCode::NumLock },
	{ XK_Caps_Lock, KeyboardKeyCode::CapsLock },
	{ XK_Scroll_Lock, KeyboardKeyCode::ScrollLock },
	{ XK_Shift_L, KeyboardKeyCode::LShift },
	{ XK_Shift_R, KeyboardKeyCode::RShift },
	{ XK_Control_L, KeyboardKeyCode::LCtrl },
	{ XK_Control_R, KeyboardKeyCode::RCtrl },
	{ XK_Alt_L, KeyboardKeyCode::LAlt },
	{ XK_Alt_R, KeyboardKeyCode::RAlt },
	{ XK_Meta_L, KeyboardKeyCode::LMeta },
	{ XK_Meta_R, KeyboardKeyCode::RMeta },
	{ XK_Super_L, KeyboardKeyCode::LSuper },
	{ XK_Super_R, KeyboardKeyCode::RSuper },
	{ XK_Mode_switch, KeyboardKeyCode::AltGr }
};
