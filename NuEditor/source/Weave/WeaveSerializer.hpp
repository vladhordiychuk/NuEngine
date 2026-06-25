#pragma once

#include <QString>
#include <Weave/WeaveGraphScene.hpp>

namespace NuEditor::Weave
{
	class WeaveSerializer
	{
	public:
		struct SaveResult
		{
			bool Success = false;
			QString Error;
		};

		struct LoadResult
		{
			bool Success = false;
			QString Error;
			int NodesLoaded = 0;
			int ConnectionsLoaded = 0;
		};

		[[nodiscard]] static SaveResult Save(const WeaveGraphScene& scene, const QString& path);
		[[nodiscard]] static LoadResult Load(WeaveGraphScene& scene, const QString& path);

	private:
		static constexpr uint32_t k_Magic = 0x57475048u;
		static constexpr uint16_t k_Version = 1;
	};
} // namespace NuEditor::Weave