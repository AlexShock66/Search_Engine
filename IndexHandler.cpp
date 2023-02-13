// owner: Jay Kynerd
// 1.0 constructor, addWord, isStopWord
// 1.5 rule of 3
// 2.0 getStem, getDocumentIDs
// 2.5 formatWord
// 2.5.1 formatWords
// 3.0 getIntersection, getUnion
// 4.0 getDisunion

#include "IndexHandler.h"

IndexHandler::IndexHandler():documents(15000) {
    totalDocuments = 0;
    theIndex = new Index;
    sumOfUniqueWords = 0;
    ifstream file("stopWords.txt");
    if (file.is_open()) { // populates AVLTree of stop words
        string temp;
        while (file >> temp) {
            stopWords.insert(temp);
        }
    } else {
        cout << "error reading stop words" <<endl;
    }
    file.close();
}

IndexHandler::IndexHandler(const IndexHandler &copy) {
    this->totalDocuments = copy.totalDocuments;
    this->theIndex = copy.theIndex;
    this->stemmedWords = copy.stemmedWords;
    this->documents = copy.documents;
    this->stopWords = copy.stopWords;
    this->sumOfUniqueWords = copy.sumOfUniqueWords;
}

IndexHandler &IndexHandler::operator=(const IndexHandler &copy) {
    if (this == &copy) {
        return *this;
    }
    this->theIndex = copy.theIndex;
    this->stemmedWords = copy.stemmedWords;
    this->documents = copy.documents;
    this->stopWords = copy.stopWords;
    this->sumOfUniqueWords = copy.sumOfUniqueWords;
    return *this;
}

IndexHandler::~IndexHandler() {
    delete theIndex;
//    for (int i = 0; i < 15000; i++) {
//        auto temp =  documents.at(i);
//        for (auto& pointer: temp) {
//
//        }
//    }
}

bool IndexHandler::addWord(string newWord, const string &docID) {
    formatWord(newWord);
    string unstemmedWord = newWord;
    if (!isStopWord(newWord) && !newWord.empty()) { // adds word if not a stop word/digit/unnecessary/empty
        newWord = getStem(newWord);
        return theIndex->addWord(newWord, docID,unstemmedWord); // returns true if docID doesn't already exist for word, false otherwise
    }
    return false; // returns false if word exists
}

void IndexHandler::addAuthor(string author, const string &docID) {
    formatWord(author);
    theIndex->addAuthor(author, docID);
}

void IndexHandler::incrementNumDocs() { // increments number of documents parsed
    totalDocuments++;
}

void IndexHandler::addDocument(const string &docID, Document *doc) { // adds document ID to existing word
    documents.insert(docID, doc);
}

long IndexHandler::getSumOfUniqueWords() const {
    return sumOfUniqueWords;
}
void IndexHandler::addToTotalUniqueWords(int a) {
    sumOfUniqueWords += a;
}

bool IndexHandler::isStopWord(const string &word) { // checks if word is digit/unnecessary or exists in stopWords.txt
    bool isNotWord = (ispunct(word.front()) || isdigit(word.front())) || (ispunct(word.back()) || isdigit(word.back()));
    return stopWords.exists(word) || isNotWord;
}

string IndexHandler::getStem(const string &word) {
    if (stemmedWords.find(word) != stemmedWords.end()) { // checks if word has been stemmed previously
        return stemmedWords.find(word)->second;
    } else { // adds word and its stem to map otherwise
        string temp = word;
        Porter2Stemmer::stem(temp);
        stemmedWords.emplace(word, temp);
        return temp;
    }
}

void IndexHandler::formatWord(string &word) { // removes punctuation and converts characters to lowercase
    auto it = word.begin();
    for (int i = 0; i < word.size() + 1; i++) { // iterates over every char
        if (ispunct(*it)) { // erases punctuation
            word.erase(it);
        }
        *it = tolower(*it); // converts char to lowercase
        ++it;
    }
}

void IndexHandler::formatWords(vector<string> &words) { // calls formatWord for each word in vector
    for (auto & word : words) {
        formatWord(word);
    }
}

vector<string> IndexHandler::getDocumentIDs(string word) { // returns document IDs in which word is found
    // formats/stems word
    formatWord(word);
    word = getStem(word);

    vector<string> docs;

    if (theIndex->contains(word)) { // checks if index contains word
        docs = theIndex->getDocuments(word);
    }
    return docs;
}

vector<Document*> IndexHandler::getDocuments(const vector<string> &docIDs) { // returns Document pointers for each ID in vector
    vector<Document*> docs;
    for (const auto &docID: docIDs) { // iterates over each document ID
        Document *temp = documents.get(docID);
        int index = 1;
        while (temp->getID() != docID) { // checks for collisions in hash table
            temp = documents.get(docID, index++);
        }
        docs.push_back(temp);
    }
    return docs;
}

vector<string> IndexHandler::getIntersection(vector<string> words) { // returns Document IDs in which all words in vector appear
    // formats/stems each word
    formatWords(words);
    for (auto &word: words) {
        word = getStem(word);
    }

    vector<string> intersection;
    unordered_set<string> cache;

    for (int i = 0; i < words.size(); i++) {
        auto current = getDocumentIDs(words.at(i)); // gets document IDs of current word

        if (i == 0) { // populates set with all IDs containing first word
            cache.insert(current.begin(), current.end());
        }

        for (const auto &doc: current) { // iterates over each document ID containing current word
            if (cache.count(doc) > 0) { // if ID appeared for previous word, add to vector
                intersection.push_back(doc);
            }
        }

        if (i < words.size() - 1) { // resets set after each word/clears vector for next word
            cache.clear();
            cache.insert(intersection.begin(), intersection.end());
            intersection.clear();
        }
    }
    return intersection;
}

