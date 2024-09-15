#pragma once

#include <Templi/types.hpp>

namespace Templi
{

	String ask_input_value(const Key &key);

	String create_config_path(String template_path);

	void ask_and_get_templi_config_value(String template_path,
		MapString &values,
		VectorString &ignored_paths,
		VectorString &before_generating,
		VectorString &after_generating);

	void execute_scripts(const MapString &values, const VectorString &scripts);

	void clone_template(String &template_path);
}  // namespace Templi
