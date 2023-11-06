#ifndef __TEMPLI_TEMPLATE__
#define __TEMPLI_TEMPLATE__

    #include <string>
    #include <fstream>
    #include <map>
    
    namespace Templi{
        bool configureFile(std::string path, std::string configPath);
        bool writeFile(std::string configPath, std::map<std::string, std::string> values);
    }

#endif