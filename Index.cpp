// owner: Jay Kynerd
// 1.0: constructor, rule of 3
// 2.0: addWord, addDocument
// 2.5: contains, getDocuments
// 3.0: addAuthor, addDocToAuthor, getAuthorDocs
// 4.0: getSize, numDocuments
// 5.0: saveIndex, clearIndex, printStats, calcMostFrequentWords

#include <map>
#include "Index.h"

Index::Index() = default;

Index::Index(const Index &copy) {
    this->words = copy.words;
    this->authors = copy.authors;
}

Index &Index::operator=(const Index &copy) {
    if (this == &copy) {
        return *this;
    }
    this->words = copy.words;
    this->authors = copy.authors;
    return *this;
}

Index::~Index() = default;

bool Index::addWord(const string& word, const string& docName, string unstemmedWord) {
    if (words.exists(word)) { // adds new document ID to existing word
        return addDocument(word, docName); // returns false if document already added for word, true otherwise
    } else { // creates new word if otherwise
        Entry newWord = Entry(word, docName);
        newWord.setUnstemmedWord(unstemmedWord);
        words.insert(newWord);
        return true;
    }
}

bool Index::addDocument(const string &word, string docName) { // adds ID to word
    return words.find(word).addDocument(docName);
}

void Index::addAuthor(const string& author, const string& docName) {
    if (authors.exists(author)) { // adds new document ID to existing author
        addDocToAuthor(author, docName);
    } else { // creates new author otherwise
        Entry newWord = Entry(author, docName);
        authors.insert(newWord);
    }
}

void Index::addDocToAuthor(const string &author, string docName) { // adds ID to author
    authors.find(author).addDocument(docName);
}

bool Index::contains(const string &word) { // returns true if word exists in index, false otherwise
    return words.exists(word);
}

bool Index::containsAuthor(const string &author) { // returns true if author exists in index, false otherwise
    return authors.exists(author);
}

vector<string> Index::getDocuments(const string &word) { // returns document IDs in which word appears
    return words.find(word).getDocuments();
}

vector<string> Index::getAuthorDocs(const string &author) { // returns document IDs by author
    return authors.find(author).getDocuments();
}

int Index::getSize() { // returns size of AVLTree (number of unique words)
    return words.size();
}

int Index::numDocuments(const string &word) { // returns number of documents containing word
    if (words.exists(word)) {
        auto temp = words.find(word);
        return temp.numDocuments();
    }
    return 0;
}

vector<Entry> Index::saveIndex() { // returns vector of entries
    return words.save();
}

void Index::clearIndex() { // clears index contents
    words.clear();
}

void Index::printStats() { // index stats
    cout << "Unique words indexed: " << words.size() << endl;
    cout << "Unique authors indexed: " << authors.size() << endl;
    cout << "The top 50 words were: " << endl;
    auto topWords = calcMostFrequentWords();
    for (const auto & word: topWords) {
        cout << word << endl;
    }
}

vector<string> Index::calcMostFrequentWords() {
    auto entries = words.save(); // gets all entries
    map<int, string> commonWords;

    // adds each entry to a map to sort by vector size (number of occurrences)
    for (auto & entry: entries) {
        commonWords.emplace(entry.numDocuments(), entry.getUnstemmedWord());
    }

    auto it = commonWords.rbegin();
    vector<string> top50;

    for (int i = 0; i < 50; i++) { // puts top 50 words in vector
        top50.push_back(it->second);
        it++;
    }
    return top50;
}