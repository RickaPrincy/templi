#pragma once

#include <Templi/types.hpp>
#include <fstream>
#include <functional>

#include "json.hpp"

namespace Templi
{
	void write_in_open_file(std::ofstream *file, String &text);

	void save_file(String path, String text);
	void save_file(String path, nlohmann::json text);
	void delete_file(String path);
	void delete_folder(String path);

	void copy_folder(String source, String destination);
	void get_folder_files(String path, VectorString &result, VectorString exclude_path = {});

	void process_for_each_line(String path,
		std::function<void(const String &line_content)> process);
}  // namespace Templi
