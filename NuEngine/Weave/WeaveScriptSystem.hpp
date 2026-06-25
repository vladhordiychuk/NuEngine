#pragma once

#include <Weave/WeaveComponent.hpp>
#include <Weave/NativeRegistry.hpp>
#include <cstring>
#include <cassert>

namespace NuEngine::Weave
{
    class WeaveScriptSystem
    {
    public:
        static void Update(WeaveComponent* components, const uint32_t* entityIds, size_t count, float dt, NuEngine::Runtime::Scene* scene)
        {
            assert(NativeRegistry::IsInitialized && "Call NativeRegistry::Initialize() first!");

            for (size_t i = 0; i < count; ++i)
            {
                WeaveComponent& comp = components[i];

                if (!comp.IsEnabled() || comp.IsSleeping()) continue;
                if (!comp.Asset || !comp.Asset->IsValid())  continue;

                comp.ResetFrame();
                ExecuteScript(comp, entityIds[i], dt, scene);
            }
        }

    private:
        static void ExecuteScript(WeaveComponent& comp, uint32_t entityId, float dt, NuEngine::Runtime::Scene* scene)
        {
            const uint8_t* code = comp.Asset->ByteCode.data();
            const size_t codeSize = comp.Asset->ByteCode.size();
            WeaveRegister* reg = comp.Registers.data();

            uint16_t& ip = comp.IP;

#define READ_BYTE()  (ip < codeSize ? code[ip++] : (uint8_t)OpCode::HALT)
#define READ_U16()   ([&]{ uint16_t v; std::memcpy(&v, &code[ip], 2); ip+=2; return v; }())
#define READ_U32()   ([&]{ uint32_t v; std::memcpy(&v, &code[ip], 4); ip+=4; return v; }())
#define READ_FLOAT() ([&]{ float    v; std::memcpy(&v, &code[ip], 4); ip+=4; return v; }())

            while (ip < codeSize)
            {
                OpCode op = static_cast<OpCode>(READ_BYTE());

                switch (op)
                {
                case OpCode::HALT:
                    return;

                case OpCode::LOAD_CONST_F:
                {
                    uint8_t dst = READ_BYTE();
                    reg[dst].f = READ_FLOAT();
                    break;
                }

                case OpCode::ADD_F:
                {
                    uint8_t a = READ_BYTE(), b = READ_BYTE(), dst = READ_BYTE();
                    reg[dst].f = reg[a].f + reg[b].f;
                    break;
                }

                case OpCode::CALL_EXTERNAL:
                {
                    uint32_t funcId = READ_U32();
                    uint8_t argCount = READ_BYTE();

                    NativeCallContext ctx{};
                    ctx.EntityId = entityId;
                    ctx.DeltaTime = dt;
                    ctx.Registers = reg;
                    ctx.ArgCount = argCount;
                    ctx.CurrentScene = scene;

                    for (uint8_t a = 0; a < argCount && a < 4; ++a)
                        ctx.ArgRegs[a] = READ_BYTE();

                    if (argCount > 0 || funcId == NativeFuncId::GetDeltaTime)
                        ctx.ReturnReg = READ_BYTE();

                    NativeRegistry::Call(funcId, ctx);
                    break;
                }

                case OpCode::SIN_F:
                {
                    uint8_t src = READ_BYTE();
                    uint8_t dst = READ_BYTE();
                    reg[dst].f = std::sin(reg[src].f);
                    break;
                }

                case OpCode::COS_F:
                {
                    uint8_t src = READ_BYTE();
                    uint8_t dst = READ_BYTE();
                    reg[dst].f = std::cos(reg[src].f);
                    break;
                }

                case OpCode::CMP_LT_F:
                {
                    uint8_t a = READ_BYTE();
                    uint8_t b = READ_BYTE();
                    uint8_t dst = READ_BYTE();
                    reg[dst].i = (reg[a].f < reg[b].f) ? 1 : 0;
                    break;
                }

                case OpCode::JUMP:
                {
                    uint16_t target = READ_U16();
                    ip = target;
                    break;
                }

                case OpCode::JUMP_IF_TRUE:
                {
                    uint8_t cond = READ_BYTE();
                    uint16_t target = READ_U16();

                    if (reg[cond].i != 0)
                    {
                        ip = target;
                    }
                    break;
                }

                case OpCode::JUMP_IF_FALSE:
                {
                    uint8_t cond = READ_BYTE();
                    uint16_t target = READ_U16();

                    if (reg[cond].i == 0)
                    {
                        ip = target;
                    }
                    break;
                }

                default:
                    return;
                }
            }
#undef READ_BYTE
#undef READ_U16
#undef READ_U32
#undef READ_FLOAT
        }
    };
}