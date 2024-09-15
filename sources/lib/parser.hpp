#pragma once

#include <Templi/types.hpp>

namespace Templi
{
	String brackets_parser(String text,const MapString values, SetString &words);
	SetString get_brackets_words(String text);
	String replace_brackets_words(String text, MapString values);

	void file_brackets_parser(String file_path, String output_path, MapString values);
	SetString file_get_brackets_words(String file_path);
}  // namespace Templi
