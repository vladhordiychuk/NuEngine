#include "RigidBody.hpp"

#include <NuEngine/Physics/Core/PhysicsEngine.hpp> 

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>

namespace NuEngine::Physics
{
    NuMath::Vector3 RigidBody::GetPosition() const
    {
        if (!IsValid()) return NuMath::Vector3(0.0f, 0.0f, 0.0f);

        auto& bodyInterface = PhysicsEngine::GetBodyInterface();
        JPH::BodyID bodyID(m_Handle);

        JPH::Vec3 pos = bodyInterface.GetPosition(bodyID);
        return NuMath::Vector3(pos.GetX(), pos.GetY(), pos.GetZ());
    }

    NuMath::Quaternion RigidBody::GetRotation() const
    {
        if (!IsValid()) return NuMath::Quaternion();

        auto& bodyInterface = PhysicsEngine::GetBodyInterface();
        JPH::BodyID bodyID(m_Handle);

        JPH::Quat rot = bodyInterface.GetRotation(bodyID);
        return NuMath::Quaternion(rot.GetX(), rot.GetY(), rot.GetZ(), rot.GetW());
    }

    void RigidBody::SetPosition(const NuMath::Vector3& position)
    {
        if (!IsValid()) return;
        auto& bodyInterface = PhysicsEngine::GetBodyInterface();
        bodyInterface.SetPosition(JPH::BodyID(m_Handle), JPH::Vec3(position.X(), position.Y(), position.Z()), JPH::EActivation::Activate);
    }

    void RigidBody::SetLinearVelocity(const NuMath::Vector3& velocity)
    {
        if (!IsValid()) return;
        auto& bodyInterface = PhysicsEngine::GetBodyInterface();
        bodyInterface.SetLinearVelocity(JPH::BodyID(m_Handle), JPH::Vec3(velocity.X(), velocity.Y(), velocity.Z()));
    }

    void RigidBody::AddForce(const NuMath::Vector3& force)
    {
        if (!IsValid()) return;
        auto& bodyInterface = PhysicsEngine::GetBodyInterface();
        bodyInterface.AddForce(JPH::BodyID(m_Handle), JPH::Vec3(force.X(), force.Y(), force.Z()));
    }
}