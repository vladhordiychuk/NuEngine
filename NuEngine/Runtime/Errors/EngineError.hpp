#pragma once

#include <variant>
#include <string>
#include <source_location>
#include <iostream>

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

        EngineError(std::string msg, std::source_location loc = std::source_location::current())
            : m_error(Core::AppError(std::move(msg), loc)) {
        }

        EngineError(const char* msg, std::source_location loc = std::source_location::current())
            : m_error(Core::AppError(msg, loc)) {
        }

        template <typename T>
            requires (std::is_constructible_v<VariantType, T&&> &&
        !std::is_same_v<std::decay_t<T>, EngineError>)
            EngineError(T&& specificError)
            : m_error(std::forward<T>(specificError))
        {
        }

        template <typename T>
            requires (!std::is_constructible_v<VariantType, T&&> &&
        !std::is_same_v<std::decay_t<T>, EngineError>)
            EngineError(T&& otherError, std::source_location loc = std::source_location::current())
            : m_error(Core::AppError(ToString(otherError), loc))
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

        template <typename T>
        [[nodiscard]] bool Is() const {
            return std::holds_alternative<T>(m_error);
        }

        template <typename T>
        [[nodiscard]] const T* Get() const {
            return std::get_if<T>(&m_error);
        }

        friend std::ostream& operator<<(std::ostream& os, const EngineError& e) {
            return os << e.ToString();
        }

    private:
        VariantType m_error;
    };

    NU_FORCEINLINE std::string ToString(const EngineError& e) { return e.ToString(); }
}