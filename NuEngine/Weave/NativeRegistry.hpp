#pragma once

#include <Weave/WeaveTypes.hpp>
#include <Core/Logging/Logger.hpp>
#include <NuEngine/Core/API.hpp>

namespace NuEngine::Runtime { class Scene; }

namespace NuEngine::Weave
{
    struct NativeCallContext
    {
        uint32_t  EntityId;
        float     DeltaTime;

        WeaveRegister* Registers;

        float (*Regs_F)[64] = nullptr;
        int32_t(*Regs_I)[64] = nullptr;
        int ChunkIndex = 0;

        uint8_t   ArgRegs[4];
        uint8_t   ReturnReg;
        uint8_t   ArgCount;

        NuEngine::Runtime::Scene* CurrentScene = nullptr;

        float GetFloat(uint8_t reg) const
        {
            return Regs_F ? Regs_F[reg][ChunkIndex] : Registers[reg].f;
        }

        int32_t GetInt(uint8_t reg) const
        {
            return Regs_I ? Regs_I[reg][ChunkIndex] : Registers[reg].i;
        }

        void SetFloat(uint8_t reg, float val)
        {
            if (Regs_F) 
            {
                Regs_F[reg][ChunkIndex] = val;
            }
            else 
            {
                Registers[reg].f = val;
            }
        }

        void SetUInt(uint8_t reg, uint32_t val)
        {
            if (Regs_I) 
            {
                Regs_I[reg][ChunkIndex] = static_cast<int32_t>(val);
            }
            else
            {
                Registers[reg].u = val;
            }
        }
    };

    using NativeFuncSignature = void(*)(NativeCallContext&);

    class NU_API NativeRegistry
    {
    public:
        static inline NativeFuncSignature Functions[256] = { nullptr };
        static inline bool IsInitialized = false;

        static void Initialize();
        static void Call(uint32_t funcId, NativeCallContext& ctx)
        {
            if (funcId < 256 && Functions[funcId])
            {
                Functions[funcId](ctx);
            }
        }
    };
}