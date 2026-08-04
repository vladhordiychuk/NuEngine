// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <system_error>
#include <string_view>
#include <type_traits>
#include <format>
#include <ostream>

#include <Core/Types/ErrorSeverity.hpp>
#include <Core/Types/ErrorTrace.hpp>
#include <Core/Types/StaticString.hpp>
#include <Core/Types/ErrorPolicy.hpp>
#include <Core/Types/ErrorTraits.hpp>
#include <Core/Platform/CompilerMacros.hpp>
#include <Core/Types/ErrorFormatter.hpp>

namespace NuEngine::Core
{
    /**
     * @brief Constrains an error code type.
     *
     * The error code must be a scoped enumeration (`enum class`) to
     * provide strong type safety and prevent implicit conversions.
     */
	template <typename T>
    concept ErrorCode = std::is_enum_v<T>;

    /**
     * @brief Constrains an error policy type.
     *
     * A valid error policy must define the storage types used by
     * BasicError and all of them must be default constructible.
     */
	template <typename P>
	concept ErrorPolicyConcept = requires
	{
		typename P::Message;
		typename P::Path;
		typename P::Trace;
		typename P::SystemError;
	}
	&& std::default_initializable<typename P::Message>
	&& std::default_initializable<typename P::Path>
	&& std::default_initializable<typename P::Trace>
	&& std::default_initializable<typename P::SystemError>;

    static_assert(ErrorPolicyConcept<CompactErrorPolicy>);
    static_assert(ErrorPolicyConcept<MediumErrorPolicy>);
    static_assert(ErrorPolicyConcept<RichErrorPolicy>);

    /**
     * @brief Generic error object.
     *
     * Stores an engine-specific error code together with optional
     * diagnostic information defined by the selected error policy.
     *
     * @tparam CodeEnum Scoped enumeration representing the error code.
     * @tparam Policy   Storage policy controlling which diagnostic
     *                  information is stored.
     */
    template<ErrorCode CodeEnum, ErrorPolicyConcept Policy = RichErrorPolicy>
    class BasicError
    {
    public:
        /// Engine-specific error code type.
        using Code_t = CodeEnum;

        /// Storage policy used by this error.
        using Policy_t = Policy;

        /// Message storage type.
        using Message_t = typename Policy_t::Message;

        /// Path storage type.
        using Path_t = typename Policy_t::Path;

        /// Stack trace storage type.
        using Trace_t = typename Policy_t::Trace;

        /// Platform error storage type.
        using SystemError_t = typename Policy_t::SystemError;

        /// Error trait specialization.
        using Traits = ErrorTraits<CodeEnum>;

        /**
         * @brief Constructs a new error object.
         *
         * @param code      Engine-specific error code.
         * @param message   Human-readable error description.
         * @param path      Optional file path associated with the error.
         * @param sysError  Optional operating system error.
         * @param severity  Error severity level.
         * @param ctx       Initial stack trace frame.
         */
        explicit BasicError(
            Code_t code,
            std::string_view message = {},
            std::string_view path = {},
            std::error_code sysError = {},
            ErrorSeverity severity = ErrorSeverity::Error,
            ErrorContext ctx = {}) noexcept
            : m_Code(code)
            , m_Severity(severity)
            , m_Message(message)
            , m_Path(path)
            , m_SystemError(sysError)
        {
            m_Trace.Push(ctx);
        }

        /**
         * @brief Adds a stack trace frame.
         *
         * @param ctx Context describing the current call site.
         * @return Reference to this error.
         */
        BasicError& AddFrame(ErrorContext ctx = {}) noexcept
        {
            m_Trace.Push(ctx);
            return *this;
        }

        /**
         * @brief Compares the stored error code with another code.
         *
         * @param rhs Error code to compare.
         * @return True if both codes are equal.
         */
        [[nodiscard]] constexpr bool operator==(Code_t rhs) const noexcept
        {
            return m_Code == rhs;
        }

        /**
         * @brief Compares two errors by their code.
         *
         * Only the error code participates in equality — message, path, severity,
         * system error and trace are diagnostic context, not identity. This also
         * makes BasicError satisfy std::equality_comparable, which Result<T,E>
         * and Result<void,E> require for their own operator==.
         *
         * @param rhs Error to compare.
         * @return True if both errors have the same code.
         */
        [[nodiscard]] constexpr bool operator==(const BasicError& rhs) const noexcept
        {
            return m_Code == rhs.m_Code;
        }

        /**
         * @brief Returns the engine-specific error code.
         */
        [[nodiscard]] constexpr Code_t GetCode() const noexcept
        {
            return m_Code;
        }

        /**
         * @brief Returns the severity of the error.
         */
        [[nodiscard]] constexpr ErrorSeverity GetSeverity() const noexcept
        {
            return m_Severity;
        }

        /**
         * @brief Returns the optional error message.
         */
        [[nodiscard]] std::string_view GetMessage() const noexcept
        {
            return m_Message.View();
        }
        
        /**
         * @brief Returns the associated file path.
         */
        [[nodiscard]] std::string_view GetPath() const noexcept
        {
            return m_Path.View();
        }

        /**
         * @brief Returns the underlying operating system error.
         */
        [[nodiscard]] const SystemError_t& GetSystemError() const noexcept
        {
            return m_SystemError;
        }

        /**
         * @brief Returns the recorded stack trace.
         */
        [[nodiscard]] const Trace_t& GetTrace() const noexcept
        {
            return m_Trace;
        }

    private:
        /// Engine-specific error code.
        Code_t m_Code;

        /// Severity level.
        ErrorSeverity m_Severity = ErrorSeverity::Error;

        /// Optional message.
        NU_NO_UNIQUE_ADDRESS Message_t m_Message;

        /// Optional file path.
        NU_NO_UNIQUE_ADDRESS Path_t m_Path;

        /// Optional platform-specific error.
        NU_NO_UNIQUE_ADDRESS SystemError_t m_SystemError;

        /// Stack trace frames.
        Trace_t m_Trace;
    };
} // namespace NuEngine::Core

/**
 * @brief std::formatter specialization for BasicError.
 *
 * Enables std::format("{}", error).
 */
template <NuEngine::Core::ErrorCode CodeEnum, NuEngine::Core::ErrorPolicyConcept Policy>
struct std::formatter<NuEngine::Core::BasicError<CodeEnum, Policy>> : std::formatter<std::string>
{
    auto format(
        const NuEngine::Core::BasicError<CodeEnum, Policy>& error,
        std::format_context& ctx) const
    {
        return std::formatter<std::string>::format(
            NuEngine::Core::ErrorFormatter<NuEngine::Core::BasicError<CodeEnum, Policy>>::Format(error),
            ctx);
    }
};

/**
 * @brief Streams a formatted error to an output stream.
 *
 * Enables usage with std::cout, loggers and file streams.
 *
 * @param os Output stream.
 * @param error Error object.
 * @return Output stream.
 */
template <NuEngine::Core::ErrorCode CodeEnum, NuEngine::Core::ErrorPolicyConcept Policy>
std::ostream& operator<<(std::ostream& os, const NuEngine::Core::BasicError<CodeEnum, Policy>& error)
{
    return os << NuEngine::Core::ErrorFormatter<NuEngine::Core::BasicError<CodeEnum, Policy>>::Format(error);
}