#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"

struct ButtonState
{
	bool IsDown = false;
	bool Changed = false;
};

class Input
{
public:
	static bool IsDown(KeyCode key);
	static bool IsPressed(KeyCode key);
	static bool IsUnpressed(KeyCode key);

	static bool IsDown(MouseCode key);
	static bool IsPressed(MouseCode key);
	static bool IsUnpressed(MouseCode key);

	static void SetMousePos(int x, int y) { m_MouseX = x; m_MouseY = y; }
	static int GetMouseX() { return m_MouseX; }
	static int GetMouseY() { return m_MouseY; }

	static void Reset();

	static void Press(uint16_t key);
	static void Unpress(uint16_t key);

	static void MousePress(uint16_t key);
	static void MouseUnpress(uint16_t key);

	static uint16_t VKToKeyCode(uint16_t vkKeyCode);
	static uint16_t VKToMouseCode(uint16_t vkMouseCode);
private:
	static ButtonState m_KeysStates[(uint16_t)KEY_MENU]; // last key
	static ButtonState m_MouseStates[(uint16_t)MOUSE_BUTTON_MIDDLE]; // last mouse button supported
	static int m_MouseX, m_MouseY;
};