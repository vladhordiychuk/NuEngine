#include <Weave/NativeRegistry.hpp>
#include <Runtime/Scene/Scene.hpp>
#include <ECS/Components.hpp>

namespace NuEngine::Weave
{
    void NativeRegistry::Initialize()
    {
        using namespace NativeFuncId;

        Functions[GetDeltaTime] = [](NativeCallContext& ctx) {
            ctx.SetFloat(ctx.ReturnReg, ctx.DeltaTime);
            };

        Functions[GetEntityPosX] = [](NativeCallContext& ctx) {
            ctx.SetFloat(ctx.ReturnReg, 0.0f);
            };

        Functions[SetVelocityZ] = [](NativeCallContext& ctx) {
            float speed = ctx.GetFloat(ctx.ArgRegs[0]);

            if (ctx.CurrentScene)
            {
                entt::entity ent = static_cast<entt::entity>(ctx.EntityId);
                auto& registry = ctx.CurrentScene->GetRegistry();

                if (registry.any_of<NuEngine::ECS::RigidBodyComponent>(ent))
                {
                    auto& physics = registry.get<NuEngine::ECS::RigidBodyComponent>(ent);

                    if (physics.Body.IsValid())
                    {
                        physics.Body.SetLinearVelocity(NuMath::Vector3(0.0f, 0.0f, speed));
                    }
                }
            }
            };

        IsInitialized = true;
    }
}