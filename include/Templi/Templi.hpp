#pragma once

#include <Templi/types.hpp>

namespace Templi
{
	void generate(String template_path,
		String output_path,
		MapString values,
		VectorString ignored_path = {});

	void generate_with_templi_config(String template_path,
		String output_path,
		String path_suffix = "");

	void configure(String template_path, VectorString ignored_path = {});

	class JSONConfig
	{
	public:
		VectorString _ignored_paths{}, _before{}, _after{};
		std::vector<Key> _keys{};

		void read_config(String template_path);
		void save_config(String template_path);

		JSONConfig() = default;
		JSONConfig(String template_path);
	};	// JSONConfig

}  // namespace Templi
