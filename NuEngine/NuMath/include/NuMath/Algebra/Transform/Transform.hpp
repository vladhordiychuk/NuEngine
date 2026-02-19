// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Algebra/Transform/TransformAPI.hpp>
#include <NuMath/Algebra/Vector/Vector3.hpp>
#include <NuMath/Algebra/Quaternion/Quaternion.hpp>
#include <NuMath/Algebra/Matrix/Matrix4x4.hpp>
#include <NuMath/Core/Common.hpp>

#include <string>
#include <sstream>

namespace NuMath
{
    /**
     * @brief Represents a 3D transformation with position, rotation, and scale.
     *
     * Stores transformation data in an optimized format and provides methods
     * for combining transformations, converting to matrices, and transforming vectors.
     */
    class alignas(16) Transform
    {
    public:
        /**
         * @brief Default constructor - creates identity transform.
         *
         * Position: (0, 0, 0)
         * Rotation: Identity quaternion (0, 0, 0, 1)
         * Scale: (1, 1, 1)
         */
        NU_FORCEINLINE Transform() noexcept
            : m_data(TransformAPI::Identity())
        {
        }

        /**
         * @brief Constructs a transform with specified components.
         *
         * @param position The translation vector.
         * @param rotation The rotation quaternion.
         * @param scale The scale vector.
         */
        NU_FORCEINLINE Transform(const Vector3& position, const Quaternion& rotation, const Vector3& scale) noexcept
            : m_data(TransformAPI::Create(position.SimdData(), rotation.SimdData(), scale.SimdData()))
        {
        }

        /**
         * @brief Copy constructor.
         */
        NU_FORCEINLINE Transform(const Transform& other) noexcept
            : m_data(other.m_data)
        {
        }

        /**
         * @brief Move constructor.
         */
        NU_FORCEINLINE Transform(Transform&& other) noexcept
            : m_data(std::move(other.m_data))
        {
        }

        /**
         * @brief Internal constructor from API data.
         */
        explicit NU_FORCEINLINE Transform(const TransformAPI::NuTransform& data) noexcept
            : m_data(data)
        {
        }

        /**
         * @brief Sets the position component.
         */
        NU_FORCEINLINE void SetPosition(const Vector3& position) noexcept
        {
            TransformAPI::SetPosition(m_data, position.SimdData());
        }

        /**
         * @brief Sets the rotation component.
         */
        NU_FORCEINLINE void SetRotation(const Quaternion& rotation) noexcept
        {
            TransformAPI::SetRotation(m_data, rotation.SimdData());
        }

        /**
         * @brief Sets the scale component.
         */
        NU_FORCEINLINE void SetScale(const Vector3& scale) noexcept
        {
            TransformAPI::SetScale(m_data, scale.SimdData());
        }

        /**
         * @brief Gets the position component.
         */
        [[nodiscard]] NU_FORCEINLINE Vector3 GetPosition() const noexcept
        {
            return Vector3(TransformAPI::GetPosition(m_data));
        }

        /**
         * @brief Gets the rotation component.
         */
        [[nodiscard]] NU_FORCEINLINE Quaternion GetRotation() const noexcept
        {
            return Quaternion(TransformAPI::GetRotation(m_data));
        }

        /**
         * @brief Gets the scale component.
         */
        [[nodiscard]] NU_FORCEINLINE Vector3 GetScale() const noexcept
        {
            return Vector3(TransformAPI::GetScale(m_data));
        }

        /**
         * @brief Converts the transform to a 4x4 transformation matrix.
         *
         * @return Matrix representing TRS (Translation * Rotation * Scale).
         */
        [[nodiscard]] NU_FORCEINLINE Matrix4x4 GetMatrix() const noexcept
        {
            return Matrix4x4(TransformAPI::ToMatrix(m_data));
        }

        /**
         * @brief Computes the inverse transformation matrix.
         */
        [[nodiscard]] NU_FORCEINLINE Matrix4x4 GetInverseMatrix() const noexcept
        {
            return Matrix4x4(TransformAPI::ToInverseMatrix(m_data));
        }

        /**
         * @brief Computes the inverse of this transform.
         */
        [[nodiscard]] NU_FORCEINLINE Transform Inverse() const noexcept
        {
            return Transform(TransformAPI::Inverse(m_data));
        }

        /**
         * @brief Combines this transform with a child transform.
         *
         * Equivalent to: parent * child
         */
        [[nodiscard]] NU_FORCEINLINE Transform Combine(const Transform& child) const noexcept
        {
            return Transform(TransformAPI::Combine(m_data, child.m_data));
        }

