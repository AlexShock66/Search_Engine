#ifndef SEARCH_ENGINE_POINTERPALS_INDEXHANDLER_H
#define SEARCH_ENGINE_POINTERPALS_INDEXHANDLER_H

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include "AVLTree.h"
#include "Document.h"
#include "HashTable.h"
#include "Index.h"
#include "Porter2/porter2_stemmer.h"

using namespace std;

class IndexHandler {
private:
    long sumOfUniqueWords;
    int totalDocuments;
    Index *theIndex;
    unordered_map<string, string> stemmedWords;
    HashTable<string, Document*> documents;
    AVLTree<string> stopWords;
    string getStem(const string &word);
    bool isStopWord(const string &word);
    void formatWord(string &word);
    void formatWords(vector<string> &words);
    int documentFrequency(string word);
public:
    IndexHandler();
    IndexHandler(const IndexHandler &copy);
    IndexHandler& operator=(const IndexHandler &copy);
    ~IndexHandler();
    bool addWord(string newWord, const string &docID);
    void addAuthor(string author, const string &docID);
    void addDocument(const string &docID, Document *doc);
    vector<string> getDocumentIDs(string word);
    void incrementNumDocs();
    vector<Document*> getDocuments(const vector<string> &documentIDs);
    vector<string> getIntersection(vector<string> words);
    vector<string> getUnion(vector<string> words);
    vector<string> getDisunion(vector<string> docs, string word);
    vector<string> filterAuthor(const vector<string> &docIDs, string author);
    vector<Document*> scoreDocuments(const vector<Document*> &docsToScore, vector<string> words);
    void saveIndex();
    void clearIndex();

    void printStats();

    long getSumOfUniqueWords() const;

    void addToTotalUniqueWords(int a);
};


#endif //SEARCH_ENGINE_POINTERPALS_INDEXHANDLER_H
