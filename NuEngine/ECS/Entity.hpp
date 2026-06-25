// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <entt/entt.hpp>

namespace NuEngine::Runtime { class Scene; }

namespace NuEngine::ECS
{
    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity handle, Runtime::Scene* scene)
            : m_EntityHandle(handle), m_Scene(scene)
        {
        }

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args);

        template<typename T>
        T& GetComponent();

        template<typename T>
        bool HasComponent();

        bool IsValid() const { return m_EntityHandle != entt::null && m_Scene != nullptr; }

        operator uint32_t() const { return static_cast<uint32_t>(m_EntityHandle); }

    private:
        entt::entity m_EntityHandle{ entt::null };
        Runtime::Scene* m_Scene = nullptr;
    };
}