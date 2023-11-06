#include <Templi/file.hpp>
#include <Templi/string.hpp>
#include <cstdio>
#include <filesystem>

namespace fs = std::filesystem;

void Templi::writeLine(std::ofstream *file, std::string text){
    *file << text << "\n";
}

bool Templi::deleteFile(std::string path){
    return std::remove(path.c_str()) != 0;
}

bool Templi::testAndWrite(std::ofstream *file,std::string &text){
    if (file->is_open()) {
        *file << text;
        file->close();
        return true;
    }
    return false;
}

bool Templi::saveOrUpdate(std::string path, std::string &text){
    std::ifstream fileExist(path);
    const bool fileExistStatus = fileExist.good();
    fileExist.close();
    
    if (fileExistStatus) {
        std::ofstream file(path, std::ios::app);
        return testAndWrite(&file, text);
    } else {
        std::ofstream file(path);
        return testAndWrite(&file, text);
    }
}

std::vector<std::tuple<std::string, std::string,std::string, int, int>> Templi::extractConfigValue(std::string configPath, std::map<std::string, std::string> &values){
    std::vector<std::tuple<std::string,std::string, std::string, int, int>> extraced;
    std::ifstream configFile(configPath);
    std::string lineContent, lastFileName = "";
    int line{1}, lastLine{-1}, sameFileSameLine{1};

    if(!configFile.is_open())
        extraced;
    
    while(std::getline(configFile, lineContent)){
        std::tuple<std::string, std::string,std::string, int, int> value = Templi::extractValues(lineContent);
        const std::string fileNameValue = std::get<0>(value),fileOutput = std::get<1>(value), wordValue = std::get<2>(value);
        size_t lineValue = std::get<3>(value), indexValue = std::get<4>(value);

        if(fileNameValue != "" && values.find(wordValue) != values.end()){
            const std::string wordContent = values.at(wordValue);
            if( lastFileName == fileNameValue && lastLine == lineValue){
                indexValue += sameFileSameLine;
                sameFileSameLine += wordContent.size();
            }else{
                sameFileSameLine = wordContent.size();
            }
            
            extraced.push_back(std::make_tuple(fileNameValue,fileOutput,wordContent, lineValue, indexValue));
            lastFileName = fileNameValue;
            lastLine = lineValue;
        }
    }
    configFile.close();
    
    return extraced;
}

std::vector<std::string> Templi::getFolderContent(std::string path){
    std::vector<std::string> result;
    
    if(fs::exists(path) && fs::is_directory(path)){
        for(const auto &file: fs::directory_iterator(path)){
            result.push_back(file.path().filename());
        }
    }

    return result;
}