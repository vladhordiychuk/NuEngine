// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <string>
#include <vector>

namespace NuBuildTool
{
	/*
	* @brief
	*/
	class Module
	{
	public:
		/*
		* @brief
		*/
		std::string Name;

		/*
		* @brief
		*/
		std::vector<std::string> Sources;

		/*
		* @brief
		*/
		std::vector<std::string> Dependencies;

		/*
		* @brief
		*/
		std::vector<std::string> CompilerFlags;

		/*
		* @brief
		*/
		std::string OutputType = "StaticLib";

		/*
		* @brief
		*/
		virtual ~Module() = default;

		/*
		* @brief
		*/
		virtual void PreBuild();

		/*
		* 
		*/
		virtual void Build();

		/*
		* @brief
		*/
		virtual void PostBuild();
	};
}