#include <Core/Input/Input.hpp>
#include <unordered_map>

namespace NuEngine::Core
{
	struct InputState
	{
		std::unordered_map<KeyCode, bool> KeyMap;
		std::unordered_map<KeyCode, bool> MouseButtonMap;
		float MouseX = 0.0f;
		float MouseY = 0.0f;
	};

	static InputState s_InputState;

	bool Input::IsKeyPressed(KeyCode key) noexcept
	{
		auto it = s_InputState.KeyMap.find(key);

		if (it != s_InputState.KeyMap.end())
		{
			return it->second;
		}

		return false;
	}

	bool Input::IsMouseButtonPressed(KeyCode key) noexcept
	{
		auto it = s_InputState.MouseButtonMap.find(key);

		if (it != s_InputState.MouseButtonMap.end())
		{
			return it->second;
		}

		return false;
	}

	NuMath::Vector2 Input::GetMousePosition() noexcept
	{
		return NuMath::Vector2(s_InputState.MouseX, s_InputState.MouseY);
	}

	float Input::GetMouseX() noexcept
	{
		return s_InputState.MouseX;
	}

	float Input::GetMouseY() noexcept
	{
		return s_InputState.MouseY;
	}

	void Input::TransitionPressed(KeyCode key) noexcept
	{
		s_InputState.KeyMap[key] = true;
	}

	void Input::TransitionReleased(KeyCode key) noexcept
	{
		s_InputState.KeyMap[key] = false;
	}

	void Input::UpdateMouse(float x, float y) noexcept
	{
		s_InputState.MouseX = x;
		s_InputState.MouseY = y;
	}

	void Input::UpdateMouseButton(KeyCode button, bool pressed) noexcept
	{
		s_InputState.MouseButtonMap[button] = pressed;
	}
}
