#ifndef SEARCH_ENGINE_POINTERPALS_DOCUMENTPARSER_H
#define SEARCH_ENGINE_POINTERPALS_DOCUMENTPARSER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include "nlohmann/json.hpp"
#include "Document.h"
#include "HashTable.h"
#include "IndexHandler.h"

using json = nlohmann::json;
using namespace std;

class DocumentParser {
private:
    void parseDocument(ifstream &input);
    IndexHandler &idx;
    map<string,pair<string,string>> metaData;
public:
    explicit DocumentParser(IndexHandler &i);
    DocumentParser(const DocumentParser &copy);
    DocumentParser& operator=(const DocumentParser &copy);
    ~DocumentParser();
    void openDirectory(const string &directory);

};

#endif //SEARCH_ENGINE_POINTERPALS_DOCUMENTPARSER_H
