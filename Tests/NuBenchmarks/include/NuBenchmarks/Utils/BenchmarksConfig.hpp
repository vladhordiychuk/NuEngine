// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <benchmark/benchmark.h>

#ifndef BENCH_ARRAY_COUNT
    #define BENCH_ARRAY_COUNT 10000
#endif

#ifndef ENABLE_BATCH_BENCHMARKS
    #define ENABLE_BATCH_BENCHMARKS 1
#endif

#ifndef ENABLE_ARRAY_BENCHMARKS
    #define ENABLE_ARRAY_BENCHMARKS 1
#endif

#ifndef ENABLE_SOA_BENCHMARKS
    #define ENABLE_SOA_BENCHMARKS 1
#endif

#ifndef ENABLE_SINGLE_BENCHMARKS
    #define ENABLE_SINGLE_BENCHMARKS 1
#endif

#define IN_TIME_STR "1.0s"

#define BENCH_START 1024
#define BENCH_END   4000000

#define REGISTER_BATCH_BENCHMARK(StorageType, Name, ...) \
    benchmark::RegisterBenchmark(#StorageType "_Batch_" #Name, \
        [](benchmark::State& state){ NuEngine::Benchmarks::BM_Batch_Op<StorageType>(state, __VA_ARGS__); }) \
        ->Range(BENCH_START, BENCH_END)

#define REGISTER_BATCH_SCALAR_BENCHMARK(StorageType, Name, ...) \
    benchmark::RegisterBenchmark(#StorageType "_Batch_" #Name, \
        [](benchmark::State& state){ NuEngine::Benchmarks::BM_Batch_ScalarRet_Op<StorageType>(state, __VA_ARGS__); }) \
        ->Range(BENCH_START, BENCH_END)

#define REGISTER_SOA_BENCHMARK(Name, ...) \
    benchmark::RegisterBenchmark("Nu_SoA_" #Name, \
        [](benchmark::State& state){ NuEngine::Benchmarks::BM_SoA_Op(state, __VA_ARGS__); }) \
        ->Range(BENCH_START, BENCH_END)

#define REGISTER_SOA_SCALAR_BENCHMARK(Name, ...) \
    benchmark::RegisterBenchmark("Nu_SoA_" #Name, \
        [](benchmark::State& state){ NuEngine::Benchmarks::BM_SoA_ScalarRet_Op(state, __VA_ARGS__); }) \
        ->Range(BENCH_START, BENCH_END)

#define REGISTER_ARRAY_BENCHMARK(Type, Name, BenchFunc, ...) \
    benchmark::RegisterBenchmark(#Type "_Array_" #Name, \
        [](benchmark::State& state){ NuEngine::Benchmarks::BenchFunc<Type>(state, __VA_ARGS__); }) \
        ->Range(BENCH_START, BENCH_END)

#define REGISTER_ARRAY_SCALAR_BENCHMARK(Type, Name, BenchFunc, ...) \
    benchmark::RegisterBenchmark(#Type "_Array_" #Name, \
        [](benchmark::State& state){ NuEngine::Benchmarks::BenchFunc<Type>(state, __VA_ARGS__); }) \
        ->Range(BENCH_START, BENCH_END)

#define REGISTER_SINGLE_BENCHMARK(Type, Name, BenchFunc, ...) \
    benchmark::RegisterBenchmark(#Type "_Single_" #Name, \
        [](benchmark::State& state){ NuEngine::Benchmarks::BenchFunc<Type>(state, __VA_ARGS__); })