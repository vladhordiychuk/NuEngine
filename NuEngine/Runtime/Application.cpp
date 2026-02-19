#include <Runtime/Application.hpp>
#include <Core/Logging/Logger.hpp>
#include <Platform/IWindow.hpp>
#include <Platform/Common/WindowEvents.hpp>
#include <Graphics/Abstractions/Core/GraphicsFactory.hpp>
#include <Graphics/Abstractions/Core/IRenderDevice.hpp>
#include <Renderer/Pipelines/Forward/ForwardPipeline.hpp>
#include <Renderer/Camera.hpp>
#include <Core/Timer/Time.hpp>
#include <Core/Input/Input.hpp>

#include <iostream>
#include <glad/glad.h>

namespace NuEngine::Runtime
{
	Application::Application(const ApplicationSpecification& spec) noexcept
		: m_specification(spec)
		, m_fileSystem("res/")
		, m_window(nullptr)
		, m_pipeline(nullptr)
		, m_isRunning(false)
	{
		std::filesystem::path logDir("logs");
		if (!std::filesystem::exists(logDir)) {
			std::filesystem::create_directories(logDir);
		}
		Core::Logger::Init("logs/nuengine.logs")
			.MapError([](auto&& err) {
			std::cerr << "[FATAL] Logger Init Failed: " << ToString(err) << std::endl;
			std::abort();
			return err;
				})
			.Ignore();

		LOG_INFO("NuEngine Constructed.");
	}

	Application::~Application() noexcept
	{
		Shutdown().Match(
			[]() { LOG_INFO("NuEngine Shutdown Cleanly."); },
			[](const auto& err) { LOG_ERROR("Shutdown error: {}", ToString(err)); }
		);
		Core::Logger::Shutdown();
	}

	Core::Result<std::unique_ptr<Platform::IWindow>, EngineError> Application::CreateAppWindow() noexcept
	{
		NU_UNWRAP(window, Platform::CreatePlatformWindow());

		Platform::WindowConfig config;
		config.SetTitle("NuEngine");
		config.SetSize(1280, 720);
		config.SetResizable(true);
		config.SetDecorated(true);

		NU_CHECK(window->Initialize(config));

		window->SetEventCallback([this](Platform::WindowEvent& event) {
			this->OnEvent(event);
			});

		NU_CHECK(window->Show());

		return Core::Ok(std::move(window));
	}

	Core::Result<std::unique_ptr<Graphics::IRenderDevice>, EngineError> Application::CreateRenderDevice(Platform::IWindow* window) noexcept
	{
		NU_UNWRAP(device, Graphics::GraphicsFactory::CreateDevice(Graphics::GraphicsAPI::OpenGL, window));
		return Core::Ok(std::move(device));
	}

	Core::Result<void, EngineError> Application::Initialize() noexcept
	{
		if (m_state != AppState::Created)
		{
			return Core::Err(EngineError("Application cannot be initialized in current state", std::source_location::current()));
		}

		LOG_INFO("Initializing Subsystems...");

		if (m_specification.Windowed)
		{
			NU_UNWRAP(window, CreateAppWindow());
			m_window = std::move(window);

			NU_UNWRAP(device, CreateRenderDevice(m_window.get()));
			m_renderDevice = std::move(device);
		}
		else
		{
			LOG_INFO("Running in Headless/Editor mode. Window creation skipped.");
		}

		if (m_renderDevice)
		{
			m_pipeline = std::make_unique<Renderer::ForwardPipeline>(m_renderDevice.get());
		}

		Core::Time::Initialize();

		m_isRunning = true;
		m_state = AppState::Running;

		LOG_INFO("Application initialized successfully.");

		return Core::Ok();
	}

	Core::Result<void, EngineError> Application::Run() noexcept
	{
		NU_CHECK(Initialize());

		LOG_INFO("Entering main loop...");

		if (!m_specification.Windowed) return Core::Ok();

		while (m_isRunning)
		{
			auto loopResult = MainLoop();
			if (loopResult.IsError())
			{
				LOG_CRITICAL("Main Loop Failure: {}", loopResult.UnwrapError().ToString());
				m_isRunning = false;
				return loopResult;
			}
		}

		return Shutdown();
	}

	Core::Result<void, EngineError> Application::Shutdown() noexcept
	{
		if (m_state == AppState::ShuttingDown || m_state == AppState::Terminated)
			return Core::Ok();

		m_state = AppState::ShuttingDown;
		m_pipeline.reset();
		m_renderDevice.reset();
		m_window.reset();
		m_isRunning = false;
		m_state = AppState::Terminated;
		LOG_INFO("Application shutdown successfully");
		return Core::Ok();
	}

	Core::Result<void, EngineError> Application::PollEvents() noexcept
	{
		if (m_window) NU_CHECK(m_window->ProcessEvents());
		return Core::Ok();
	}

