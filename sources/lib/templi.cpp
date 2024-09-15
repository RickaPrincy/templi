#include <TColor/TColor.hpp>
#include <Templi/Templi.hpp>
#include <Templi/TempliConfig.hpp>
#include <Templi/types.hpp>

#include "fs_utils.hpp"
#include "parser.hpp"
#include "utils.hpp"

using namespace Templi;

const String GIT_SUFFIX = ".git";

void Templi::generate(String template_path,
	String output_path,
	MapString values,
	VectorString ignored_path)
{
	VectorString files;
	Templi::get_folder_files(template_path, files, ignored_path);

	if (files.empty())
		throw Templi::Exception("Folder empty or no words inside {{}} was found");

	Templi::copy_folder(template_path, output_path);
	Templi::delete_file(Templi::create_config_path(output_path));

	for (auto file : files)
	{
		auto path = output_path + file.substr(template_path.size());
		Templi::file_brackets_parser(file, path, values);
	}
}

void Templi::configure(String template_path, VectorString ignored_path)
{
	VectorString files;
	SetString words;
	JSONConfig json_config;

	json_config._ignored_paths = ignored_path;
	json_config._ignored_paths.push_back(TEMPLI_CONFIG_NAME);

	Templi::get_folder_files(template_path, files, ignored_path);

	if (files.empty())
		throw Templi::Exception("Folder empty or no words inside {{}} was found");

	for (auto file : files)
	{
		SetString words_found = Templi::file_get_brackets_words(file);

		if (words_found.empty())
			json_config._ignored_paths.push_back(file.substr(template_path.size() + 1));

		for (auto word_found : words_found)
			words.insert(word_found);
	}

	for (auto word : words)
	{
		Key new_key;
		new_key._key = word;
		json_config._keys.push_back(new_key);
	}
	json_config.save_config(template_path);
}

void Templi::generate_with_templi_config(String template_path,
	String output_path,
	String path_suffix)
{
	MapString values{ { "TEMPLI_OUTPUT_FOLDER", output_path } };
	VectorString ignored_paths{}, before_generating{}, after_generating{};
	bool is_github_repository = false;

	if (template_path.length() >= GIT_SUFFIX.length())
	{
		// test if the template_path is a git repository
		if (0 == template_path.compare(
					 template_path.length() - GIT_SUFFIX.length(), GIT_SUFFIX.length(), GIT_SUFFIX))
		{
			Templi::clone_template(template_path);
			is_github_repository = true;
		}
	}
	String json_template_path = template_path + path_suffix;

	try
	{
		Templi::ask_and_get_templi_config_value(
			json_template_path, values, ignored_paths, before_generating, after_generating);
		Templi::execute_scripts(values, before_generating);
		Templi::generate(json_template_path, output_path, values, ignored_paths);
		Templi::execute_scripts(values, after_generating);
	}
	catch (Templi::Exception error)
	{
		if (is_github_repository)
		{
			Templi::delete_folder(template_path);
		}
		String message = error.what();
		throw Templi::Exception(message);
	}

	if (is_github_repository)
	{
		Templi::delete_folder(template_path);
	}
}
