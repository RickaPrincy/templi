#include <TColor/TColor.hpp>
#include <Templi/Cli/Templi_Cli.hpp>
#include <Templi/file.hpp>
#include <Templi/Templi.hpp>

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

void Templi::takeConfiguration(){
    bool runConfigure = true;
    std::vector<std::string> filePaths;
    std::vector<std::pair<std::string, std::string>> files;
    std::string configFileName, templateFolder;

    Templi::writeLine();
    std::cout << "Config file (default: 'config.templi'): ";
    Templi::cleanInput(configFileName);

    std::cout << "Template folder(default: 'template'): ";
    Templi::cleanInput(templateFolder);

    if(configFileName.empty())
        configFileName = "config.templi";
    if(templateFolder.empty())
        templateFolder= "template";

    filePaths = Templi::getFolderContent(templateFolder);

    for(const auto &path : filePaths){
        std::string fileOutput;
        std::cout << "Enter output path for " << path << ": ";
        Templi::cleanInput(fileOutput);
        if(!fileOutput.empty()){
            files.push_back({templateFolder +"/" + path, fileOutput});
        }
    }

    launchConfiguration(configFileName, files);
}

void Templi::launchConfiguration(std::string config, std::vector<std::pair<std::string, std::string>> &files){
    for(const auto &file: files){
        TColor::write_endl(TColor::YELLOW, "Configuring " + file.first + " ...");
        Templi::configureFile(file.first, file.second, config);
    }

    TColor::write(TColor::GREEN, "Configuration finished!, run ");
    TColor::write(TColor::YELLOW, "templi --generate ");
    TColor::write_endl(TColor::GREEN, "to create tempalte");
}