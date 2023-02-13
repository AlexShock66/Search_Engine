#ifndef SEARCH_ENGINE_POINTERPALS_INDEX_H
#define SEARCH_ENGINE_POINTERPALS_INDEX_H

#include <vector>
#include <string>
#include "AVLTree.h"
#include "Entry.h"

using namespace std;

class Index {
private:
    AVLTree<Entry> words;
    AVLTree<Entry> authors;
public:
    Index();
    Index(const Index &copy);
    Index& operator=(const Index &copy);
    ~Index();
    bool addWord(const string& word , const string& docName,string unstemmedWord);
    bool contains(const string &word);
    bool containsAuthor( const string &author);
    bool addDocument(const string &word, string docName);
    vector<string> getAuthorDocs(const string &author);
    vector<string> getDocuments(const string &word);
    void addAuthor(const string& author, const string& docName);
    void addDocToAuthor(const string &author, string docName);
    int getSize();
    int numDocuments(const string &word);
    vector<Entry> saveIndex();
    void clearIndex();



    vector<std::string> calcMostFrequentWords();

    void printStats();

};


#endif //SEARCH_ENGINE_POINTERPALS_INDEX_H
