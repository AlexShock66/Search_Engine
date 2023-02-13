#include "Entry.h"

Entry::Entry() = default;

Entry::Entry(string word) {
    data.first = word;
}

Entry::Entry(string word, string document) {
    this->data.first = word;
    this->data.second.push_back(document);
}

Entry::Entry(const Entry &copy) {
    this->data.first = copy.data.first;
    this->data.second = copy.data.second;
    this->unstemmedWord = copy.unstemmedWord;
}

Entry& Entry::operator=(const Entry &copy) {
    if (this == &copy) {
        return *this;
    }
    this->unstemmedWord = copy.unstemmedWord;
    this->data.first = copy.data.first;
    this->data.second = copy.data.second;
    return *this;
}

const string &Entry::getUnstemmedWord() const {
    return unstemmedWord;
}

void Entry::setUnstemmedWord(const string &unstemmedWord) {
    Entry::unstemmedWord = unstemmedWord;
}

Entry::~Entry() = default;

bool Entry::addDocument(const string &docName) { // pushes back the doc id to the vector<string>
    if (!containsDoc(docName)) {
        data.second.push_back(docName);
        return true;
    } else {
        return false;
    }
}

string Entry::getWord() {
    return data.first;
}

vector<string> Entry::getDocuments() {
    return data.second;
}

int Entry::numDocuments() {
    return data.second.size();
}

bool Entry::operator==(const string &comparison) const {
    return this->data.first == comparison;
}

bool Entry::operator==(const Entry &comparison) const {
    return this->data.first == comparison.data.first;
}

bool Entry::operator<(const string &comparison) const {
    return this->data.first < comparison;
}

bool Entry::operator<(const Entry &comparison) const {
    return this->data.first < comparison.data.first;
}

bool Entry::operator>(const string &comparison) const {
    return this->data.first > comparison;
}

bool Entry::operator>(const Entry &comparison) const {
    return this->data.first > comparison.data.first;
}

bool Entry::containsDoc(const string &docToCheck) {
    return std::find(data.second.begin(), data.second.end(), docToCheck) != data.second.end();
}

ostream& operator<<(ostream& os, const Entry& dt)
{
    os << dt.data.first;
    return os;
}
