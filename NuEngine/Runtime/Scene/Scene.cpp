#include "Scene.hpp"
#include <NuEngine/Physics/Core/PhysicsEngine.hpp>
#include <NuEngine/Weave/WeaveScriptSystem.hpp>
#include <NuEngine/Weave/WeaveComponent.hpp>
#include <NuEngine/Weave/WeaveChunkSystem.hpp> // <-- ДОДАНО ДЛЯ DoD

namespace NuEngine::Runtime
{
    ECS::Entity Scene::CreateEntity(const std::string& name)
    {
        entt::entity handle = m_Registry.create();
        return ECS::Entity(handle, this);
    }

    void Scene::OnUpdate(float deltaTime)
    {
        // 1. Оновлення масових SoA систем (DoD)
        for (auto& [asset, manager] : m_MassWeaveSystems)
        {
            Weave::WeaveChunkSystem::UpdateAll(manager, deltaTime, this);
        }

        // 2. Оновлення старої системи для залишкових об'єктів (AoS)
        auto weaveView = m_Registry.view<Weave::WeaveComponent>();
        for (auto entity : weaveView)
        {
            auto& weaveComp = weaveView.get<Weave::WeaveComponent>(entity);
            uint32_t eId = static_cast<uint32_t>(entity);

            Weave::WeaveScriptSystem::Update(&weaveComp, &eId, 1, deltaTime, this);
        }

        // 3. Фізика
        Physics::PhysicsEngine::Update(deltaTime);

        // 4. Синхронізація
        auto view = m_Registry.view<ECS::TransformComponent, ECS::RigidBodyComponent>();
        for (auto entity : view)
        {
            auto& transform = view.get<ECS::TransformComponent>(entity);
            auto& physics = view.get<ECS::RigidBodyComponent>(entity);

            if (physics.Body.IsValid())
            {
                transform.Position = physics.Body.GetPosition();
                transform.Rotation = physics.Body.GetRotation();
            }
        }
    }
}