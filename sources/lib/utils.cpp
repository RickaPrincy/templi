#include "utils.hpp"

#include <TColor/TColor.hpp>
#include <Templi/Templi.hpp>
#include <Templi/TempliConfig.hpp>
#include <Templi/types.hpp>
#include <chrono>
#include <cstdlib>
#include <random>
#include <rcli/input_config.hpp>
#include <rcli/inputs.hpp>
#include <sstream>

#ifdef WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif	// WIN32

#include "parser.hpp"

using namespace Templi;

String Templi::create_config_path(String template_path)
{
	return template_path + TEMPLI_SEPARATOR + TEMPLI_CONFIG_NAME;
}

String Templi::ask_input_value(const Key &key)
{
	rcli::InputConfig config;

	if (key._type == KeyType::SELECT)
		return rcli::ask_value_in_list_as_number(key._label, key._choices);

	if (key._type == KeyType::BOOLEAN)
		return rcli::ask_boolean(key._label, key._default == "true") ? "true" : "false";

	config.text(key._label).default_value(key._default).required(key._required).clean(key._clean);

	return rcli::ask_input_value(config);
}

void Templi::ask_and_get_templi_config_value(String template_path,
	MapString &values,
	VectorString &ignored_paths,
	VectorString &before_generating,
	VectorString &after_generating)
{
	JSONConfig json_config(template_path);
	ignored_paths = json_config._ignored_paths;
	before_generating = json_config._before;
	after_generating = json_config._after;

	for (const auto &key : json_config._keys)
		values.insert(std::make_pair(key._key, ask_input_value(key)));
}

void Templi::execute_scripts(const MapString &values, const VectorString &scripts)
{
	for (const auto script : scripts)
	{
		String command = Templi::replace_brackets_words(script, values);
		TColor::write_endl(TColor::B_WHITE, command);
		std::system(command.c_str());
	}
}

static String get_temporary_path()
{
#ifdef WIN32
	char temp_dir[MAX_PATH];
	DWORD result = GetTempPathA(MAX_PATH, temp_dir);

	if (result > 0 && result <= MAX_PATH)
	{
		template_path;
	}
	return "";
#else
	const char *temp_dir = std::getenv("TMPDIR");
	if (temp_dir == nullptr)
	{
		temp_dir = "/tmp";
	}
	String result = temp_dir;
	return result;
#endif	// WIN32
}

static String generate_unique_id()
{
	auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::mt19937_64 generator(seed);

	std::uniform_int_distribution<uint64_t> distribution;
	uint64_t randomValue = distribution(generator);
	std::stringstream ss;
	ss << std::hex << randomValue;
	return ss.str();
}

void Templi::clone_template(String &template_path)
{
	String temporary_path = get_temporary_path();
	if (template_path.empty())
	{
		throw new Templi::Exception("Cannot get the temporary path");
	}
	const String new_template_path = temporary_path + TEMPLI_SEPARATOR + generate_unique_id();
	const String clone_command =
		"git clone " + template_path + " " + new_template_path + NULL_OUTPUT;
	template_path = new_template_path;

	TColor::write_endl(TColor::B_GREEN, " Cloning the template...\n");
	std::system(clone_command.c_str());
}
