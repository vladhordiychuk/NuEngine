// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Core/Types/ErrorTraits.hpp>
#include <Core/Types/BasicError.hpp>

namespace NuEngine::Physics
{
	/**
	 * @brief Error codes for physics operations.
	 */
	enum class PhysicsErrorCode : uint8_t
	{
		Success,               ///> Operation completed successfully.
		SystemInitFailed,      ///> Failed to initialize the physics system.
		AllocatorInitFailed,   ///> Failed to initialize the memory allocator.
		JobSystemInitFailed,   ///> Failed to initialize the job system.
		BodyCreationFailed,    ///> Failed to create a physics body.
		ShapeCreationFailed,   ///> Failed to create a collision shape.
		InvalidBodyID,         ///> Invalid body identifier.
		MaxBodiesExceeded,     ///> Maximum number of bodies exceeded.
		SimulationError,       ///> Simulation step failed.
		InvalidLayer,          ///> Invalid collision layer.
		InvalidParameter       ///> Invalid parameter was provided.
	};
} // namespace NuEngine::Physics

namespace NuEngine::Core
{
	using PhysicsErrorCode = NuEngine::Physics::PhysicsErrorCode;

	/**
	 * @brief Error trait specialization for physics errors.
	 *
	 * Provides the error category name and conversion of
	 * error codes to human-readable strings.
	 */
	template <>
	struct ErrorTraits<PhysicsErrorCode>
	{
		static constexpr std::string_view Category = "PhysicsError";

		/**
		 * @brief Converts a physics error code to text.
		 *
		 * @param code Error code.
		 * @return Human-readable description.
		 */
		[[nodiscard]] static constexpr std::string_view ToString(PhysicsErrorCode code) noexcept
		{
			switch (code)
			{
				case PhysicsErrorCode::Success:
					return "Success";
				case PhysicsErrorCode::SystemInitFailed:
					return "System initialization failed";
				case PhysicsErrorCode::AllocatorInitFailed:
					return "Allocator initialization failed";
				case PhysicsErrorCode::JobSystemInitFailed:
					return "Job system initialization failed";
				case PhysicsErrorCode::BodyCreationFailed:
					return "Body creation failed";
				case PhysicsErrorCode::ShapeCreationFailed:
					return "Shape creation failed";
				case PhysicsErrorCode::InvalidBodyID:
					return "Invalid body ID";
				case PhysicsErrorCode::MaxBodiesExceeded:
					return "Maximum bodies limit exceeded";
				case PhysicsErrorCode::SimulationError:
					return "Simulation update error";
				case PhysicsErrorCode::InvalidLayer:
					return "Invalid collision layer";
				case PhysicsErrorCode::InvalidParameter:
					return "Invalid parameter";
			}

			return "Unknown Physics Error";
		}
	};

	static_assert(ErrorTraitsConcept<ErrorTraits<PhysicsErrorCode>, PhysicsErrorCode>);
} // namespace NuEngine::Core

namespace NuEngine::Physics
{
	using PhysicsError = Core::BasicError<PhysicsErrorCode>;
} // namespace NuEngine::Physics