#ifndef SEARCH_ENGINE_POINTERPALS_ENTRY_H
#define SEARCH_ENGINE_POINTERPALS_ENTRY_H
//Owner: Alex Shockley
/*
 * 1.0: added a class that would hold 2 value types but only compare one when operator == is used
 * 1.1: added the rule of 3 because I was passing by reference
 * 1.2 overloaded all the operators including << to make displaying the contents easier
 */
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

class Entry { // basically a wrapper class for pair<string,vector<string>> that we overloaded operators to only compare the pair.first to make finding it in the AVL tree easier
private:
    pair<string, vector<string>> data;
    bool containsDoc(const string &docToCheck);
    string unstemmedWord;
public:
    Entry();
    Entry(string word);
    Entry(string word, string document);
    Entry(const Entry &copy);
    Entry& operator=(const Entry &copy);
    ~Entry();
    bool addDocument(const string &docName);
    string getWord();
    vector<string> getDocuments();
     int numDocuments() ;
    bool operator==(const string &comparison) const;
    bool operator==(const Entry &comparison) const;
    bool operator<(const string &comparison)const;
    bool operator<(const Entry &comparison)const;
    bool operator>(const string &comparison)const;
    bool operator>(const Entry &comparison)const;
    friend ostream& operator<<(ostream& os, const Entry& dt);

    const string &getUnstemmedWord() const;

    void setUnstemmedWord(const string &unstemmedWord);
};


#endif //SEARCH_ENGINE_POINTERPALS_ENTRY_H
