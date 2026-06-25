#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

namespace NuEngine::Physics
{
	namespace Layers
	{
		static constexpr JPH::ObjectLayer STATIC = 0;
		static constexpr JPH::ObjectLayer DYNAMIC = 1;
		static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
	}

	namespace BroadPhaseLayers
	{
		static constexpr JPH::BroadPhaseLayer STATIC(0);
		static constexpr JPH::BroadPhaseLayer DYNAMIC(1);
		static constexpr uint32_t NUM_LAYERS(2);
	}

	class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
	{
	public:
		BPLayerInterfaceImpl()
		{
			m_ObjectToBroadPhase[Layers::STATIC] = BroadPhaseLayers::STATIC;
			m_ObjectToBroadPhase[Layers::DYNAMIC] = BroadPhaseLayers::DYNAMIC;
		}

		uint32_t GetNumBroadPhaseLayers() const override { return BroadPhaseLayers::NUM_LAYERS; }
		JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override { return m_ObjectToBroadPhase[inLayer]; }
		const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override
		{
			if (inLayer == BroadPhaseLayers::STATIC) return "STATIC";
			if (inLayer == BroadPhaseLayers::DYNAMIC) return "DYNAMIC";
			return "NON_MOVING";
		}
	private:
		JPH::BroadPhaseLayer m_ObjectToBroadPhase[Layers::NUM_LAYERS];
	};

	class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
	{
	public:
		bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override
		{
			switch (inLayer1)
			{
			case Layers::STATIC:
				return inLayer2 == BroadPhaseLayers::DYNAMIC;
			case Layers::DYNAMIC:
				return true;
			default:
				return false;
			}
		}
	};

	class ObjectLayersPairFilterImpl : public JPH::ObjectLayerPairFilter
	{
	public:
		bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override
		{
			switch (inObject1)
			{
			case Layers::STATIC:
				return inObject2 == Layers::DYNAMIC;
			case Layers::DYNAMIC:
				return true;
			default:
				return false;
			}
		}
	};
}