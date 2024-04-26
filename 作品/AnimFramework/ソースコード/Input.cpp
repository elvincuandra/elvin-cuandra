#include "Input.h"

BYTE Input::m_keys[256];
BYTE Input::m_prevKeys[256];

void Input::Init()
{
	GetKeyboardState(m_keys);
}

void Input::Update()
{
	memcpy_s(m_prevKeys, sizeof(m_prevKeys), m_keys, sizeof(m_keys));
	GetKeyboardState(m_keys);
}

bool Input::IsKeyPress(BYTE keyCode)
{
	return m_keys[keyCode] & 0x80;
}

bool Input::IsKeyTrigger(BYTE keyCode)
{
	return (m_keys[keyCode] ^ m_prevKeys[keyCode]) & m_keys[keyCode] & 0x80;
}

bool Input::IsKeyRelease(BYTE keyCode)
{
	return (m_keys[keyCode] ^ m_prevKeys[keyCode]) & m_prevKeys[keyCode] & 0x80;;
}
