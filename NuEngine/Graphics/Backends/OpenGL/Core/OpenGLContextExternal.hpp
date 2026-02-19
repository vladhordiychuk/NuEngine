#pragma once

namespace NuEngine::Graphics::OpenGL
{
    class OpenGLExternalContext : public OpenGLContext
    {
    public:
        // Нічого не робимо при ініціалізації, бо Qt вже все зробив
        Core::Result<void, GraphicsError> Initialize() noexcept override
        {
            // Можна додати перевірку, чи завантажений GLAD, але в Qt ми це вже зробили
            return Core::Ok();
        }

        // Нічого не свапаємо, бо Qt робить це сам
        Core::Result<void, GraphicsError> SwapBuffers() noexcept override
        {
            return Core::Ok();
        }

        // Нічого не робимо, бо контекст Qt вже активний
        Core::Result<void, GraphicsError> MakeCurrent() noexcept override
        {
            return Core::Ok();
        }
    };
}