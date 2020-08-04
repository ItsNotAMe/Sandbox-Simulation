#include "Input.h"

#include <windows.h>

ButtonState Input::m_KeysStates[(uint16_t)KEY_MENU];
ButtonState Input::m_MouseStates[(uint16_t)MOUSE_BUTTON_MIDDLE];
int Input::m_MouseX = 0, Input::m_MouseY = 0;

bool Input::IsDown(KeyCode key)
{
	return m_KeysStates[(uint16_t)key].IsDown;
}

bool Input::IsPressed(KeyCode key)
{
	return m_KeysStates[(uint16_t)key].IsDown && m_KeysStates[(uint16_t)key].Changed;
}

bool Input::IsUnpressed(KeyCode key)
{
	return !m_KeysStates[(uint16_t)key].IsDown && m_KeysStates[(uint16_t)key].Changed;
}

bool Input::IsDown(MouseCode key)
{
	return m_MouseStates[(uint16_t)key].IsDown;
}

bool Input::IsPressed(MouseCode key)
{
	return m_MouseStates[(uint16_t)key].IsDown && m_MouseStates[(uint16_t)key].Changed;
}

bool Input::IsUnpressed(MouseCode key)
{
	return !m_MouseStates[(uint16_t)key].IsDown && m_MouseStates[(uint16_t)key].Changed;
}

void Input::Reset()
{
	for (ButtonState& state : m_KeysStates)
		state.Changed = false;
	for (ButtonState& state : m_MouseStates)
		state.Changed = false;
}

void Input::Press(uint16_t key)
{
	m_KeysStates[key].IsDown = true;
	m_KeysStates[key].Changed = true;
}

void Input::Unpress(uint16_t key)
{
	m_KeysStates[key].IsDown = false;
	m_KeysStates[key].Changed = true;
}

void Input::MousePress(uint16_t key)
{
	m_MouseStates[key].IsDown = true;
	m_MouseStates[key].Changed = true;
}

void Input::MouseUnpress(uint16_t key)
{
	m_MouseStates[key].IsDown = false;
	m_MouseStates[key].Changed = true;
}

