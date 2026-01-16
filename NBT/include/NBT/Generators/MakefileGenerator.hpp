#pragma once

#include "IProjectGenerator.hpp"
#include <filesystem>

namespace NBT
{
	class MakefileGenerator : public IProjectGenerator
	{
	public:
		MakefileGenerator(std::filesystem::path outputDir, std::filesystem::path projectRoot);

		bool Generate(const std::string& projectName, const std::vector<Module>& modules) override;

	private:
		std::filesystem::path m_outputDir;
		std::filesystem::path m_projectRoot;
	};
}