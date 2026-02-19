// Copyright(c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <vector>
#include <new>
#include <limits>
#include <cstdlib>

#if defined(_MSC_VER)
    #include <malloc.h>
#endif

namespace NuEngine::Core
{
    /**
     * @brief Allocator that guarantees memory alignment.
     * Default alignment is 32 bytes (compatible with AVX/AVX2 registers).
     */
    template <typename T, size_t Alignment = 32>
    class AlignedAllocator
    {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using propagate_on_container_move_assignment = std::true_type;

        AlignedAllocator() noexcept = default;

        template <typename U>
        AlignedAllocator(const AlignedAllocator<U, Alignment>&) noexcept {}

        [[nodiscard]] T* allocate(size_t n)
        {
            if (n > (std::numeric_limits<size_t>::max)() / sizeof(T))
            {
                throw std::bad_alloc();
            }

            size_t bytes = n * sizeof(T);
            void* ptr = nullptr;

#if defined(_MSC_VER)
            ptr = _aligned_malloc(bytes, Alignment);
#else
            if (posix_memalign(&ptr, Alignment, bytes) != 0)
            {
                ptr = nullptr;
            }
#endif
            if (!ptr)
            {
                throw std::bad_alloc();
            }

            return static_cast<T*>(ptr);
        }

        void deallocate(T* p, size_t) noexcept
        {
#if defined(_MSC_VER)
            _aligned_free(p);
#else
            free(p);
#endif
        }

        template <typename U>
        struct rebind
        {
            using other = AlignedAllocator<U, Alignment>;
        };

        [[nodiscard]] bool operator==(const AlignedAllocator&) const noexcept
        {
            return true;
        }

        [[nodiscard]] bool operator!=(const AlignedAllocator& other) const noexcept
        {
            return !(*this == other);
        }
    };
}

namespace NuEngine
{
    /**
     * @brief Flexible vector with memory alignment.
     * Usage:
     * NuEngine::AlignedVector<float>     -> alignment 16 bytes (Default, for SSE)
     * NuEngine::AlignedVector<float, 32> -> alignment 32 bytes (for AVX)
     */
    template <typename T, size_t Alignment = 16>
    using AlignedVector = std::vector<T, NuEngine::Core::AlignedAllocator<T, Alignment>>;
}