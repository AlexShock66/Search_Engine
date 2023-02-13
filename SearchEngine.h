#ifndef SEARCH_ENGINE_POINTERPALS_SEARCHENGINE_H
#define SEARCH_ENGINE_POINTERPALS_SEARCHENGINE_H

#include "DocumentParser.h"
#include "IndexHandler.h"
#include "SearchQuery.h"

using namespace std;

class SearchEngine {
private:
    IndexHandler index;
    DocumentParser parser;
    SearchQuery query;
public:
    SearchEngine();
    void run(const string &directory);
};


#endif //SEARCH_ENGINE_POINTERPALS_SEARCHENGINE_H
