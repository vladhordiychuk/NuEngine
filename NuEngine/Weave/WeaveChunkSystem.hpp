#pragma once
#include <NuEngine/Weave/WeaveChunk.hpp>
#include <NuEngine/Weave/NativeRegistry.hpp>
#include <cstring>
#include <cmath>

namespace NuEngine::Runtime 
{ 
    class Scene; 
}

namespace NuEngine::Weave
{
    class WeaveChunkSystem
    {
    public:
        static void UpdateAll(WeavePoolManager& manager, float dt, NuEngine::Runtime::Scene* scene)
        {
            if (!manager.Asset || manager.Asset->ByteCode.empty())
            {
                return;
            }

            const uint8_t* code = manager.Asset->ByteCode.data();
            const size_t size = manager.Asset->ByteCode.size();

            for (auto& chunk : manager.Chunks)
            {
                if (chunk.Count == 0) 
                {
                    continue;
                }

                uint16_t ip = 0;

                auto readByte = [&]() { 
                    return ip < size ? code[ip++] : 0; 
                    };

                auto readF32 = [&]() { 
                    float v; std::memcpy(&v, &code[ip], 4); ip += 4; return v; 
                    };

                while (ip < size)
                {
                    OpCode op = static_cast<OpCode>(readByte());

                    switch (op)
                    {
                    case OpCode::HALT:
                        ip = size;
                        break;

                    case OpCode::LOAD_CONST_F:
                    {
                        uint8_t dst = readByte();
                        float val = readF32();

                        for (int i = 0; i < chunk.Count; ++i)
                        {
                            chunk.Regs_F[dst][i] = val;
                        }

                        break;
                    }

                    case OpCode::MUL_F:
                    {
                        uint8_t a = readByte(), b = readByte(), dst = readByte();

                        for (int i = 0; i < chunk.Count; ++i)
                        {
                            chunk.Regs_F[dst][i] = chunk.Regs_F[a][i] * chunk.Regs_F[b][i];
                        }

                        break;
                    }

                    case OpCode::SIN_F:
                    {
                        uint8_t src = readByte(), dst = readByte();
                        for (int i = 0; i < chunk.Count; ++i)
                        {
                            chunk.Regs_F[dst][i] = std::sin(chunk.Regs_F[src][i]);
                        }

                        break;
                    }

                    case OpCode::COS_F:
                    {
                        uint8_t src = readByte(), dst = readByte();
                        #pragma omp simd
                        {
                            for (int i = 0; i < chunk.Count; ++i)
                                chunk.Regs_F[dst][i] = std::cos(chunk.Regs_F[src][i]);
                        }
                        break;
                    }

                    default: break;
                    }
                }
            }
        }
    };
}