        /**
         * @brief Transforms a point by this transformation.
         */
        [[nodiscard]] NU_FORCEINLINE Vector3 TransformPoint(const Vector3& point) const noexcept
        {
            return Vector3(TransformAPI::TransformPoint(m_data, point.SimdData()));
        }

        /**
         * @brief Transforms a direction (ignores translation).
         */
        [[nodiscard]] NU_FORCEINLINE Vector3 TransformDirection(const Vector3& direction) const noexcept
        {
            return Vector3(TransformAPI::TransformDirection(m_data, direction.SimdData()));
        }

        /**
         * @brief Gets the forward direction vector (local Z axis).
         */
        [[nodiscard]] NU_FORCEINLINE Vector3 GetForward() const noexcept
        {
            return Vector3(TransformAPI::GetForward(m_data));
        }

        /**
         * @brief Gets the up direction vector (local Y axis).
         */
        [[nodiscard]] NU_FORCEINLINE Vector3 GetUp() const noexcept
        {
            return Vector3(TransformAPI::GetUp(m_data));
        }

        /**
         * @brief Gets the right direction vector (local X axis).
         */
        [[nodiscard]] NU_FORCEINLINE Vector3 GetRight() const noexcept
        {
            return Vector3(TransformAPI::GetRight(m_data));
        }

        /**
         * @brief Linearly interpolates between two transforms.
         */
        [[nodiscard]] NU_FORCEINLINE static Transform Lerp(const Transform& a, const Transform& b, float t) noexcept
        {
            return Transform(TransformAPI::Lerp(a.m_data, b.m_data, t));
        }

        /**
         * @brief Creates an identity transform.
         */
        [[nodiscard]] NU_FORCEINLINE static Transform Identity() noexcept
        {
            return Transform(TransformAPI::Identity());
        }

        /**
         * @brief Creates a transform with only translation.
         */
        [[nodiscard]] NU_FORCEINLINE static Transform Translation(const Vector3& position) noexcept
        {
            return Transform(TransformAPI::Translation(position.SimdData()));
        }

        /**
         * @brief Creates a transform with only rotation.
         */
        [[nodiscard]] NU_FORCEINLINE static Transform Rotation(const Quaternion& rotation) noexcept
        {
            return Transform(TransformAPI::Rotation(rotation.SimdData()));
        }

        /**
         * @brief Creates a transform with only scale.
         */
        [[nodiscard]] NU_FORCEINLINE static Transform Scale(const Vector3& scale) noexcept
        {
            return Transform(TransformAPI::Scale(scale.SimdData()));
        }

        /**
         * @brief Combine operator (parent * child).
         */
        [[nodiscard]] NU_FORCEINLINE Transform operator*(const Transform& other) const noexcept
        {
            return Combine(other);
        }

        /**
         * @brief Copy assignment.
         */
        NU_FORCEINLINE Transform& operator=(const Transform& other) noexcept
        {
            if (this != &other)
            {
                m_data = other.m_data;
            }
            return *this;
        }

        /**
         * @brief Move assignment.
         */
        NU_FORCEINLINE Transform& operator=(Transform&& other) noexcept
        {
            if (this != &other)
            {
                m_data = std::move(other.m_data);
            }
            return *this;
        }

        /**
         * @brief Equality comparison.
         */
        [[nodiscard]] NU_FORCEINLINE bool operator==(const Transform& other) const noexcept
        {
            return TransformAPI::Equal(m_data, other.m_data);
        }

        /**
         * @brief Inequality comparison.
         */
        [[nodiscard]] NU_FORCEINLINE bool operator!=(const Transform& other) const noexcept
        {
            return !(*this == other);
        }

        /**
         * @brief String representation.
         */
        [[nodiscard]] std::string ToString() const
        {
            std::ostringstream oss;
            oss << "Transform(\n";
            oss << "  Position: " << GetPosition().ToString() << "\n";
            oss << "  Rotation: " << GetRotation().ToString() << "\n";
            oss << "  Scale: " << GetScale().ToString() << "\n";
            oss << ")";
            return oss.str();
        }

        /**
         * @brief Stream output operator.
         */
        friend std::ostream& operator<<(std::ostream& os, const Transform& transform)
        {
            os << transform.ToString();
            return os;
        }

    private:
        TransformAPI::NuTransform m_data;
    };
} // namespace NuMath