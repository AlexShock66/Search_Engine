#ifndef SEARCH_ENGINE_POINTERPALS_SEARCHQUERY_H
#define SEARCH_ENGINE_POINTERPALS_SEARCHQUERY_H

#include <cstring>
#include <string>
#include "IndexHandler.h"

using namespace std;

class SearchQuery {
private:
    IndexHandler &idx;
    vector<Document*> currentSearch;
    vector<Document*> rankSearches(const vector<Document*> &docIDs, const vector<string> &words);
    static bool isNumber(const string &stringToCheck);
public:
    SearchQuery(IndexHandler &i);
    bool printResults(const string &search);
    bool printResult(int resultNumber);
    string getQuery(int status);
};


#endif //SEARCH_ENGINE_POINTERPALS_SEARCHQUERY_H
