// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <memory>
#include <type_traits>
#include <utility>
#include <Core/Platform/CompilerMacros.hpp>
#include <Core/Types/Result/ResultFwd.hpp>
#include <Core/Types/Result/ResultWrappers.hpp>

namespace NuEngine::Core::Detail
{
    template <typename T, typename E, bool>
    struct ResultStorage
    {
        static_assert(std::is_trivially_copyable_v<T>&& std::is_trivially_copyable_v<E>);

        union
        {
            T ok;
            E err;
        };
        bool isOk;

        NU_FORCEINLINE constexpr ResultStorage(OkWrapper<T>&& w) noexcept
            : ok(std::move(w.value)), isOk(true)
        {
        }

        NU_FORCEINLINE constexpr ResultStorage(ErrWrapper<E>&& w) noexcept
            : err(std::move(w.error)), isOk(false)
        {
        }

        template <typename... Args>
        NU_FORCEINLINE constexpr ResultStorage(std::in_place_t, Args&&... args)
            noexcept(std::is_nothrow_constructible_v<T, Args...>)
            : ok(std::forward<Args>(args)...), isOk(true)
        {
        }

        template <typename... Args>
        NU_FORCEINLINE constexpr ResultStorage(InPlaceErr_t, Args&&... args)
            noexcept(std::is_nothrow_constructible_v<E, Args...>)
            : err(std::forward<Args>(args)...), isOk(false)
        {
        }

        [[nodiscard]] NU_FORCEINLINE constexpr bool IsOk() const noexcept
        {
            return isOk;
        }

        [[nodiscard]] NU_FORCEINLINE constexpr T& Ok_() & noexcept 
        { 
            return ok; 
        }
        [[nodiscard]] NU_FORCEINLINE constexpr const T& Ok_() const& noexcept 
        { 
            return ok; 
        }
        [[nodiscard]] NU_FORCEINLINE constexpr T&& Ok_() && noexcept 
        { 
            return std::move(ok); 
        }

        [[nodiscard]] NU_FORCEINLINE constexpr E& Err_() & noexcept 
        { 
            return err; 
        }
        [[nodiscard]] NU_FORCEINLINE constexpr const E& Err_() const& noexcept 
        { 
            return err; 
        }
        [[nodiscard]] NU_FORCEINLINE constexpr E&& Err_() && noexcept 
        { 
            return std::move(err); 
        }
    };

    template <typename T, typename E>
    struct ResultStorage<T, E, false>
    {
        union Storage
        {
            T ok;
            E err;

            constexpr Storage() noexcept {}
            constexpr ~Storage() noexcept {}
        } m_data;
        bool m_isOk;

        NU_FORCEINLINE constexpr ResultStorage(OkWrapper<T>&& w) noexcept(std::is_nothrow_move_constructible_v<T>)
            : m_isOk(true)
        {
            std::construct_at(std::addressof(m_data.ok), std::move(w.value));
        }
        NU_FORCEINLINE constexpr ResultStorage(ErrWrapper<E>&& w) noexcept(std::is_nothrow_move_constructible_v<E>)
            : m_isOk(false)
        {
            std::construct_at(std::addressof(m_data.err), std::move(w.error));
        }

        template <typename... Args>
        NU_FORCEINLINE constexpr ResultStorage(std::in_place_t, Args&&... args)
            noexcept(std::is_nothrow_constructible_v<T, Args...>)
            : m_isOk(true)
        {
            std::construct_at(std::addressof(m_data.ok), std::forward<Args>(args)...);
        }

        template <typename... Args>
        NU_FORCEINLINE constexpr ResultStorage(InPlaceErr_t, Args&&... args)
            noexcept(std::is_nothrow_constructible_v<E, Args...>)
            : m_isOk(false)
        {
            std::construct_at(std::addressof(m_data.err), std::forward<Args>(args)...);
        }

        NU_FORCEINLINE constexpr ResultStorage(const ResultStorage& other)
            noexcept(std::is_nothrow_copy_constructible_v<T>&& std::is_nothrow_copy_constructible_v<E>)
            : m_isOk(other.m_isOk)
        {
            if (m_isOk)
            {
                std::construct_at(std::addressof(m_data.ok), other.m_data.ok);
            }
            else
            {
                std::construct_at(std::addressof(m_data.err), other.m_data.err);
            }
        }

