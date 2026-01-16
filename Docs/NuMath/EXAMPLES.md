<div align="center">

# 🎯 NuMath Code Examples

**[🏠 Home](../../NuMath/README.md)** • **[⚙️ Installation](INSTALL.md)** • **[📖 API Reference](API.md)** • **[📊 Benchmarks](benchmarks.html)**

</div>

---

## 📑 Table of Contents

1. [Game Development](#1-game-development)
   - [Entity Transform System](#entity-transform-system)
   - [Camera System](#camera-system)
   - [Particle System](#particle-system)
2. [Graphics Programming](#2-graphics-programming)
   - [MVP Matrix Pipeline](#mvp-matrix-pipeline)
   - [Ray Casting](#ray-casting)
   - [Mesh Transformations](#mesh-transformations)
3. [Physics Simulation](#3-physics-simulation)
   - [Projectile Motion](#projectile-motion)
   - [Collision Detection](#collision-detection)
4. [Color & Post-Processing](#4-color--post-processing)
   - [UI Theming](#ui-theming)
   - [HDR Tone Mapping](#hdr-tone-mapping)
   - [Color Grading](#color-grading)
5. [Batch Processing](#5-batch-processing)
   - [Particle System (SoA)](#particle-system-soa)
   - [Physics Integration](#physics-integration)

---

## 1. Game Development

### Entity Transform System

**ECS (Entity Component System) with NuMath**

```cpp
#include <Math/NuMath.hpp>
#include <vector>

using namespace NuMath;

// Component structures
struct Transform {
    Vector3 position = Vector3::Zero();
    Vector3 rotation = Vector3::Zero();  // Euler angles (will be Quaternion in v0.2)
    Vector3 scale = Vector3::One();
};

struct Velocity {
    Vector3 linear = Vector3::Zero();
    Vector3 angular = Vector3::Zero();
};

struct Entity {
    Transform transform;
    Velocity velocity;
    // ... other components
};

// System: Update all entities
void UpdateTransforms(std::vector<Entity>& entities, float deltaTime) {
    for (auto& entity : entities) {
        // SIMD-optimized position update
        entity.transform.position += entity.velocity.linear * deltaTime;
        
        // Rotation update (Euler angles)
        entity.transform.rotation += entity.velocity.angular * deltaTime;
    }
}

// System: Build transformation matrices
void BuildTransformMatrices(const std::vector<Entity>& entities, 
                           std::vector<Matrix4x4>& outMatrices) {
    outMatrices.resize(entities.size());
    
    for (size_t i = 0; i < entities.size(); ++i) {
        const auto& t = entities[i].transform;
        
        // Build TRS matrix
        Matrix4x4 T = Matrix4x4::CreateTranslation(t.position);
        Matrix4x4 Rx = Matrix4x4::CreateRotationX(t.rotation.X());
        Matrix4x4 Ry = Matrix4x4::CreateRotationY(t.rotation.Y());
        Matrix4x4 Rz = Matrix4x4::CreateRotationZ(t.rotation.Z());
        Matrix4x4 S = Matrix4x4::CreateScale(t.scale);
        
        // Combine: Model = T * R * S
        outMatrices[i] = T * Rz * Ry * Rx * S;
    }
}

// Usage
int main() {
    std::vector<Entity> entities(1000);
    std::vector<Matrix4x4> modelMatrices;
    
    // Game loop
    float deltaTime = 1.0f / 60.0f;  // 60 FPS
    
    while (true) {
        UpdateTransforms(entities, deltaTime);
        BuildTransformMatrices(entities, modelMatrices);
        
        // Upload to GPU...
        for (size_t i = 0; i < entities.size(); ++i) {
            // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrices[i].Data());
        }
    }
}
```

---

### Camera System

**First-Person & Third-Person Camera**

```cpp
#include <Math/NuMath.hpp>

using namespace NuMath;

class Camera {
public:
    // First-person camera
    void UpdateFirstPerson(const Vector3& position, float yaw, float pitch) {
        m_position = position;
        m_yaw = yaw;
        m_pitch = pitch;
        
        // Calculate forward direction
        Vector3 forward(
            cos(m_pitch) * cos(m_yaw),
            sin(m_pitch),
            cos(m_pitch) * sin(m_yaw)
        );
        m_forward = forward.Normalize();
        
        // Calculate right and up vectors
        m_right = Vector3::UnitY().Cross(m_forward).Normalize();
        m_up = m_forward.Cross(m_right);
    }
    
    // Third-person camera (orbiting around target)
    void UpdateThirdPerson(const Vector3& target, float distance, 
                          float yaw, float pitch) {
        // Calculate camera offset
        Vector3 offset(
            distance * cos(pitch) * cos(yaw),
            distance * sin(pitch),
            distance * cos(pitch) * sin(yaw)
        );
        
        m_position = target + offset;
        m_forward = (target - m_position).Normalize();
        m_right = Vector3::UnitY().Cross(m_forward).Normalize();
        m_up = m_forward.Cross(m_right);
    }
    
    // Get view matrix
    Matrix4x4 GetViewMatrix() const {
        return Matrix4x4::CreateLookAt(m_position, m_position + m_forward, m_up);
    }
    
    // Get projection matrix
    Matrix4x4 GetProjectionMatrix(float fov, float aspect, 
                                 float near, float far) const {
        return Matrix4x4::CreatePerspective(fov, aspect, near, far);
    }
    
    // Movement
    void MoveForward(float amount) { m_position += m_forward * amount; }
    void MoveRight(float amount) { m_position += m_right * amount; }
    void MoveUp(float amount) { m_position += m_up * amount; }

private:
    Vector3 m_position = Vector3(0, 0, 10);
    Vector3 m_forward = Vector3(0, 0, -1);
    Vector3 m_right = Vector3(1, 0, 0);
    Vector3 m_up = Vector3(0, 1, 0);
    float m_yaw = 0.0f;
    float m_pitch = 0.0f;
};

// Usage
int main() {
    Camera camera;
    
    // First-person controls
    float yaw = 0.0f, pitch = 0.0f;
    while (true) {
        // Input handling...
        // yaw += mouseX * sensitivity;
        // pitch += mouseY * sensitivity;
        
        camera.UpdateFirstPerson(Vector3(0, 1.7f, 0), yaw, pitch);
        
        Matrix4x4 view = camera.GetViewMatrix();
        Matrix4x4 proj = camera.GetProjectionMatrix(
            1.57f,        // 90° FOV
            16.0f / 9.0f, // 16:9 aspect ratio
            0.1f,         // Near plane
            1000.0f       // Far plane
        );
        
        Matrix4x4 VP = proj * view;
        // Upload VP to shaders...
    }
}
```

---

### Particle System

**Simple CPU particle system**

```cpp
#include <Math/NuMath.hpp>
#include <vector>
#include <algorithm>

using namespace NuMath;

struct Particle {
    Vector3 position;
    Vector3 velocity;
    Color color;
    float lifetime;
    float maxLifetime;
};

class ParticleEmitter {
public:
    void Emit(const Vector3& position, int count) {
        for (int i = 0; i < count; ++i) {
            Particle p;
            p.position = position;
            
            // Random velocity (simplified)
            p.velocity = Vector3(
                RandomRange(-1, 1),
                RandomRange(2, 5),
                RandomRange(-1, 1)
            );
            
            p.color = Color::FromSRGB(1, 0.8f, 0.2f);  // Fire color
            p.maxLifetime = RandomRange(1.0f, 3.0f);
            p.lifetime = p.maxLifetime;
            
            m_particles.push_back(p);
        }
    }
    
    void Update(float deltaTime) {
        const Vector3 gravity(0, -9.8f, 0);
        
        for (auto& p : m_particles) {
            // Physics
            p.velocity += gravity * deltaTime;
            p.position += p.velocity * deltaTime;
            
            // Fade out
            p.lifetime -= deltaTime;
            float alpha = p.lifetime / p.maxLifetime;
            
            // Color transition: yellow → orange → red
            Color start = Color::FromSRGB(1, 1, 0);     // Yellow
            Color end = Color::FromSRGB(1, 0, 0);       // Red
            p.color = Color::Lerp(start, end, 1.0f - alpha);
        }
        
        // Remove dead particles
        m_particles.erase(
            std::remove_if(m_particles.begin(), m_particles.end(),
                [](const Particle& p) { return p.lifetime <= 0; }),
            m_particles.end()
        );
    }
    
    const std::vector<Particle>& GetParticles() const { return m_particles; }

private:
    std::vector<Particle> m_particles;
    
    float RandomRange(float min, float max) {
        return min + (max - min) * (rand() / (float)RAND_MAX);
    }
};
```

---

## 2. Graphics Programming

### MVP Matrix Pipeline

**Model-View-Projection transformation**

```cpp
#include <Math/NuMath.hpp>

using namespace NuMath;

// Complete rendering pipeline
class RenderPipeline {
public:
    void SetupCamera(const Vector3& eye, const Vector3& target, 
                     float fov, float aspect) {
        m_view = Matrix4x4::CreateLookAt(eye, target, Vector3::UnitY());
        m_projection = Matrix4x4::CreatePerspective(fov, aspect, 0.1f, 1000.0f);
        m_viewProjection = m_projection * m_view;
    }
    
    Matrix4x4 GetMVP(const Vector3& position, const Vector3& scale, 
                     float rotationY) const {
        Matrix4x4 T = Matrix4x4::CreateTranslation(position);
        Matrix4x4 R = Matrix4x4::CreateRotationY(rotationY);
        Matrix4x4 S = Matrix4x4::CreateScale(scale);
        
        Matrix4x4 model = T * R * S;
        return m_viewProjection * model;
    }
    
    // Transform point from world to clip space
    Vector4 WorldToClip(const Vector3& worldPos) const {
        return m_viewProjection * Vector4(worldPos.X(), worldPos.Y(), 
                                          worldPos.Z(), 1.0f);
    }
    
    // Perspective divide (clip → NDC)
    Vector3 ClipToNDC(const Vector4& clipPos) const {
        float w = clipPos.W();
        return Vector3(clipPos.X() / w, clipPos.Y() / w, clipPos.Z() / w);
    }

private:
    Matrix4x4 m_view;
    Matrix4x4 m_projection;
    Matrix4x4 m_viewProjection;
};

// Usage
int main() {
    RenderPipeline pipeline;
    
    // Setup camera
    pipeline.SetupCamera(
        Vector3(0, 5, 10),   // Eye position
        Vector3(0, 0, 0),    // Look at origin
        1.57f,               // 90° FOV
        16.0f / 9.0f         // Aspect ratio
    );
    
    // Render object
    Vector3 objectPos(5, 0, 0);
    Vector3 objectScale(1, 2, 1);
    float rotation = 0.785f;  // 45°
    
    Matrix4x4 mvp = pipeline.GetMVP(objectPos, objectScale, rotation);
    
    // Upload to GPU
    // glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, mvp.Data());
}
```

---

### Ray Casting

**Ray-sphere and ray-plane intersection**

```cpp
#include <Math/NuMath.hpp>
#include <optional>

using namespace NuMath;

struct Ray {
    Vector3 origin;
    Vector3 direction;  // Must be normalized
};

struct Sphere {
    Vector3 center;
    float radius;
};

struct Plane {
    Vector3 point;
    Vector3 normal;  // Must be normalized
};

// Ray-Sphere Intersection
std::optional<float> IntersectRaySphere(const Ray& ray, const Sphere& sphere) {
    Vector3 oc = ray.origin - sphere.center;
    
    float a = ray.direction.Dot(ray.direction);
    float b = 2.0f * oc.Dot(ray.direction);
    float c = oc.Dot(oc) - sphere.radius * sphere.radius;
    
    float discriminant = b * b - 4 * a * c;
    
    if (discriminant < 0) {
        return std::nullopt;  // No intersection
    }
    
    // Return nearest intersection point
    float t = (-b - sqrt(discriminant)) / (2.0f * a);
    return t > 0 ? std::optional<float>(t) : std::nullopt;
}

// Ray-Plane Intersection
std::optional<float> IntersectRayPlane(const Ray& ray, const Plane& plane) {
    float denom = ray.direction.Dot(plane.normal);
    
    if (abs(denom) < 1e-6f) {
        return std::nullopt;  // Ray parallel to plane
    }
    
    Vector3 p0_origin = plane.point - ray.origin;
    float t = p0_origin.Dot(plane.normal) / denom;
    
    return t >= 0 ? std::optional<float>(t) : std::nullopt;
}

// Mouse Picking
Ray ScreenPointToRay(const Vector2& screenPos, const Vector2& screenSize,
                     const Matrix4x4& viewProj) {
    // Convert screen coords to NDC [-1, 1]
    float x = (2.0f * screenPos.X()) / screenSize.X() - 1.0f;
    float y = 1.0f - (2.0f * screenPos.Y()) / screenSize.Y();
    
    // Unproject (simplified - assumes inverse is available)
    Matrix4x4 invViewProj = viewProj.Inverse();
    
    Vector4 nearPoint = invViewProj * Vector4(x, y, -1, 1);
    Vector4 farPoint = invViewProj * Vector4(x, y, 1, 1);
    
    // Perspective divide
    Vector3 near(nearPoint.X() / nearPoint.W(), 
                 nearPoint.Y() / nearPoint.W(), 
                 nearPoint.Z() / nearPoint.W());
    Vector3 far(farPoint.X() / farPoint.W(), 
                farPoint.Y() / farPoint.W(), 
                farPoint.Z() / farPoint.W());
    
    Ray ray;
    ray.origin = near;
    ray.direction = (far - near).Normalize();
    return ray;
}
```

---

### Mesh Transformations

**Transforming vertex data**

```cpp
#include <Math/NuMath.hpp>
#include <vector>

using namespace NuMath;

struct Vertex {
    Vector3 position;
    Vector3 normal;
    Vector2 texCoord;
};

// Transform mesh vertices (CPU-side)
void TransformMesh(std::vector<Vertex>& vertices, const Matrix4x4& transform) {
    // Extract 3x3 for normals (inverse transpose, but we skip for simplicity)
    for (auto& vertex : vertices) {
        // Transform position
        Vector4 pos(vertex.position.X(), vertex.position.Y(), 
                    vertex.position.Z(), 1.0f);
        Vector4 transformedPos = transform * pos;
        
        vertex.position = Vector3(transformedPos.X(), transformedPos.Y(), 
                                 transformedPos.Z());
        
        // Transform normal (simplified - should use inverse transpose)
        Vector4 norm(vertex.normal.X(), vertex.normal.Y(), 
                     vertex.normal.Z(), 0.0f);
        Vector4 transformedNorm = transform * norm;
        
        vertex.normal = Vector3(transformedNorm.X(), transformedNorm.Y(), 
                               transformedNorm.Z()).Normalize();
    }
}

// Generate simple cube mesh
std::vector<Vertex> GenerateCube() {
    std::vector<Vertex> vertices;
    
    // Front face
    vertices.push_back({{-1, -1,  1}, { 0,  0,  1}, {0, 0}});
    vertices.push_back({{ 1, -1,  1}, { 0,  0,  1}, {1, 0}});
    vertices.push_back({{ 1,  1,  1}, { 0,  0,  1}, {1, 1}});
    // ... (add remaining faces)
    
    return vertices;
}
```

---

## 3. Physics Simulation

### Projectile Motion

**Ballistic trajectory with drag**

```cpp
#include <Math/NuMath.hpp>

using namespace NuMath;

class Projectile {
public:
    Projectile(const Vector3& position, const Vector3& velocity)
        : m_position(position), m_velocity(velocity) {}
    
    void Update(float deltaTime) {
        // Apply gravity
        const Vector3 gravity(0, -9.8f, 0);
        m_velocity += gravity * deltaTime;
        
        // Apply air resistance (simplified)
        const float dragCoeff = 0.1f;
        Vector3 drag = m_velocity * -dragCoeff;
        m_velocity += drag * deltaTime;
        
        // Update position (Euler integration)
        m_position += m_velocity * deltaTime;
    }
    
    Vector3 GetPosition() const { return m_position; }
    Vector3 GetVelocity() const { return m_velocity; }
    
    // Predict future position
    Vector3 PredictPosition(float time) const {
        const Vector3 gravity(0, -9.8f, 0);
        return m_position + m_velocity * time + gravity * (0.5f * time * time);
    }

private:
    Vector3 m_position;
    Vector3 m_velocity;
};

// Usage: Artillery shot
int main() {
    Vector3 cannonPos(0, 2, 0);
    Vector3 targetPos(100, 0, 0);
    
    // Calculate required velocity to hit target
    float distance = targetPos.Distance(cannonPos);
    float angle = 0.785f;  // 45° launch angle
    float g = 9.8f;
    
    float v = sqrt((g * distance) / sin(2 * angle));
    Vector3 direction = (targetPos - cannonPos).Normalize();
    Vector3 velocity = direction * v;
    
    Projectile projectile(cannonPos, velocity);
    
    // Simulate
    float time = 0;
    while (projectile.GetPosition().Y() >= 0) {
        projectile.Update(0.016f);  // 60 FPS
        time += 0.016f;
    }
}
```

---

### Collision Detection

**AABB and Sphere collision**

```cpp
#include <Math/NuMath.hpp>

using namespace NuMath;

struct AABB {
    Vector3 min;
    Vector3 max;
    
    bool Contains(const Vector3& point) const {
        return point.X() >= min.X() && point.X() <= max.X() &&
               point.Y() >= min.Y() && point.Y() <= max.Y() &&
               point.Z() >= min.Z() && point.Z() <= max.Z();
    }
};

struct BoundingSphere {
    Vector3 center;
    float radius;
};

// AABB vs AABB
bool CollideAABB(const AABB& a, const AABB& b) {
    return (a.min.X() <= b.max.X() && a.max.X() >= b.min.X()) &&
           (a.min.Y() <= b.max.Y() && a.max.Y() >= b.min.Y()) &&
           (a.min.Z() <= b.max.Z() && a.max.Z() >= b.min.Z());
}

// Sphere vs Sphere
bool CollideSphere(const BoundingSphere& a, const BoundingSphere& b) {
    float distance = a.center.Distance(b.center);
    return distance < (a.radius + b.radius);
}

// Sphere vs AABB
bool CollideSphereAABB(const BoundingSphere& sphere, const AABB& aabb) {
    // Find closest point on AABB to sphere center
    Vector3 closest = Vector3::Max(aabb.min, Vector3::Min(sphere.center, aabb.max));
    
    float distance = sphere.center.Distance(closest);
    return distance < sphere.radius;
}
```

---

## 4. Color & Post-Processing

### UI Theming

**Dynamic color themes**

```cpp
#include <Math/NuMath.hpp>

using namespace NuMath;

struct Theme {
    Color primary;
    Color secondary;
    Color background;
    Color text;
    
    // Derived colors (computed in linear space)
    Color primaryLight;
    Color primaryDark;
    Color secondaryLight;
    Color secondaryDark;
    
    static Theme CreateDark() {
        Theme t;
        t.primary = Color::FromBytes(66, 126, 234);      // Blue
        t.secondary = Color::FromBytes(118, 75, 162);    // Purple
        t.background = Color::FromBytes(18, 18, 18);     // Dark gray
        t.text = Color::FromBytes(255, 255, 255);        // White
        
        // Compute derived colors (in linear space!)
        t.primaryLight = Color::Lerp(t.primary, Colors::Linear::White, 0.3f);
        t.primaryDark = t.primary * 0.7f;
        t.secondaryLight = Color::Lerp(t.secondary, Colors::Linear::White, 0.3f);
        t.secondaryDark = t.secondary * 0.7f;
        
        return t;
    }
    
    // Apply tint to textures
    Color TintTexture(const Color& texColor, const Color& tint) const {
        return texColor * tint;
    }
};

// Usage
void RenderButton(const Theme& theme, bool hovered, bool pressed) {
    Color buttonColor = theme.primary;
    
    if (pressed) {
        buttonColor = theme.primaryDark;
    } else if (hovered) {
        buttonColor = theme.primaryLight;
    }
    
    uint32_t rgba = buttonColor.ToRGBA32();
    // SetButtonColor(rgba);
}
```

---

### HDR Tone Mapping

**Convert HDR colors to displayable range**

```cpp
#include <Math/NuMath.hpp>

using namespace NuMath;

// Reinhard tone mapping
Color ToneMapReinhard(const Color& hdrColor) {
    Color mapped = hdrColor / (hdrColor + Color(1.0f));
    return mapped.Saturate();
}

// Filmic tone mapping (Uncharted 2)
Color ToneMapFilmic(const Color& hdrColor) {
    auto Filmic = [](const Color& x) -> Color {
        const float A = 0.15f, B = 0.50f, C = 0.10f;
        const float D = 0.20f, E = 0.02f, F = 0.30f;
        return ((x * (x * A + Color(C * B)) + Color(D * E)) /
                (x * (x * A + Color(B)) + Color(D * F))) - Color(E / F);
    };
    
    const float exposure = 2.0f;
    Color curr = Filmic(hdrColor * exposure);
    Color whiteScale = Color(1.0f) / Filmic(Color(11.2f));
    
    return (curr * whiteScale).Saturate();
}

// Usage in post-processing
void PostProcess(std::vector<Color>& framebuffer) {
    for (auto& pixel : framebuffer) {
        pixel = ToneMapFilmic(pixel);
        // pixel now in [0, 1] range, ready for display
    }
}
```

---

### Color Grading

**Cinematic color correction**

```cpp
#include <Math/NuMath.hpp>

using namespace NuMath;

struct ColorGrade {
    Color shadows = Color(1, 1, 1);
    Color midtones = Color(1, 1, 1);
    Color highlights = Color(1, 1, 1);
    float saturation = 1.0f;
    float contrast = 1.0f;
    
    Color Apply(const Color& input) const {
        // Luminance (Rec. 709)
        float lum = input.R() * 0.2126f + input.G() * 0.7152f + input.B() * 0.0722f;
        
        // Apply lift-gamma-gain
        Color result = input;
        
        if (lum < 0.33f) {
            result = result * shadows;
        } else if (lum < 0.66f) {
            result = result * midtones;
        } else {
            result = result * highlights;
        }
        
        // Saturation
        Color gray = input.ToGrayscale();
        result = Color::Lerp(gray, result, saturation);
        
        // Contrast
        result = Color::Lerp(Color(0.5f), result, contrast);
        
        return result.Saturate();
    }
};

// Presets
ColorGrade CreateWarmGrade() {
    ColorGrade grade;
    grade.shadows = Color::FromSRGB(1.0f, 0.9f, 0.8f);     // Warm shadows
    grade.highlights = Color::FromSRGB(1.0f, 0.95f, 0.9f); // Warm highlights
    grade.saturation = 1.2f;
    return grade;
}
```

---

## 5. Batch Processing

### Particle System (SoA)

**High-performance particle system using Structure of Arrays**

```cpp
#include <Math/NuMath.hpp>
#include <NuMath/Batch/BatchVectorSoA.hpp>

using namespace NuMath;

class HighPerformanceParticleSystem {
public:
    static constexpr size_t MAX_PARTICLES = 100000;
    
    // SoA layout (SIMD-friendly)
    alignas(32) float pos_x[MAX_PARTICLES];
    alignas(32) float pos_y[MAX_PARTICLES];
    alignas(32) float pos_z[MAX_PARTICLES];
    
    alignas(32) float vel_x[MAX_PARTICLES];
    alignas(32) float vel_y[MAX_PARTICLES];
    alignas(32) float vel_z[MAX_PARTICLES];
    
    alignas(32) float lifetime[MAX_PARTICLES];
    
    size_t activeCount = 0;
    
    void Emit(const Vector3& position, const Vector3& velocity, int count) {
        for (int i = 0; i < count && activeCount < MAX_PARTICLES; ++i) {
            pos_x[activeCount] = position.X();
            pos_y[activeCount] = position.Y();
            pos_z[activeCount] = position.Z();
            
            vel_x[activeCount] = velocity.X();
            vel_y[activeCount] = velocity.Y();
            vel_z[activeCount] = velocity.Z();
            
            lifetime[activeCount] = 3.0f;
            
            ++activeCount;
        }
    }
    
    void Update(float deltaTime) {
        auto pos = Batch::SoA::MakeView(pos_x, pos_y, pos_z);
        auto vel = Batch::SoA::MakeView(vel_x, vel_y, vel_z);
        
        // Apply gravity (manual loop, simple)
        for (size_t i = 0; i < activeCount; ++i) {
            vel_y[i] -= 9.8f * deltaTime;
            lifetime[i] -= deltaTime;
        }
        
        // Update positions (SIMD: 4-8 particles per cycle!)
        Batch::SoA::Add(pos, pos, vel, activeCount);
        
        // Apply damping
        Batch::SoA::Mul(vel, vel, 0.99f, activeCount);
        
        // Remove dead particles (swap with last)
        for (size_t i = 0; i < activeCount; ) {
            if (lifetime[i] <= 0) {
                --activeCount;
                pos_x[i] = pos_x[activeCount];
                pos_y[i] = pos_y[activeCount];
                pos_z[i] = pos_z[activeCount];
                vel_x[i] = vel_x[activeCount];
                vel_y[i] = vel_y[activeCount];
                vel_z[i] = vel_z[activeCount];
                lifetime[i] = lifetime[activeCount];
            } else {
                ++i;
            }
        }
    }
};

// Benchmark: 100,000 particles
// NuMath (SoA): ~200 μs per update
// Standard (AoS): ~1,500 μs per update
// = 7.5x faster!
```

---

### Physics Integration

**Batch Verlet integration**

```cpp
#include <NuMath/Batch/BatchVectorSoA.hpp>

using namespace NuMath;

class VerletIntegrator {
public:
    void Initialize(size_t particleCount) {
        count = particleCount;
        
        // Allocate aligned memory
        curr_x.resize(count);
        curr_y.resize(count);
        curr_z.resize(count);
        
        prev_x.resize(count);
        prev_y.resize(count);
        prev_z.resize(count);
    }
    
    void Update(float deltaTime) {
        auto curr = Batch::SoA::MakeView(curr_x.data(), curr_y.data(), curr_z.data());
        auto prev = Batch::SoA::MakeView(prev_x.data(), prev_y.data(), prev_z.data());
        
        // Verlet integration: x_new = 2*x - x_old + a*dt^2
        // velocity = curr - prev
        std::vector<float> vel_x(count), vel_y(count), vel_z(count);
        auto vel = Batch::SoA::MakeView(vel_x.data(), vel_y.data(), vel_z.data());
        
        // vel = curr - prev
        Batch::SoA::Sub(vel, curr, prev, count);
        
        // temp = curr (save current positions)
        std::vector<float> temp_x = curr_x, temp_y = curr_y, temp_z = curr_z;
        
        // curr += vel (Euler step)
        Batch::SoA::Add(curr, curr, vel, count);
        
        // Apply gravity
        for (size_t i = 0; i < count; ++i) {
            curr_y[i] -= 9.8f * deltaTime * deltaTime;
        }
        
        // prev = temp (update previous positions)
        prev_x = temp_x;
        prev_y = temp_y;
        prev_z = temp_z;
    }

private:
    size_t count = 0;
    std::vector<float> curr_x, curr_y, curr_z;
    std::vector<float> prev_x, prev_y, prev_z;
};
```

---

## 📚 More Resources

- **[⚙️ Installation](INSTALL.md)** - Set up NuMath in your project
- **[📖 API Reference](API.md)** - Complete API documentation
- **[📊 Benchmarks](benchmarks.html)** - Performance analysis
- **[🏠 Main README](../../NuMath/README.md)** - Library overview

---

<div align="center">

**Need more examples?** [Request in Issues](https://github.com/VladHordiichuk/NuEngine/issues)

**[🏠 Home](../../NuMath/README.md)** • **[⚙️ Install](INSTALL.md)** • **[📖 API](API.md)** • **[📊 Benchmarks](benchmarks.html)**

</div>