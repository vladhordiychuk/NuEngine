#pragma once

#include <Weave/WeaveTypes.hpp>
#include <vector>
#include <array>
#include <string>

namespace NuEngine::Weave
{
    struct WeaveGraphAsset
    {
        std::vector<uint8_t> ByteCode;
        uint32_t Checksum = 0;

        bool IsValid() const { return !ByteCode.empty(); }
    };

    struct WeaveComponent
    {
        const WeaveGraphAsset* Asset = nullptr;

        std::array<WeaveRegister, k_RegisterCount> Registers = {};

        uint16_t IP = 0;
        uint8_t Flags = 0;
        uint8_t _Padding = 0;

        static constexpr uint8_t FLAG_ENABLED = 0x01;
        static constexpr uint8_t FLAG_SLEEPING = 0x02;

        bool IsEnabled()  const { return (Flags & FLAG_ENABLED) != 0; }
        bool IsSleeping() const { return (Flags & FLAG_SLEEPING) != 0; }

        void Enable() { Flags |= FLAG_ENABLED; }
        void Disable() { Flags &= ~FLAG_ENABLED; }
        void Sleep() { Flags |= FLAG_SLEEPING; }
        void WakeUp() { Flags &= ~FLAG_SLEEPING; }

        void ResetFrame() { IP = 0; }
    };
}