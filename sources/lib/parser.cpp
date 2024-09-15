#include "parser.hpp"

#include "fs_utils.hpp"

using namespace Templi;

String Templi::brackets_parser(String text, const MapString values, SetString &words)
{
	String result;
	size_t pos = 0;

	while (pos < text.length())
	{
		size_t open_brace_pos = text.find("{{", pos);
		size_t escape_pos = text.find("\\{", pos);

		// Handle \{
		if (escape_pos != String::npos &&
			(open_brace_pos == String::npos || escape_pos < open_brace_pos))
		{
			result += text.substr(pos, escape_pos - pos) + '{';
			pos = escape_pos + 2;
			continue;
		}

		// If there's no more {{, just add the remaining part of the text to result
		if (open_brace_pos == String::npos)
		{
			result += text.substr(pos);
			break;
		}

		// Add the text before {{ to the result
		result += text.substr(pos, open_brace_pos - pos);

		// Find the closing braces }}
		size_t start_pos = open_brace_pos + 2;
		size_t end_pos = text.find("}}", start_pos);

		if (end_pos == String::npos)
		{
			result += text.substr(open_brace_pos);
			break;
		}

		size_t wordLength = end_pos - start_pos;
		String word = text.substr(start_pos, wordLength);

		words.insert(word);
		auto it = values.find(word);

		if (it != values.end())
		{
			result += it->second;
			pos = end_pos + 2;
		}
		else
		{
			result += "{{" + word + "}}";
			pos = end_pos + 2;
		}
	}

	return result;
}

SetString Templi::get_brackets_words(String text)
{
	SetString words;
	brackets_parser(text, {}, words);
	return words;
}

String Templi::replace_brackets_words(String text, MapString values)
{
	SetString words;
	return brackets_parser(text, values, words);
}

SetString Templi::file_get_brackets_words(String file_path)
{
	SetString words;
	process_for_each_line(file_path,
		[&](const String &line_content)
		{
			for (auto word : get_brackets_words(line_content))
				words.insert(word);
		});
	return words;
}

void Templi::file_brackets_parser(String file_path, String output_path, MapString values)
{
	std::stringstream file_parsed_content;

	process_for_each_line(file_path,
		[&](const String &line_content)
		{ file_parsed_content << replace_brackets_words(line_content, values) << "\n"; });

	save_file(output_path, file_parsed_content.str());
}
