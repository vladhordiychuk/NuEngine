#include <NuEngine/NuEngine.hpp>
#include <NuEngine/Runtime/EntryPoint.hpp>
#include <NuEngine/Runtime/Scene/Scene.hpp>
#include <NuEngine/ECS/Components.hpp>
#include <NuEngine/Physics/Core/PhysicsEngine.hpp>
#include <NuEngine/Weave/NativeRegistry.hpp>
#include <NuEngine/Weave/WeaveComponent.hpp>

#include <memory>
#include <vector>
#include <fstream>

class SandboxApp : public NuEngine::Runtime::Application
{
private:
    std::shared_ptr<NuEngine::Runtime::Scene> m_Scene;
    std::vector<NuEngine::ECS::Entity> m_Cubes;

public:
    SandboxApp() {}

    void OnInit() override
    {
        LOG_INFO("Sandbox App OnInit!");

        NuEngine::Weave::NativeRegistry::Initialize();

        static NuEngine::Weave::WeaveGraphAsset realAsset;

        std::ifstream file("test_script.wbc", std::ios::binary);
        if (file.is_open())
        {
            NuEngine::Weave::WbcFileHeader header;
            file.read(reinterpret_cast<char*>(&header), sizeof(header));

            realAsset.ByteCode.resize(header.BytecodeSize);
            file.read(reinterpret_cast<char*>(realAsset.ByteCode.data()), header.BytecodeSize);

            LOG_INFO("Successfully loaded test_script.wbc! Size: {} bytes", realAsset.ByteCode.size());
        }
        else
        {
            LOG_ERROR("Failed to load test_script.wbc! Перевірте, чи файл знаходиться у робочій директорії гри.");
        }

        m_Scene = std::make_shared<NuEngine::Runtime::Scene>();

        if (auto pipeline = GetPipeline())
        {
            if (auto camera = pipeline->GetCamera())
            {
                camera->SetPosition(NuMath::Vector3(0.0f, 0.0f, 20.0f));
                camera->Rotate(NuMath::Vector3(0.0f, 0.0f, 0.0f));
            }
        }

        auto floor = m_Scene->CreateEntity("Floor");
        floor.AddComponent<NuEngine::ECS::TransformComponent>(
            NuMath::Vector3(0.0f, -2.0f, 0.0f)
        );
        auto& floorPhysics = floor.AddComponent<NuEngine::ECS::RigidBodyComponent>();
        floorPhysics.Type = NuEngine::Physics::BodyType::Static;
        floorPhysics.Body = NuEngine::Physics::PhysicsEngine::CreateBox(
            NuMath::Vector3(50.0f, 0.5f, 50.0f),
            NuMath::Vector3(0.0f, -2.0f, 0.0f),
            NuEngine::Physics::BodyType::Static
        );

        struct CubeSpawnInfo { NuMath::Vector3 pos; };
        CubeSpawnInfo spawnPoints[] = {
            { NuMath::Vector3(0.0f, 5.0f,  0.0f) },
            { NuMath::Vector3(1.0f, 7.0f,  0.5f) },
            { NuMath::Vector3(-1.0f, 9.0f, -0.5f) },
            { NuMath::Vector3(0.5f, 11.0f, 1.0f) },
            { NuMath::Vector3(-0.5f, 13.0f,-1.0f) },
        };

        for (int i = 0; i < 5; i++)
        {
            auto cube = m_Scene->CreateEntity("Cube_" + std::to_string(i));

            cube.AddComponent<NuEngine::ECS::TransformComponent>(spawnPoints[i].pos);

            auto& physics = cube.AddComponent<NuEngine::ECS::RigidBodyComponent>();
            physics.Type = NuEngine::Physics::BodyType::Dynamic;
            physics.Body = NuEngine::Physics::PhysicsEngine::CreateBox(
                NuMath::Vector3(0.5f, 0.5f, 0.5f),
                spawnPoints[i].pos,
                NuEngine::Physics::BodyType::Dynamic
            );

            auto& weaveComp = cube.AddComponent<NuEngine::Weave::WeaveComponent>();
            weaveComp.Asset = &realAsset;
            weaveComp.Enable();

            m_Cubes.push_back(cube);
        }
    }

    void OnUpdate(float deltaTime) override
    {
        NuEngine::Runtime::Application::OnUpdate(deltaTime);
        m_Scene->OnUpdate(deltaTime);
    }

    void OnRender() override
    {
        if (auto pipeline = GetPipeline())
        {
            for (int i = 0; i < (int)m_Cubes.size(); i++)
            {
                auto& cube = m_Cubes[i];
                if (!cube.IsValid()) continue;
                if (!cube.HasComponent<NuEngine::ECS::TransformComponent>()) continue;

                auto& transform = cube.GetComponent<NuEngine::ECS::TransformComponent>();
                NuMath::Transform t;
                t.SetPosition(transform.Position);
                t.SetRotation(transform.Rotation);
                pipeline->RenderCube(t, true);
            }
        }
    }
};

#ifdef _WIN32
    #define NU_EXPORT extern "C" __declspec(dllexport)
#else
    #define NU_EXPORT extern "C" __attribute__((visibility("default")))
#endif

NU_EXPORT NuEngine::Runtime::Application* CreateGameApplication(const NuEngine::Runtime::ApplicationSpecification& spec)
{
    return new SandboxApp();
}

NU_EXPORT void DestroyGameApplication(NuEngine::Runtime::Application* app)
{
    delete app;
}

#ifndef NU_EDITOR_MODE

namespace NuEngine
{
    std::unique_ptr<Runtime::Application> CreateApplication()
    {
        return std::make_unique<SandboxApp>();
    }
}

#endif