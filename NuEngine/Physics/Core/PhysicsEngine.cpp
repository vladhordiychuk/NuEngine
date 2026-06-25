#include <Physics/Core/PhysicsEngine.hpp>
#include <Physics/Core/PhysicsLayers.hpp>

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Core/Logging/Logger.hpp>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

namespace NuEngine::Physics
{
	static JPH::TempAllocatorImpl* s_TempAllocator = nullptr;
	static JPH::JobSystemThreadPool* s_JobSystem = nullptr;
	static JPH::PhysicsSystem* s_PhysicsSystem = nullptr;

	static BPLayerInterfaceImpl s_BPLayerInterface;
	static ObjectVsBroadPhaseLayerFilterImpl s_ObjVsBpFilter;
	static ObjectLayersPairFilterImpl s_ObjPairFilter;

    Core::Result<void, PhysicsError> PhysicsEngine::Initialize() noexcept
    {
        LOG_INFO("Initializing Jolt Physics Engine Backend...");

        JPH::RegisterDefaultAllocator();
        JPH::Factory::sInstance = new JPH::Factory();
        JPH::RegisterTypes();

        s_TempAllocator = new JPH::TempAllocatorImpl(10 * 1024 * 1024);
        s_JobSystem = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);

        // Параметри для Init: 
        // 1. maxBodies (збільшуємо з 1024 до 5000)
        // 2. numBodyMutexes (0 — дефолт)
        // 3. maxBodyPairs (максимальна кількість пар для обробки колізій, збільшуємо пропорційно)
        // 4. maxContactConstraints (ліміт одночасних контактів між тілами)
        s_PhysicsSystem = new JPH::PhysicsSystem();
        s_PhysicsSystem->Init(5000, 0, 10000, 10000, s_BPLayerInterface, s_ObjVsBpFilter, s_ObjPairFilter);

        return Core::Ok();
    }

	void PhysicsEngine::Shutdown() noexcept
	{
		LOG_INFO("Shutting down Jolt Physics Engine...");
		delete s_PhysicsSystem;
		delete s_JobSystem;
		delete s_TempAllocator;
		delete JPH::Factory::sInstance;
		JPH::Factory::sInstance = nullptr;
	}

	void PhysicsEngine::Update(float deltaTime) noexcept
	{
		if (s_PhysicsSystem)
		{
			s_PhysicsSystem->Update(deltaTime, 1, s_TempAllocator, s_JobSystem);
		}
	}

	JPH::PhysicsSystem& PhysicsEngine::GetSystem() noexcept
	{
		return *s_PhysicsSystem;
	}

	JPH::BodyInterface& PhysicsEngine::GetBodyInterface() noexcept
	{
		return s_PhysicsSystem->GetBodyInterface();
	}

    RigidBody PhysicsEngine::CreateBox(
        const NuMath::Vector3& halfExtents,
        const NuMath::Vector3& position,
        BodyType type) noexcept
    {
        JPH::BoxShapeSettings shapeSettings(
            JPH::Vec3(halfExtents.X(), halfExtents.Y(), halfExtents.Z())
        );

        JPH::ShapeSettings::ShapeResult shapeResult = shapeSettings.Create();
        if (shapeResult.HasError())
        {
            LOG_ERROR("Failed to create box shape: {}", shapeResult.GetError().c_str());
            return RigidBody{};
        }

        JPH::EMotionType motionType;
        JPH::ObjectLayer layer;

        switch (type)
        {
        case BodyType::Static:
            motionType = JPH::EMotionType::Static;
            layer = Layers::STATIC;
            break;
        case BodyType::Kinematic:
            motionType = JPH::EMotionType::Kinematic;
            layer = Layers::DYNAMIC;
            break;
        case BodyType::Dynamic:
        default:
            motionType = JPH::EMotionType::Dynamic;
            layer = Layers::DYNAMIC;
            break;
        }

        JPH::BodyCreationSettings bodySettings(
            shapeResult.Get(),
            JPH::RVec3(position.X(), position.Y(), position.Z()),
            JPH::Quat::sIdentity(),
            motionType,
            layer
        );

        bodySettings.mRestitution = 0.6f;  // ← додай це
        bodySettings.mFriction = 0.5f;

        JPH::BodyID bodyID = s_PhysicsSystem->GetBodyInterface()
            .CreateAndAddBody(bodySettings, JPH::EActivation::Activate);

        if (bodyID.IsInvalid())
        {
            LOG_ERROR("Failed to create rigid body!");
            return RigidBody{};
        }

        return RigidBody{ bodyID.GetIndexAndSequenceNumber() };
    }
}