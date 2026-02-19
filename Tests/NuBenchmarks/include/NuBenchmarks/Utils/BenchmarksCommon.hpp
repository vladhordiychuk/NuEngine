// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <benchmark/benchmark.h>
#include <NuEngine/Core/Memory/AlignedAllocator.hpp>
#include <NuMath/Algebra/Vector/VectorAPI.hpp>
#include <NuMath/Core/StorageTypes.hpp>

#include <random>

namespace NuEngine::Benchmarks
{
    struct FastRNG
    {
        uint32_t state = 0x811C9DC5;
        inline float NextFloat()
        {
            state ^= state << 13;
            state ^= state >> 17;
            state ^= state << 5;
            return static_cast<float>(state) * 2.3283064365386963e-10f;
        }
    };

    template <typename T>
    inline T MakeTestValue(FastRNG& rng);

    template <>
    inline NuMath::NuVecStorage4 MakeTestValue<NuMath::NuVecStorage4>(FastRNG& rng) {
        return { rng.NextFloat(), rng.NextFloat(), rng.NextFloat(), rng.NextFloat() };
    }

    inline void ReportThroughput(benchmark::State& state, size_t count, size_t bytesPerOp) {
        state.SetItemsProcessed(state.iterations() * count);
        state.SetBytesProcessed(state.iterations() * count * bytesPerOp);
    }

    // =========================================================================
    // SoA Benchmarks
    // =========================================================================

    template <typename SoAFunc>
    void BM_SoA_Op(benchmark::State& state, SoAFunc func)
    {
        const size_t count = state.range(0);
        FastRNG rng;

        AlignedVector<float> storage(count * 8);
        AlignedVector<float> results(count * 4);

        for (auto& v : storage) v = rng.NextFloat();

        float* ax = storage.data() + count * 0; float* ay = storage.data() + count * 1;
        float* az = storage.data() + count * 2; float* aw = storage.data() + count * 3;
        float* bx = storage.data() + count * 4; float* by = storage.data() + count * 5;
        float* bz = storage.data() + count * 6; float* bw = storage.data() + count * 7;

        float* rx = results.data() + count * 0; float* ry = results.data() + count * 1;
        float* rz = results.data() + count * 2; float* rw = results.data() + count * 3;

        NuMath::SoAVec4Const viewA = { ax, ay, az, aw };
        NuMath::SoAVec4Const viewB = { bx, by, bz, bw };
        NuMath::SoAVec4 viewR = { rx, ry, rz, rw };

        for (auto _ : state)
        {
            func(viewR, viewA, viewB, count);
            benchmark::ClobberMemory();
        }
        ReportThroughput(state, count, 16 * 3);
    }

    template <typename SoAFunc>
    void BM_SoA_ScalarRet_Op(benchmark::State& state, SoAFunc func)
    {
        const size_t count = state.range(0);
        FastRNG rng;
        AlignedVector<float> storage(count * 8);
        AlignedVector<float> results(count);

        for (auto& v : storage) v = rng.NextFloat();

        float* ax = storage.data();              float* ay = storage.data() + count;
        float* az = storage.data() + count * 2;  float* aw = storage.data() + count * 3;
        float* bx = storage.data() + count * 4;  float* by = storage.data() + count * 5;
        float* bz = storage.data() + count * 6;  float* bw = storage.data() + count * 7;

        NuMath::SoAVec4Const viewA = { ax, ay, az, aw };
        NuMath::SoAVec4Const viewB = { bx, by, bz, bw };

        for (auto _ : state) {
            func(results.data(), viewA, viewB, count);
            benchmark::ClobberMemory();
        }
        ReportThroughput(state, count, 36);
    }

    // =========================================================================
    // Batch (AoS) Benchmarks
    // =========================================================================
    template <typename StorageT, typename BatchFunc>
    void BM_Batch_Op(benchmark::State& state, BatchFunc func)
    {
        const size_t count = state.range(0);
        FastRNG rng;
        AlignedVector<StorageT> A(count), B(count), Result(count);
        for (size_t i = 0; i < count; ++i) {
            A[i] = MakeTestValue<StorageT>(rng);
            B[i] = MakeTestValue<StorageT>(rng);
        }

        for (auto _ : state) {
            func(Result.data(), A.data(), B.data(), count);
            benchmark::ClobberMemory();
        }
        ReportThroughput(state, count, sizeof(StorageT) * 3);
    }

