// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <array>
#include <string>
#include <string_view>
#include <format>
#include <ostream>
#include <iterator>

#include <Core/Types/Types.hpp>
#include <Core/Types/ErrorContext.hpp>

namespace NuEngine::Physics
{
    enum class PhysicsErrorCode
    {
        Success,
        SystemInitFailed,
        AllocatorInitFailed,
        JobSystemInitFailed,
        BodyCreationFailed,
        ShapeCreationFailed,
        InvalidBodyID,
        MaxBodiesExceeded,
        SimulationError,
        InvalidLayer,
        InvalidParameter
    };

    [[nodiscard]] constexpr std::string_view ToErrorString(PhysicsErrorCode code) noexcept
    {
        switch (code)
        {
        case PhysicsErrorCode::Success: return "Success";
        case PhysicsErrorCode::SystemInitFailed: return "System initialization failed";
        case PhysicsErrorCode::AllocatorInitFailed: return "Allocator initialization failed";
        case PhysicsErrorCode::JobSystemInitFailed: return "Job system initialization failed";
        case PhysicsErrorCode::BodyCreationFailed: return "Body creation failed";
        case PhysicsErrorCode::ShapeCreationFailed: return "Shape creation failed";
        case PhysicsErrorCode::InvalidBodyID: return "Invalid body ID";
        case PhysicsErrorCode::MaxBodiesExceeded: return "Maximum bodies limit exceeded";
        case PhysicsErrorCode::SimulationError: return "Simulation update error";
        case PhysicsErrorCode::InvalidLayer: return "Invalid collision layer";
        case PhysicsErrorCode::InvalidParameter: return "Invalid parameter";
        default: return "Unknown physics error";
        }
    }

    constexpr size_t MAX_ERROR_TRACE_DEPTH = 8;

    struct ErrorTrace
    {
        std::array<Core::ErrorContext, MAX_ERROR_TRACE_DEPTH> frames;
        uint8_t count = 0;

        constexpr void Push(const Core::ErrorContext& ctx) noexcept
        {
            if (count < MAX_ERROR_TRACE_DEPTH)
            {
                frames[count++] = ctx;
            }
        }

        [[nodiscard]] constexpr bool Empty() const noexcept { return count == 0; }
    };

    struct PhysicsError
    {
        PhysicsErrorCode code;
        Core::ErrorSeverity severity;
        std::string details;
        ErrorTrace trace;

        PhysicsError(PhysicsErrorCode c, std::string d = "",
            Core::ErrorContext ctx = {},
            Core::ErrorSeverity sev = Core::ErrorSeverity::Error) noexcept
            : code(c), severity(sev), details(std::move(d))
        {
            trace.Push(ctx);
        }

        bool operator==(PhysicsErrorCode c) const noexcept { return code == c; }
        bool operator!=(PhysicsErrorCode c) const noexcept { return code != c; }

        template <typename OutputIt>
        void FormatTo(OutputIt out) const
        {
            out = std::format_to(out, "PhysicsError: {}", ToErrorString(code));

            if (!details.empty())
            {
                out = std::format_to(out, " -> {}", details);
            }

            if (!trace.Empty())
            {
                out = std::format_to(out, "\nTrace:");
                for (size_t i = 0; i < trace.count; ++i)
                {
                    out = std::format_to(out, "\n  [{}] {}:{} ({})",
                        i, trace.frames[i].file, trace.frames[i].line, trace.frames[i].function);
                }
            }
        }

        [[nodiscard]] std::string ToString() const
        {
            std::string buffer;
            buffer.reserve(256 + details.size());
            FormatTo(std::back_inserter(buffer));
            return buffer;
        }
    }; // <-- Тут була пропущена крапка з комою

    inline std::ostream& operator<<(std::ostream& os, const PhysicsError& e)
    {
        return os << e.ToString();
    }
}

template <>
struct std::formatter<NuEngine::Physics::PhysicsError> : std::formatter<std::string> {
    auto format(const NuEngine::Physics::PhysicsError& err, format_context& ctx) const {
        err.FormatTo(ctx.out());
        return ctx.out();
    }
};