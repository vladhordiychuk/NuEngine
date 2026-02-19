// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <cstdint>

namespace NuEngine::Core
{
	using KeyCode = uint16_t;

	namespace Key
	{
		enum : KeyCode
		{
			Space = 32,
			A = 65, B = 66, C = 67, D = 68, E = 69, F = 70, G = 71, H = 72,
			I = 73, J = 74, K = 75, L = 76, M = 77, N = 78, O = 79, P = 80,
			Q = 81, R = 82, S = 83, T = 84, U = 85, V = 86, W = 87, X = 88,
			Y = 89, Z = 90,

			Escape = 256,
			Enter = 257,
			Tab = 258,
			Backspace = 259,
			Insert = 260,
			Delete = 261,
			Right = 262,
			Left = 263,
			Down = 264,
			Up = 265,
			PageUp = 266,
			PageDown = 267,
			Home = 268,
			End = 269,
			CapsLock = 280,
			ScrollLock = 281,
			NumLock = 282,
			PrintScreen = 283,
			Pause = 284,
			F1 = 290,
			F2 = 291,
			F3 = 292,
			F4 = 293,
			F5 = 294,
			F6 = 295,
			F7 = 296,
			F8 = 297,
			F9 = 298,
			F10 = 299,
			F11 = 300,
			F12 = 301,
			LeftShift = 340,
			LeftControl = 341,
			LeftAlt = 342
		};
	}

	namespace Mouse
	{
		enum : KeyCode
		{
			Button0 = 0,
			Button1 = 1,
			Button2 = 2,
			Button3 = 3,
			Button4 = 4,
			Button5 = 5,
			Left = Button0,
			Right = Button1,
			Middle = Button2
		};
	}
}