uint16_t Input::VKToKeyCode(uint16_t vkKeyCode)
{
	switch (vkKeyCode)
	{
		case VK_SPACE: return (uint16_t)KEY_SPACE;
		case VK_OEM_7: return (uint16_t)KEY_APOSTROPHE;
		case VK_OEM_COMMA: return (uint16_t)KEY_COMMA;
		case VK_OEM_MINUS: return (uint16_t)KEY_MINUS;
		case VK_OEM_PERIOD: return (uint16_t)KEY_PERIOD;
		case VK_OEM_2: return (uint16_t)KEY_SLASH;
		case 0x30: return (uint16_t)KEY_0;
		case 0x31: return (uint16_t)KEY_1;
		case 0x32: return (uint16_t)KEY_2;
		case 0x33: return (uint16_t)KEY_3;
		case 0x34: return (uint16_t)KEY_4;
		case 0x35: return (uint16_t)KEY_5;
		case 0x36: return (uint16_t)KEY_6;
		case 0x37: return (uint16_t)KEY_7;
		case 0x38: return (uint16_t)KEY_8;
		case 0x39: return (uint16_t)KEY_9;
		case VK_OEM_1: return (uint16_t)KEY_SEMICOLON;
		case VK_OEM_PLUS: return (uint16_t)KEY_EQUAL;
		case 0x41: return (uint16_t)KEY_A;
		case 0x42: return (uint16_t)KEY_B;
		case 0x43: return (uint16_t)KEY_C;
		case 0x44: return (uint16_t)KEY_D;
		case 0x45: return (uint16_t)KEY_E;
		case 0x46: return (uint16_t)KEY_F;
		case 0x47: return (uint16_t)KEY_G;
		case 0x48: return (uint16_t)KEY_H;
		case 0x49: return (uint16_t)KEY_I;
		case 0x4a: return (uint16_t)KEY_J;
		case 0x4b: return (uint16_t)KEY_K;
		case 0x4c: return (uint16_t)KEY_L;
		case 0x4d: return (uint16_t)KEY_M;
		case 0x4e: return (uint16_t)KEY_N;
		case 0x4f: return (uint16_t)KEY_O;
		case 0x50: return (uint16_t)KEY_P;
		case 0x51: return (uint16_t)KEY_Q;
		case 0x52: return (uint16_t)KEY_R;
		case 0x53: return (uint16_t)KEY_S;
		case 0x54: return (uint16_t)KEY_T;
		case 0x55: return (uint16_t)KEY_U;
		case 0x56: return (uint16_t)KEY_V;
		case 0x57: return (uint16_t)KEY_W;
		case 0x58: return (uint16_t)KEY_X;
		case 0x59: return (uint16_t)KEY_Y;
		case 0x5a: return (uint16_t)KEY_Z;
		case VK_OEM_4: return (uint16_t)KEY_LEFT_BRACKET;
		case VK_OEM_5: return (uint16_t)KEY_BACKSLASH;
		case VK_OEM_6: return (uint16_t)KEY_RIGHT_BRACKET;
		case VK_OEM_3: return (uint16_t)KEY_GRAVE_ACCENT;
		//case VK_: return (uint16_t)KEY_WORLD_1;
		//case VK_: return (uint16_t)KEY_WORLD_2;

		/* Function keys */
		case VK_ESCAPE: return (uint16_t)KEY_ESCAPE;
		case VK_RETURN: return (uint16_t)KEY_ENTER;
		case VK_TAB: return (uint16_t)KEY_TAB;
		case VK_BACK: return (uint16_t)KEY_BACKSPACE;
		case VK_INSERT: return (uint16_t)KEY_INSERT;
		case VK_DELETE: return (uint16_t)KEY_DELETE;
		case VK_RIGHT: return (uint16_t)KEY_RIGHT;
		case VK_LEFT: return (uint16_t)KEY_LEFT;
		case VK_DOWN: return (uint16_t)KEY_DOWN;
		case VK_UP: return (uint16_t)KEY_UP;
		case VK_PRIOR: return (uint16_t)KEY_PAGE_UP;
		case VK_NEXT: return (uint16_t)KEY_PAGE_DOWN;
		case VK_HOME: return (uint16_t)KEY_HOME;
		case VK_END: return (uint16_t)KEY_END;
		case VK_CAPITAL: return (uint16_t)KEY_CAPS_LOCK;
		case VK_SCROLL: return (uint16_t)KEY_SCROLL_LOCK;
		case VK_NUMLOCK: return (uint16_t)KEY_NUM_LOCK;
		case VK_SNAPSHOT: return (uint16_t)KEY_PRINT_SCREEN;
		case VK_PAUSE: return (uint16_t)KEY_PAUSE;
		case VK_F1: return (uint16_t)KEY_F1;
		case VK_F2: return (uint16_t)KEY_F2;
		case VK_F3: return (uint16_t)KEY_F3;
		case VK_F4: return (uint16_t)KEY_F4;
		case VK_F5: return (uint16_t)KEY_F5;
		case VK_F6: return (uint16_t)KEY_F6;
		case VK_F7: return (uint16_t)KEY_F7;
		case VK_F8: return (uint16_t)KEY_F8;
		case VK_F9: return (uint16_t)KEY_F9;
		case VK_F10: return (uint16_t)KEY_F10;
		case VK_F11: return (uint16_t)KEY_F11;
		case VK_F12: return (uint16_t)KEY_F12;
		case VK_F13: return (uint16_t)KEY_F13;
		case VK_F14: return (uint16_t)KEY_F14;
		case VK_F15: return (uint16_t)KEY_F15;
		case VK_F16: return (uint16_t)KEY_F16;
		case VK_F17: return (uint16_t)KEY_F17;
		case VK_F18: return (uint16_t)KEY_F18;
		case VK_F19: return (uint16_t)KEY_F19;
		case VK_F20: return (uint16_t)KEY_F20;
		case VK_F21: return (uint16_t)KEY_F21;
		case VK_F22: return (uint16_t)KEY_F22;
		case VK_F23: return (uint16_t)KEY_F23;
		case VK_F24: return (uint16_t)KEY_F24;
		//case VK_F25: return (uint16_t)KEY_F25;

		/* Keypad */
		case VK_NUMPAD0: return (uint16_t)KEY_KP_0;
		case VK_NUMPAD1: return (uint16_t)KEY_KP_1;
		case VK_NUMPAD2: return (uint16_t)KEY_KP_2;
		case VK_NUMPAD3: return (uint16_t)KEY_KP_3;
		case VK_NUMPAD4: return (uint16_t)KEY_KP_4;
		case VK_NUMPAD5: return (uint16_t)KEY_KP_5;
		case VK_NUMPAD6: return (uint16_t)KEY_KP_6;
		case VK_NUMPAD7: return (uint16_t)KEY_KP_7;
		case VK_NUMPAD8: return (uint16_t)KEY_KP_8;
		case VK_NUMPAD9: return (uint16_t)KEY_KP_9;
		case VK_DECIMAL: return (uint16_t)KEY_KP_DECIMAL;
		case VK_DIVIDE: return (uint16_t)KEY_KP_DIVIDE;
		case VK_MULTIPLY: return (uint16_t)KEY_KP_MULTIPLY;
		case VK_SUBTRACT: return (uint16_t)KEY_KP_SUBTRACT;
		case VK_ADD: return (uint16_t)KEY_KP_ADD;
		/*case VK_: return (uint16_t)KEY_KP_ENTER;
		case VK_: return (uint16_t)KEY_KP_EQUAL;*/

		case VK_SHIFT: return (uint16_t)KEY_LEFT_SHIFT;
		case VK_CONTROL: return (uint16_t)KEY_LEFT_CONTROL;
		case VK_MENU: return (uint16_t)KEY_LEFT_ALT;
		//case VK_: return (uint16_t)KEY_LEFT_SUPER;
		//case VK_RSHIFT: return (uint16_t)KEY_RIGHT_SHIFT;
		//case VK_RCONTROL: return (uint16_t)KEY_RIGHT_CONTROL;
		//case VK_RMENU: return (uint16_t)KEY_RIGHT_ALT;
		//case VK_: return (uint16_t)KEY_RIGHT_SUPER;
		//case VK_MENU: return (uint16_t)KEY_MENU;
	}
	return 0;
}

uint16_t Input::VKToMouseCode(uint16_t vkMouseCode)
{
	switch (vkMouseCode)
	{
		case MK_LBUTTON: return (uint16_t)MOUSE_BUTTON_0;
		case MK_RBUTTON: return (uint16_t)MOUSE_BUTTON_1;
		case MK_MBUTTON: return (uint16_t)MOUSE_BUTTON_2;
		/*case VK_: return (uint16_t)MOUSE_BUTTON_3;
		case VK_: return (uint16_t)MOUSE_BUTTON_4;
		case VK_: return (uint16_t)MOUSE_BUTTON_5;
		case VK_: return (uint16_t)MOUSE_BUTTON_6;
		case VK_: return (uint16_t)MOUSE_BUTTON_7;
		case VK_: return (uint16_t)MOUSE_BUTTON_LAST;
		case VK_: return (uint16_t)MOUSE_BUTTON_LEFT;
		case VK_: return (uint16_t)MOUSE_BUTTON_RIGHT;
		case VK_: return (uint16_t)MOUSE_BUTTON_MIDDLE;*/
	}
	return 0;
}
