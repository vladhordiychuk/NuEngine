// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <chrono>
#include <Core/Types/Types.hpp>

namespace NuEngine::Core
{
    using NuFloat = NuEngine::Core::Types::NuFloat;

    /*
    * @brief Клас для вимірювання часу (дельта та загальний час)
    */
    class Timer
    {
    public:
        Timer();

        /*
        * @brief Скидає таймер (обновлює старт і останній час)
        */
        void Reset();

        /*
        * @brief Повертає час між викликами (у секундах)
        */
        NuFloat GetDeltaTime();

        /*
        * @brief Повертає загальний час від Reset (у секундах)
        */
        NuFloat GetElapsedTime() const;

    private:
        std::chrono::high_resolution_clock::time_point m_startTime;
        std::chrono::high_resolution_clock::time_point m_lastTime;
    };
}
