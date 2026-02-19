// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Core/Input/KeyCodes.hpp>
#include <NuMath/Algebra/Vector/Vector2.hpp>

namespace NuEngine::Runtime
{
	class Application;
}

class NuOpenGLWidget;

namespace NuEngine::Core
{
	class Input
	{
	public:
		[[nodiscard]] static bool IsKeyPressed(KeyCode key) noexcept;
		[[nodiscard]] static bool IsMouseButtonPressed(KeyCode button) noexcept;
		[[nodiscard]] static NuMath::Vector2 GetMousePosition() noexcept;
		[[nodiscard]] static float GetMouseX() noexcept;
		[[nodiscard]] static float GetMouseY() noexcept;
	private:
		static void TransitionPressed(KeyCode key) noexcept;
		static void TransitionReleased(KeyCode key) noexcept;
		static void UpdateMouse(float x, float y) noexcept;
		static void UpdateMouseButton(KeyCode button, bool pressed) noexcept;

		friend class NuEngine::Runtime::Application;
		friend class ::NuOpenGLWidget;
	};
}