	void Application::OnEvent(Platform::WindowEvent& event) noexcept
	{
		switch (event.GetType())
		{
		case Platform::EventType::WindowClose:
			OnWindowClose(static_cast<Platform::WindowClosedEvent&>(event));
			break;
		case Platform::EventType::WindowResize:
			OnWindowResize(static_cast<Platform::WindowResizedEvent&>(event));
			break;
		case Platform::EventType::KeyPressed:
		{
			auto& keyEvent = static_cast<Platform::KeyEvent&>(event);
			Core::Input::TransitionPressed(static_cast<Core::KeyCode>(keyEvent.key));
			break;
		}
		case Platform::EventType::KeyReleased:
		{
			auto& keyEvent = static_cast<Platform::KeyEvent&>(event);
			Core::Input::TransitionReleased(static_cast<Core::KeyCode>(keyEvent.key));
			break;
		}
		case Platform::EventType::MouseButtonPressed:
		{
			auto& mouseEvent = static_cast<Platform::MouseButtonEvent&>(event);
			Core::Input::UpdateMouseButton(static_cast<Core::KeyCode>(mouseEvent.button), true);
			break;
		}
		case Platform::EventType::MouseButtonReleased:
		{
			auto& mouseEvent = static_cast<Platform::MouseButtonEvent&>(event);
			Core::Input::UpdateMouseButton(static_cast<Core::KeyCode>(mouseEvent.button), false);
			break;
		}
		case Platform::EventType::MouseMoved:
		{
			auto& mouseEvent = static_cast<Platform::MouseMoveEvent&>(event);
			Core::Input::UpdateMouse((float)mouseEvent.x, (float)mouseEvent.y);
			break;
		}
		}
	}

	bool Application::OnWindowClose(Platform::WindowClosedEvent& event) noexcept
	{
		m_isRunning = false;
		return true;
	}

	bool Application::OnWindowResize(Platform::WindowResizedEvent& event) noexcept
	{
		if (event.width == 0 || event.height == 0) return false;
		if (m_pipeline) m_pipeline->SetViewport(0, 0, event.width, event.height);
		return false;
	}

	Core::Result<void, EngineError> Application::Update() noexcept
	{
		OnUpdate(Core::Time::GetDeltaTime());
		return Core::Ok();
	}

	void Application::OnUpdate(float deltaTime)
	{
		if (!m_pipeline) return;

		auto camera = m_pipeline->GetCamera();
		if (camera)
		{
			float speed = 5.0f * deltaTime;

			if (Core::Input::IsKeyPressed(Core::Key::LeftShift)) speed *= 2.0f;

			float moveX = 0.0f;
			float moveY = 0.0f;
			float moveZ = 0.0f;

			if (Core::Input::IsKeyPressed(Core::Key::W)) moveZ -= 1.0f;
			if (Core::Input::IsKeyPressed(Core::Key::S)) moveZ += 1.0f;
			if (Core::Input::IsKeyPressed(Core::Key::A)) moveX -= 1.0f;
			if (Core::Input::IsKeyPressed(Core::Key::D)) moveX += 1.0f;

			if (Core::Input::IsKeyPressed(Core::Key::Space)) moveY += 1.0f;
			if (Core::Input::IsKeyPressed(Core::Key::LeftControl) || Core::Input::IsKeyPressed(17)) moveY -= 1.0f;

			if (moveX != 0.0f || moveY != 0.0f || moveZ != 0.0f)
			{
				NuMath::Vector3 movement(moveX, moveY, moveZ);
				camera->Move(movement.Normalize() * speed);
			}

			float rotSpeed = 2.0f * deltaTime;
			float rotX = 0.0f;
			float rotY = 0.0f;

			if (Core::Input::IsKeyPressed(Core::Key::Left)) rotY += rotSpeed;
			if (Core::Input::IsKeyPressed(Core::Key::Right)) rotY -= rotSpeed;
			if (Core::Input::IsKeyPressed(Core::Key::Up)) rotX += rotSpeed;
			if (Core::Input::IsKeyPressed(Core::Key::Down)) rotX -= rotSpeed;

			if (rotX != 0.0f || rotY != 0.0f)
			{
				NuMath::Vector3 rot(rotX, rotY, 0.0f);
				camera->Rotate(rot);
			}
		}
	}

	Core::Result<void, EngineError> Application::Render() noexcept
	{
		if (m_pipeline) NU_CHECK(m_pipeline->Render(true));
		return Core::Ok();
	}

	Core::Result<void, EngineError> Application::MainLoop() noexcept
	{
		Core::Time::Update();
		NU_CHECK(PollEvents());
		NU_CHECK(Update());
		NU_CHECK(Render());
		return Core::Ok();
	}

	void Application::RenderFrame()
	{
		if (m_pipeline) m_pipeline->Render(false);
		OnRender();
	}

	void Application::UpdateFrame(float deltaTime)
	{
		if (m_window) m_window->ProcessEvents();
		OnUpdate(deltaTime);
	}

	void Application::InitializeGraphicsForEditor()
	{
		LOG_INFO("Initializing Graphics for Editor Mode...");
		if (!gladLoadGL()) return;

		auto deviceResult = CreateRenderDevice(nullptr);
		if (deviceResult.IsOk()) {
			m_renderDevice = std::move(deviceResult.Unwrap());
			m_pipeline = std::make_unique<Renderer::ForwardPipeline>(m_renderDevice.get());
			m_pipeline->SetViewport(0, 0, 800, 600);
		}
	}
}