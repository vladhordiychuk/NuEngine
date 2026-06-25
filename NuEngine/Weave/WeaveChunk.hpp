#pragma once

#include <vector>
#include <cstdint>

#include <Weave/WeaveTypes.hpp>
#include <Weave/WeaveComponent.hpp>

namespace NuEngine::Weave
{
    struct alignas(64) WeaveChunk
    {
        static constexpr int k_Capacity = 64;

        const WeaveGraphAsset* Asset = nullptr;

        alignas(64) float Regs_F[k_RegisterCount][k_Capacity] = {};
        alignas(64) int32_t Regs_I[k_RegisterCount][k_Capacity] = {};

        uint32_t EntityIds[k_Capacity] = {};
        int Count = 0;

        bool IsFull() const 
        { 
            return Count >= k_Capacity; 
        }

        void Add(uint32_t entityId)
        {
            if (!IsFull())
            {
                EntityIds[Count++] = entityId;
            }
        }
    };

    struct WeavePoolManager
    {
        const WeaveGraphAsset* Asset = nullptr;
        std::vector<WeaveChunk> Chunks;

        explicit WeavePoolManager(const WeaveGraphAsset* asset = nullptr)
            : Asset(asset)
        {
        }

        void Add(uint32_t entityId)
        {
            for (auto& chunk : Chunks)
            {
                if (!chunk.IsFull())
                {
                    chunk.Add(entityId);
                    return;
                }
            }

            Chunks.emplace_back();
            Chunks.back().Asset = Asset;
            Chunks.back().Add(entityId);
        }
    };
}