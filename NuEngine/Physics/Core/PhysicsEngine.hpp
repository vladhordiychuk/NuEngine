// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.
#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Core/Types/Result.hpp>
#include <Physics/Errors/PhysicsError.hpp>
#include <Physics/Bodies/RigidBody.hpp>
#include <NuMath/NuMath.hpp>
#include <NuEngine/Core/API.hpp>

namespace NuEngine::Physics
{
    class NU_API PhysicsEngine
    {
    public:
        static Core::Result<void, PhysicsError> Initialize() noexcept;
        static void Shutdown() noexcept;
        static void Update(float deltaTime) noexcept;
        static JPH::PhysicsSystem& GetSystem() noexcept;
        static JPH::BodyInterface& GetBodyInterface() noexcept;
        static RigidBody CreateBox(
            const NuMath::Vector3& halfExtents,
            const NuMath::Vector3& position,
            BodyType type) noexcept;
    };
}