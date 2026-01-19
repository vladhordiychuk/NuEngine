// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <memory>
#include <vector>

#include <Graphics/Abstractions/IVertexBuffer.hpp>
#include <Graphics/Abstractions/IIndexBuffer.hpp>

namespace NuEngine::Graphics
{
	class IVertexArray
	{
	public:
		virtual ~IVertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<IVertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<std::shared_ptr<IVertexBuffer>>& GetVertexBuffer() const = 0;
		virtual const std::shared_ptr<IIndexBuffer>& GetIndexBuffer() const = 0;
	};
}