    template <typename StorageT, typename BatchFunc>
    void BM_Batch_ScalarRet_Op(benchmark::State& state, BatchFunc func)
    {
        const size_t count = state.range(0);
        FastRNG rng;
        AlignedVector<StorageT> A(count), B(count);
        AlignedVector<float> Result(count);
        for (size_t i = 0; i < count; ++i) {
            A[i] = MakeTestValue<StorageT>(rng);
            B[i] = MakeTestValue<StorageT>(rng);
        }

        for (auto _ : state) {
            func(Result.data(), A.data(), B.data(), count);
            benchmark::ClobberMemory();
        }
        ReportThroughput(state, count, sizeof(StorageT) * 2 + sizeof(float));
    }

    // =========================================================================
    // Array (Iterative AoS) Benchmarks - ДОДАНО
    // =========================================================================
    template <typename T, typename Func>
    void BM_Array_BinaryOp(benchmark::State& state, Func func)
    {
        const size_t count = state.range(0);
        FastRNG rng;
        AlignedVector<T> A(count), B(count), R(count);
        for (size_t i = 0; i < count; ++i) { A[i] = MakeTestValue<T>(rng); B[i] = MakeTestValue<T>(rng); }

        for (auto _ : state) {
            for (size_t i = 0; i < count; ++i) {
                R[i] = func(A[i], B[i]);
            }
            benchmark::ClobberMemory();
        }
        ReportThroughput(state, count, sizeof(T) * 3);
    }

    template <typename T, typename Func>
    void BM_Array_ScalarRet_Op(benchmark::State& state, Func func)
    {
        const size_t count = state.range(0);
        FastRNG rng;
        AlignedVector<T> A(count), B(count);
        AlignedVector<float> R(count);
        for (size_t i = 0; i < count; ++i) { A[i] = MakeTestValue<T>(rng); B[i] = MakeTestValue<T>(rng); }

        for (auto _ : state) {
            for (size_t i = 0; i < count; ++i) {
                R[i] = func(A[i], B[i]);
            }
            benchmark::ClobberMemory();
        }
        ReportThroughput(state, count, sizeof(T) * 2 + sizeof(float));
    }

    template <typename T, typename Func>
    void BM_Array_UnaryOp(benchmark::State& state, Func func)
    {
        const size_t count = state.range(0);
        FastRNG rng;

        AlignedVector<T> A(count), R(count);

        for (size_t i = 0; i < count; ++i) {
            A[i] = MakeTestValue<T>(rng);
        }

        for (auto _ : state) {
            for (size_t i = 0; i < count; ++i) {
                R[i] = func(A[i]);
            }
            benchmark::ClobberMemory();
        }

        ReportThroughput(state, count, sizeof(T) * 2);
    }

    // =========================================================================
    // Single / Scalar Benchmarks (Latency)
    // =========================================================================
    template <typename T, typename Func>
    void BM_Single_BinaryOp(benchmark::State& state, Func func)
    {
        FastRNG rng;
        T a = MakeTestValue<T>(rng);
        T b = MakeTestValue<T>(rng);
        benchmark::DoNotOptimize(a); benchmark::DoNotOptimize(b);

        for (auto _ : state) {
            auto res = func(a, b);
            benchmark::DoNotOptimize(res);
        }
    }

    template <typename T, typename Func>
    void BM_Single_AssignOp(benchmark::State& state, Func func)
    {
        FastRNG rng;
        T a = MakeTestValue<T>(rng);
        T b = MakeTestValue<T>(rng);
        benchmark::DoNotOptimize(a); benchmark::DoNotOptimize(b);

        for (auto _ : state) {
            T tmp = a;
            func(tmp, b);
            benchmark::DoNotOptimize(tmp);
        }
    }

    template <typename T, typename Func>
    void BM_Single_UnaryOp(benchmark::State& state, Func func)
    {
        FastRNG rng;
        T a = MakeTestValue<T>(rng);
        benchmark::DoNotOptimize(a);

        for (auto _ : state) {
            auto res = func(a);
            benchmark::DoNotOptimize(res);
        }
    }

    template <typename T, typename Func>
    void BM_Single_ScalarOp(benchmark::State& state, Func func)
    {
        FastRNG rng;
        T a = MakeTestValue<T>(rng);
        float b = rng.NextFloat();
        benchmark::DoNotOptimize(a); benchmark::DoNotOptimize(b);

        for (auto _ : state) {
            auto res = func(a, b);
            benchmark::DoNotOptimize(res);
        }
    }

    template <typename T, typename Func>
    void BM_Single_ScalarAssignOp(benchmark::State& state, Func func)
    {
        FastRNG rng;
        T a = MakeTestValue<T>(rng);
        float b = rng.NextFloat();
        benchmark::DoNotOptimize(a); benchmark::DoNotOptimize(b);

        for (auto _ : state) {
            T tmp = a;
            func(tmp, b);
            benchmark::DoNotOptimize(tmp);
        }
    }
}