#ifndef SEARCH_ENGINE_POINTERPALS_DOCUMENT_H
#define SEARCH_ENGINE_POINTERPALS_DOCUMENT_H

#include <string>
#include <unordered_set>
#include <sstream>

using namespace std;

class Document {
private:
    string datePublished;
    string publisher;
    string ID;
    string title;
    string author;
    string abstract;
    string body;
    int words;
public:
    Document(string docID, string docTitle, string docAuthor, string docAbstract, string docBody, int uniqueWords);
    ~Document();
    Document(const Document &copy);
    Document& operator=(const Document &copy);
    // functions
    string& getID();
    string& getTitle();
    string& getAuthor();
    string& getAbstract();
    string& getBody();
    int getWords();

    const string &getDatePublished() const;

    void setDatePublished(const string &datePublished);

    const string &getPublisher() const;

    void setPublisher(const string &publisher);

    const string &getId() const;

    void setId(const string &id);

    void setTitle(const string &title);

    void setAuthor(const string &author);

    void setAbstract(const string &abstract);

    void setBody(const string &body);

    void setWords(int words);
};


#endif //SEARCH_ENGINE_POINTERPALS_DOCUMENT_H
