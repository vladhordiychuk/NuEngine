#pragma once

#include <variant>
#include <string>
#include <source_location>

#include <Core/Types/AppError.hpp>
#include <Platform/Errors/WindowError.hpp>
#include <Graphics/Errors/GraphicsError.hpp>
#include <Core/Errors/FileSystemError.hpp>

namespace NuEngine::Runtime
{
    class EngineError
    {
    public:
        using VariantType = std::variant<
            Core::AppError,
            Platform::WindowError,
            Graphics::GraphicsError,
            Core::FileSystemError
        >;

        EngineError(const char* msg, std::source_location loc = std::source_location::current())
            : m_error(Core::AppError(msg, loc)) {
        }

        EngineError(std::string msg, std::source_location loc = std::source_location::current())
            : m_error(Core::AppError(std::move(msg), loc)) {
        }

        EngineError(Core::AppError err) : m_error(std::move(err)) {}

        template <typename T>
            requires (!std::is_same_v<std::decay_t<T>, EngineError> &&
        !std::is_same_v<std::decay_t<T>, Core::AppError>)
            EngineError(T&& innerError, std::source_location loc = std::source_location::current())
            : m_error(Core::AppError(
                ToString(innerError),
                loc
            ))
        {
        }

        [[nodiscard]] std::string ToString() const
        {
            return std::visit([](auto&& arg) -> std::string {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (requires { arg.ToString(); }) return arg.ToString();
                else return "Unknown Error";
                }, m_error);
        }

        friend std::ostream& operator<<(std::ostream& os, const EngineError& e) {
            return os << e.ToString();
        }

    private:
        VariantType m_error;
    };

    NU_FORCEINLINE std::string ToString(const EngineError& e) { return e.ToString(); }
}

template <>
struct std::formatter<NuEngine::Runtime::EngineError> : std::formatter<std::string> {
    auto format(const NuEngine::Runtime::EngineError& err, format_context& ctx) const {
        return formatter<string>::format(err.ToString(), ctx);
    }
};