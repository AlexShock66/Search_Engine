// owner: Jay Kynerd
// 1.0: get/set ID, title, author, abstract, body, words
// 1.5: rule of 3
// 1.6: get/set words

#include <iostream>
#include "Document.h"

Document::Document(string docID, string docTitle, string docAuthor, string docAbstract, string docBody, int uniqueWords) {
    ID = docID;
    title = docTitle;
    author = docAuthor;
    abstract = docAbstract;
    body = docBody;
    words = uniqueWords;

}

const string &Document::getDatePublished() const {
    return datePublished;
}

void Document::setDatePublished(const string &datePublished) {
    Document::datePublished = datePublished;
}

const string &Document::getPublisher() const {
    return publisher;
}

void Document::setPublisher(const string &publisher) {
    Document::publisher = publisher;
}

const string &Document::getId() const {
    return ID;
}

void Document::setId(const string &id) {
    ID = id;
}

void Document::setTitle(const string &title) {
    Document::title = title;
}

void Document::setAuthor(const string &author) {
    Document::author = author;
}

void Document::setAbstract(const string &abstract) {
    Document::abstract = abstract;
}

void Document::setBody(const string &body) {
    Document::body = body;
}

void Document::setWords(int words) {
    Document::words = words;
}

Document::~Document() {}

Document::Document(const Document &copy) {
    this->title = copy.title;
    this->author = copy.author;
    this->abstract = copy.abstract;
    this->body = copy.body;
}

Document& Document::operator=(const Document &copy) {
    if (this == &copy) {
        return *this;
    }
    this->title = copy.title;
    this->author = copy.author;
    this->abstract = copy.abstract;
    this->body = copy.body;
    return *this;
}

string& Document::getID() {
    return ID;
}

string& Document::getTitle() {
    return title;
}

string& Document::getAuthor() {
    return author;
}

string& Document::getAbstract() {
    return abstract;
}

string& Document::getBody() {
    return body;
}

int Document::getWords() {
    return words;
}