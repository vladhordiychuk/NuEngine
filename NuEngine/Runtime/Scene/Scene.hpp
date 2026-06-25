// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuEngine/ECS/Entity.hpp>
#include <NuEngine/ECS/Components.hpp>
#include <NuEngine/Core/API.hpp>

#include <entt/entt.hpp>
#include <unordered_map>
#include <NuEngine/Weave/WeaveChunk.hpp> 

namespace NuEngine::Runtime
{
    class NU_API Scene
    {
    public:
        Scene() = default;
        ~Scene() = default;

        ECS::Entity CreateEntity(const std::string& name = "Empty Entity");
        void OnUpdate(float deltaTime);

        entt::registry& GetRegistry() { return m_Registry; }

        void AddMassScript(ECS::Entity entity, const Weave::WeaveGraphAsset* asset)
        {
            if (!asset) return;

            // ВИПРАВЛЕНО: Тепер ми використовуємо наш новий оператор перетворення
            uint32_t entityId = static_cast<uint32_t>(entity);

            if (m_MassWeaveSystems.find(asset) == m_MassWeaveSystems.end())
            {
                m_MassWeaveSystems.emplace(asset, Weave::WeavePoolManager(asset));
            }

            m_MassWeaveSystems[asset].Add(entityId);
        }

    private:
        entt::registry m_Registry;

        std::unordered_map<const Weave::WeaveGraphAsset*, Weave::WeavePoolManager> m_MassWeaveSystems;

        friend class ECS::Entity;
    };
}

namespace NuEngine::ECS
{
    template<typename T, typename... Args>
    inline T& Entity::AddComponent(Args&&... args)
    {
        return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
    }

    template<typename T>
    inline T& Entity::GetComponent()
    {
        return m_Scene->m_Registry.get<T>(m_EntityHandle);
    }

    template<typename T>
    inline bool Entity::HasComponent()
    {
        return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
    }
}