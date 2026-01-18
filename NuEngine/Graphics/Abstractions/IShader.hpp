// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/NuMath.hpp>

#include <string>

namespace NuEngine::Graphics
{
	class IShader
	{
	public:
		virtual ~IShader() = default;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetVec3(const std::string& name, const NuMath::Vector3& vec3) = 0;
		virtual void SetVec4(const std::string& name, const NuMath::Vector4& vec4) = 0;
		virtual void SetMat4x4(const std::string& name, const NuMath::Matrix4x4& mat4x4) = 0;
	};
} //namespace NuEngine::Graphics