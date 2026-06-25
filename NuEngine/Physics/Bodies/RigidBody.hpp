// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.

#pragma once

#include <cstdint>

#include <NuMath/NuMath.hpp>
#include <NuEngine/Core/API.hpp>

namespace NuEngine::Physics
{
    enum class BodyType
    {
        Static,
        Dynamic,
        Kinematic
    };

    class NU_API RigidBody
    {
    public:
        RigidBody() = default;
        explicit RigidBody(uint32_t handle) : m_Handle(handle) {}

        [[nodiscard]] bool IsValid() const { return m_Handle != 0xFFFFFFFF; }

        [[nodiscard]] NuMath::Vector3 GetPosition() const;
        [[nodiscard]] NuMath::Quaternion GetRotation() const;

        void SetPosition(const NuMath::Vector3& position);
        void SetLinearVelocity(const NuMath::Vector3& velocity);
        void AddForce(const NuMath::Vector3& force);

        [[nodiscard]] uint32_t GetHandle() const { return m_Handle; }

    private:
        uint32_t m_Handle = 0xFFFFFFFF;
    };
}