        NU_FORCEINLINE constexpr ResultStorage(ResultStorage&& other)
            noexcept(std::is_nothrow_move_constructible_v<T>&& std::is_nothrow_move_constructible_v<E>)
            : m_isOk(other.m_isOk)
        {
            if (m_isOk)
            {
                std::construct_at(std::addressof(m_data.ok), std::move(other.m_data.ok));
            }
            else
            {
                std::construct_at(std::addressof(m_data.err), std::move(other.m_data.err));
            }
        }

        NU_FORCEINLINE constexpr ResultStorage& operator=(const ResultStorage& other)
            noexcept(std::is_nothrow_copy_constructible_v<T>&& std::is_nothrow_copy_constructible_v<E>&&
                std::is_nothrow_copy_assignable_v<T>&& std::is_nothrow_copy_assignable_v<E>)
        {
            if (this == &other)
            {
                return *this;
            }

            if (m_isOk && other.m_isOk)
            {
                m_data.ok = other.m_data.ok;
            }
            else if (!m_isOk && !other.m_isOk)
            {
                m_data.err = other.m_data.err;
            }
            else
            {
                if (other.m_isOk)
                {
                    T temp(other.m_data.ok);
                    Destroy();
                    m_isOk = true;
                    std::construct_at(std::addressof(m_data.ok), std::move(temp));
                }
                else
                {
                    E temp(other.m_data.err);
                    Destroy();
                    m_isOk = false;
                    std::construct_at(std::addressof(m_data.err), std::move(temp));
                }
            }
            return *this;
        }

        NU_FORCEINLINE constexpr ResultStorage& operator=(ResultStorage&& other)
            noexcept(std::is_nothrow_move_constructible_v<T>&& std::is_nothrow_move_constructible_v<E>&&
                std::is_nothrow_move_assignable_v<T>&& std::is_nothrow_move_assignable_v<E>)
        {
            if (this == &other)
            {
                return *this;
            }

            if (m_isOk && other.m_isOk)
            {
                m_data.ok = std::move(other.m_data.ok);
            }
            else if (!m_isOk && !other.m_isOk)
            {
                m_data.err = std::move(other.m_data.err);
            }
            else
            {
                if (other.m_isOk)
                {
                    T temp(std::move(other.m_data.ok));
                    Destroy();
                    m_isOk = true;
                    std::construct_at(std::addressof(m_data.ok), std::move(temp));
                }
                else
                {
                    E temp(std::move(other.m_data.err));
                    Destroy();
                    m_isOk = false;
                    std::construct_at(std::addressof(m_data.err), std::move(temp));
                }
            }
            return *this;
        }

        constexpr ~ResultStorage()
        {
            Destroy();
        }

        NU_FORCEINLINE constexpr void Destroy() noexcept
        {
            if (m_isOk)
            {
                std::destroy_at(std::addressof(m_data.ok));
            }
            else
            {
                std::destroy_at(std::addressof(m_data.err));
            }
        }

        [[nodiscard]] NU_FORCEINLINE constexpr bool IsOk() const noexcept
        {
            return m_isOk;
        }

        [[nodiscard]] NU_FORCEINLINE constexpr T& Ok_() & noexcept 
        { 
            return m_data.ok; 
        }
        [[nodiscard]] NU_FORCEINLINE constexpr const T& Ok_() const& noexcept 
        { 
            return m_data.ok; 
        }
        [[nodiscard]] NU_FORCEINLINE constexpr T&& Ok_() && noexcept 
        { 
            return std::move(m_data.ok); 
        }

        [[nodiscard]] NU_FORCEINLINE constexpr E& Err_() & noexcept 
        { 
            return m_data.err; 
        }
        [[nodiscard]] NU_FORCEINLINE constexpr const E& Err_() const& noexcept 
        { 
            return m_data.err; 
        }
        [[nodiscard]] NU_FORCEINLINE constexpr E&& Err_() && noexcept 
        { 
            return std::move(m_data.err); 
        }
    };
} // namespace NuEngine::Core::Detail
