#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#define TEMPLI_CONFIG_NAME "templi.json"

// utils to avoid showing command output
#ifdef WIN32
#define NULL_OUTPUT " > nul 2>&1"
#define TEMPLI_SEPARATOR "\\"
#else
#define NULL_OUTPUT " > /dev/null 2>&1"
#define TEMPLI_SEPARATOR "/"
#endif

namespace Templi
{
	using String = std::string;
	using VectorString = std::vector<String>;
	using MapString = std::map<String, String>;
	using SetString = std::set<String>;

	class Exception : public std::exception
	{
	private:
		String _message;

	public:
		Exception(String message) : _message(message)
		{
		}
		const char* what() const noexcept override
		{
			return _message.c_str();
		}
	};	// TempliException

	// all supported types of key in the templi.json["keys"]
	enum class KeyType
	{
		INPUT,
		BOOLEAN,
		SELECT
	};	// KeyType

	class Key
	{
	public:
		String _key /*key_value*/ {}, _label{}, _default{};
		KeyType _type;
		VectorString _choices{};
		bool _required{ false }, _clean{ true };

		Key() = default;

		static KeyType keytype_value_of(String type);
		static String keytype_to_string(KeyType type);
	};	// Key

}  // namespace Templi
