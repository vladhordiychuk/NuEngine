#pragma once

#include "NBT/Core/Module.hpp"
#include "NBT/Core/Config.hpp"
#include <vector>
#include <string>

namespace NBT
{
	/**
	 * @brief
	 */
	class IProjectGenerator
	{
	public:
		/**
		 * @brief
		 */
		virtual ~IProjectGenerator() = default;

		/**
		 * @brief
		 */
		virtual bool Generate(const std::string& projectName, const std::vector<Module>& modules) = 0;
	};
}