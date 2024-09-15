#include <Templi/Templi.hpp>
#include <Templi/TempliConfig.hpp>
#include <fstream>

#include "fs_utils.hpp"
#include "utils.hpp"

using namespace Templi;

using json = nlohmann::json;

Templi::JSONConfig::JSONConfig(String template_path)
{
	this->read_config(template_path);
}

// json config
void Templi::JSONConfig::read_config(String template_path)
{
	const String config_full_path = Templi::create_config_path(template_path);
	std::ifstream config_file(config_full_path);
	json config_json;

	if (!config_file.is_open())
		throw Templi::Exception("Cannot open " + config_full_path);

	try
	{
		config_file >> config_json;
		config_file.close();
	}
	catch (const json::parse_error &e)
	{
		throw Templi::Exception(config_full_path + " is not a valid json file");
	}

	try
	{
		_ignored_paths = config_json["ignored_paths"];
		if (config_json["scripts"].is_object())
		{
			json scripts = config_json["scripts"];
			_before = scripts["before"];
			_after = scripts["after"];
		}

		for (auto key : config_json["keys"])
		{
			Key new_key;
			new_key._key = key["key"];
			new_key._label = key["label"];
			new_key._type = Key::keytype_value_of(key["type"]);

			if (new_key._type == KeyType::SELECT)
				new_key._choices = key["choices"];
			if (key["required"].is_boolean())
				new_key._required = key["required"];
			if (key["clean"].is_boolean())
				new_key._clean = key["clean"];
			if (key["default"].is_string() && !key["default"].empty())
				new_key._default = key["default"];
			this->_keys.push_back(new_key);
		}
	}
	catch (const json::exception &error)
	{
		throw Templi::Exception(config_full_path +
								" is not a valid config file (ref: "
								"https://github.com/RickaPrincy/Templi/blob/main/templi.json.md)");
	}
}

void Templi::JSONConfig::save_config(String template_path)
{
	const String config_full_path = Templi::create_config_path(template_path);
	json new_config_json = json::object();
	json keys_json = json::array();

	new_config_json["ignored_paths"] = _ignored_paths;

	if (!_before.empty() || !_after.empty())
	{
		json scripts = json::object();
		scripts["before"] = _before;
		scripts["after"] = _before;
		new_config_json["scripts"] = scripts;
	}

	for (auto key : _keys)
	{
		json new_key = json::object();
		new_key["key"] = key._key;
		new_key["label"] = key._label;
		new_key["type"] = Key::keytype_to_string(key._type);

		if (key._required)
			new_key["required"] = key._required;
		if (!key._clean)
			new_key["clean"] = key._clean;
		if (key._type == KeyType::SELECT)
			new_key["choices"] = key._choices;
		if (!key._default.empty())
			new_key["default"] = key._default;
		keys_json.push_back(new_key);
	}
	new_config_json["keys"] = keys_json;

	Templi::save_file(config_full_path, new_config_json);
};
