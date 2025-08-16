#include <Core/Application.hpp>
#include <Math/Math.hpp>

#include <iostream>

int main() 
{
    NuEngine::Core::Application::Application app;
    app.Run();

    using namespace NuEngine::Math;

    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(3.0f, 2.0f, 1.0f);

    std::cout << "A + B = " << (a + b).ToString() << '\n';
    std::cout << "Dot = " << a.Dot(b) << '\n';
    std::cout << "Cross = " << a.Cross(b).ToString() << '\n';
    std::cout << "Normalized A = " << a.Normalize().ToString() << '\n';
    return 0;
}