vector<string> IndexHandler::getUnion(vector<string> words) {
    // formats/stems each word
    formatWords(words);
    for (auto &word: words) {
        word = getStem(word);
    }

    vector<string> _union;
    unordered_set<string> cache;

    for (const auto& word: words) { // iterates over every word
        auto current = theIndex->getDocuments(word); // gets IDs containing current word
        cache.insert(current.begin(), current.end()); // adds IDs to set to ensure no duplicates
    }

    for (const auto& document: cache) { // adds each ID in set to vector
        _union.push_back(document);
    }

    return _union;
}

vector<string> IndexHandler::getDisunion(vector<string> docIDs, string word) { // removes document IDs containing word
    // formats/stems word
    formatWord(word);
    word = getStem(word);

    auto docsToRemove = getDocumentIDs(word); // gets IDs to remove
    unordered_set<string> cache{docsToRemove.begin(), docsToRemove.end()}; // creates set of IDs to remove

    for (auto it = docIDs.begin(); it != docIDs.end(); ++it) { // iterates over each ID in vector
        if (cache.count(*it) > 0) { // removes current ID from vector if it exists in set
            docIDs.erase(it--);
        }
    }

    return docIDs;
}

vector<string> IndexHandler::filterAuthor(const vector<string> &docIDs, string author) { // removes document IDs by author
    // formats author string
    formatWord(author);

    vector<string> documentsByAuthor;

    if (theIndex->containsAuthor(author)) {
        auto authorIDs = theIndex->getAuthorDocs(author); // gets IDs written by author

        unordered_set<string> cache{authorIDs.begin(), authorIDs.end()}; // creates set of author IDs

        for (auto& docID: docIDs) { // iterates over each ID in vector
            if (cache.count(docID) > 0) { // adds ID to vector if it exists in set
                documentsByAuthor.push_back(docID);
            }
        }
    } else {
        cout << "Author " << author << " not found." << endl;
    }
    return documentsByAuthor;
}

int IndexHandler::documentFrequency(string word) { // returns number of documents containing word
    formatWord(word);
    word = getStem(word);

    int freq = 0;

    if (theIndex->contains(word)) {
        freq = theIndex->numDocuments(word);
    }

    return freq;
}

// returns vector containing top 15 results
vector<Document*> IndexHandler::scoreDocuments(const vector<Document*> &docsToScore, vector<string> words) {
    vector<Document*> scoredDocuments;
    vector<pair<double, Document*>> sorter;

    for (const auto& doc: docsToScore) { // iterates over each document in vector
        // creates stringstream for title, abstract, and body
        stringstream titleWords(doc->getTitle());
        stringstream abstractWords(doc->getAbstract());
        stringstream bodyWords(doc->getBody());

        double docScore = 0;

        for (auto& word: words) { // iterates over each word for which to score document
            // formats/stems word
            formatWord(word);
            word = getStem(word);

            // idf = log(# total documents / # documents containing word)
            double idf = log((double) totalDocuments / (double) documentFrequency(word));

            if (idf == 0) { // ensures unique score for each document
                idf = 1;
            }

            double temp = 0;
            string buffer;

            while (titleWords >> buffer) {
                formatWord(buffer);
                if (getStem(buffer) == word) { // weights title occurrences 10x
                    temp += 10;
                }
            }

            while (abstractWords >> buffer) {
                formatWord(buffer);
                if (getStem(buffer) == word) { // weights abstract appearances 5x
                    temp += 5;
                }
            }

            while (bodyWords >> buffer) {
                formatWord(buffer);
                if (getStem(buffer) == word) { // unweighted for body occurrences
                    temp++;
                }
            }

            temp *= idf; // (weighted) term frequency * IDF

            docScore += temp; // adds score for each word to overall document score
        }
        sorter.emplace_back(docScore, doc); // adds pair of score and Document pointer for sorting
    }

    sort(sorter.begin(), sorter.end()); // sorts results

    auto it = sorter.rbegin();

    for (int i = 0; i < 15; i++) { // inserts top 15 occurrences or all if occurrences < 15
        if (it == sorter.rend()) {
            break;
        } else {
            scoredDocuments.push_back(it->second);
            it++;
        }
    }
    return scoredDocuments;
}

void IndexHandler::saveIndex() { // writes contents of index to output file
    ofstream output("Index.txt");
    auto library = theIndex->saveIndex();
    for (auto& entry: library) {
        output << entry.getWord() << ": " << endl;
        auto temp = entry.getDocuments();
        for (const auto& docID: temp) {
            output << "     " << docID << endl;
        }
    }
    output.close();
}

void IndexHandler::clearIndex() { // clears contents without deleting index
    theIndex->clearIndex();
}

void IndexHandler::printStats() {
    cout << '\n' << "Documents parsed: " << totalDocuments << endl;
    cout << "Words indexed per document: " << sumOfUniqueWords / totalDocuments << endl;
    theIndex->printStats();
}