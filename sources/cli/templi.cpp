#include <TColor/TColor.hpp>
#include <Templi/Templi.hpp>
#include <Templi/TempliConfig.hpp>
#include <Templi/types.hpp>
#include <rcli/command.hpp>
#include <rcli/input_config.hpp>
#include <rcli/inputs.hpp>
#include <rcli/rcli.hpp>

using namespace Templi;

int main(int argc, const char *argv[])
{
	// base of the application
	rcli::App templi("templi", "Generate a project easily", Templi_VERSION);
	templi.add_informations(
		{ { "Author", "RickaPrincy" }, { "Github", "<https://github.com/RickaPrincy/Templi>" } });

	rcli::InputConfig config;
	config.required(true).clean(true);

	// configure command
	rcli::Option template_path_option("-t,--template", "Specify template path", "template_path");
	rcli::Command configure("configure",
		"Configure one template to generate templi.json",
		[&](rcli::Command *_configure)
		{
			String template_path = _configure->get_option_value("template_path");
			if (template_path.empty())
				template_path = rcli::ask_input_value(config.text("Template path"));
			try
			{
				Templi::configure(template_path);
				TColor::write_endl(TColor::B_GREEN, "[ DONE ]: \"templi.json\" was generated");
			}
			catch (Templi::Exception error)
			{
				String message = error.what();
				TColor::write_endl(TColor::B_RED, "[ ERROR ]: " + message);
			}
		});

	configure.add_option(&template_path_option);

	// generate command
	rcli::Command generate("generate",
		"Generate new project with one template",
		[&](rcli::Command *_generate)
		{
			String template_path = _generate->get_option_value("template_path");
			String output_path = _generate->get_option_value("output_path");
			String path_suffix = _generate->get_option_value("path_suffix");

			if (template_path.empty())
				template_path = rcli::ask_input_value(config.text("Template path (or github url)"));

			if (output_path.empty())
				output_path = rcli::ask_input_value(config.text("Output path"));

			try
			{
				Templi::generate_with_templi_config(template_path, output_path, path_suffix);
				TColor::write_endl(TColor::B_GREEN, "\n[ DONE ]: Project generated successfully");
			}
			catch (Templi::Exception error)
			{
				String message = error.what();
				TColor::write_endl(TColor::B_RED, "[ ERROR ]: " + message);
			}
		});

	generate.add_option(&template_path_option);
	generate.add_option("-o,--output", "Specify output path", "output_path");
	generate.add_option(
		"-p, --path-suffix", "Path suffix after template_path if you use monorepo", "path_suffix");

	templi.add_subcommand(&configure);
	templi.add_subcommand(&generate);

	templi.run(argc, argv);
	return 0;
}
