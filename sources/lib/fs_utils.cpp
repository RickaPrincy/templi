#include "fs_utils.hpp"

#include <Templi/TempliConfig.hpp>
#include <Templi/types.hpp>
#include <algorithm>
#include <cstdio>
#include <filesystem>

namespace fs = std::filesystem;
using namespace Templi;

void Templi::save_file(String path, nlohmann::json text)
{
	std::ofstream file(path);
	if (file.is_open())
	{
		file << text.dump(4);
		return;
	}
	throw Templi::Exception("Cannot save " + path);
}

void Templi::save_file(String path, String text)
{
	std::ofstream file(path);
	write_in_open_file(&file, text);
}

void Templi::delete_file(String path)
{
	if (!(std::remove(path.c_str()) == 0))
		throw Templi::Exception("Cannot delete " + path);
}

void Templi::write_in_open_file(std::ofstream *file, String &text)
{
	if (file->is_open())
	{
		*file << text;
		file->close();
		return;
	}
	throw Templi::Exception("Cannot save text to a file which is not open");
}

void Templi::get_folder_files(String path, VectorString &result, VectorString exclude_paths)
{
	if (!fs::exists(path) || !fs::is_directory(path))
		throw Templi::Exception(path + " is a not a folder");

	for (const auto &file : fs::directory_iterator(path))
	{
		auto is_exclude = std::find_if(exclude_paths.begin(),
			exclude_paths.end(),
			[&](const auto &exclude_path)
			{ return file.path().string() == (path + TEMPLI_SEPARATOR + exclude_path); });

		if (is_exclude != exclude_paths.end())
			continue;

		if (fs::is_directory(file))
			Templi::get_folder_files(file.path().string(), result, exclude_paths);
		else if (fs::is_regular_file(file))
			result.push_back(file.path().string());
	}
}

void Templi::copy_folder(String source, String destination)
{
	try
	{
		fs::copy(source,
			destination,
			fs::copy_options::recursive | fs::copy_options::overwrite_existing);
	}
	catch (const std::exception &e)
	{
		throw Templi::Exception("Error when try to copy " + source + " to " + destination);
	}
}

void Templi::process_for_each_line(String path,
	std::function<void(const String &line_content)> process)
{
	std::ifstream file(path);

	if (!file.is_open())
		throw Templi::Exception("Cannot open " + path);

	std::string line;

	while (std::getline(file, line))
		process(line);

	file.close();
}

void Templi::delete_folder(String path)
{
	if (!fs::exists(path))
	{
		return;
	}

	try
	{
		fs::remove_all(path);
	}
	catch (const std::filesystem::filesystem_error &e)
	{
		String message = e.what();
		throw Templi::Exception(message);
	}
}
