#pragma once

#include <NuMath/NuMath.hpp>
#include <Physics/Bodies/RigidBody.hpp>
#include <Core/API.hpp>

#include <string>
#include <vector>
#include <array>

namespace NuEngine::ECS
{
    struct NU_API TransformComponent
    {
        NuMath::Vector3 Position = { 0.0f, 0.0f, 0.0f };
        NuMath::Quaternion Rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
        NuMath::Vector3 Scale = { 1.0f, 1.0f, 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const NuMath::Vector3& pos) : Position(pos) {}
    };

    struct NU_API RigidBodyComponent
    {
        Physics::RigidBody Body;
        Physics::BodyType Type = Physics::BodyType::Static;

        RigidBodyComponent() = default;
        RigidBodyComponent(const RigidBodyComponent&) = default;
    };

    struct NU_API NameComponent
    {
        std::string Name = "Empty entity";
    };
}