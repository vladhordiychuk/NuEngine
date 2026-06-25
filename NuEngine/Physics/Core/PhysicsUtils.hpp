#pragma once

#include <NuMath/NuMath.hpp>

#include <Jolt/Jolt.h>
#include <Jolt/Math/Vec3.h>
#include <Jolt/Math/Vec4.h>
#include <Jolt/Math/Quat.h>
#include <Jolt/Math/Mat44.h>

namespace NuEngine::Physics
{
	inline JPH::Vec3 ToJolt(const NuMath::Vector3& v) noexcept
	{
		return JPH::Vec3(v.X(), v.Y(), v.Z());
	}

	inline JPH::Vec4 ToJolt(const NuMath::Vector4& v) noexcept
	{
		return JPH::Vec4(v.X(), v.Y(), v.Z(), v.W());
	}

	inline JPH::Quat ToJolt(const NuMath::Quaternion& q) noexcept
	{
		return JPH::Quat(q.X(), q.Y(), q.Z(), q.W());
	}
}