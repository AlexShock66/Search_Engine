//
// Created by Alex Shockley on 11/30/2020.
//

#include <sstream>
#include "SearchQuery.h"

SearchQuery::SearchQuery(IndexHandler &i) : idx(i) { }

string SearchQuery::getQuery(int status) { // Takes the user input and determines what functions to call based on it
    string userInput;

    if (status == 0) { // startup call, runs before anything has been parsed to prompt the user to fill the Index
        cout << '\n' << R"(Enter "SEARCH" to run search engine or "QUIT" to exit.)" << endl;
        while (true) {
            cout << '-';
            getline(cin, userInput);
            if (userInput == "SEARCH") { // if the user wants to search, return value that denotes a search
                return "*** search";
            } else if (userInput == "QUIT") { // same for quit
                return "*** quit";
            } else {
                cout << R"(Please enter "SEARCH" or "QUIT.")" << endl;
            }
        }

    } else if (status == 1) { // this runs if the Index has already been filled
        cout << '\n' << R"(Enter your search query, "SAVE" or "CLEAR" to save/clear the search index, or "QUIT" to exit.)" << endl;
        while (true) {
            cout << '-';
            getline(cin, userInput);
            if (userInput == "SAVE"){ //compareIgnoreCase(userInput, "SAVE")) { // returns a value that allows user to save the index
                return "*** save";
            } else if (userInput == "CLEAR"){//compareIgnoreCase(userInput, "CLEAR")) { // returns a value that allows the user to clear the index
                return "*** clear";
            } else if (userInput == "QUIT"){//compareIgnoreCase(userInput, "QUIT")) { // returns a value that allows the user to quit the program
                return "*** quit";
            } else {
                break;
            }
        }

    } else if (status == 2) { // this runs once the search results have come up
        cout << '\n' << R"(Enter a result number to see more info, "NEW" to start a new search, or "QUIT" to exit)" << endl;
        while (true) {
            cout << '-';
            getline(cin, userInput);
            if (userInput == "NEW"){//compareIgnoreCase(userInput, "NEW")) { // allows the user to do a new search
                return "*** new";
            } else if (userInput == "QUIT"){//compareIgnoreCase(userInput, "QUIT")) { // allows the user to quit the program
                return "*** quit";
            } else {
                if (isNumber(userInput)) { // allows the user to see more information on a given article
                    break;
                } else {
                    cout << '\n' << R"(Please enter a valid result number, "NEW," or "QUIT.")" << endl;
                }
            }
        }
    } else if (status == 3) { // runs if the user inputs an incorrect command
        cout << '\n' << R"(Please enter a valid result number, "NEW," or "QUIT.")" << endl;
        while (true) { // wont let the user continue unless a valid command is entered
            cout << '-';
            getline(cin, userInput);
            if (userInput == "NEW") {
                return "*** new";
            } else if (userInput == "QUIT") {
                return "*** quit";
            } else {
                break;
            }
        }
    }

    return userInput;
}

bool SearchQuery::printResults(const string &search) { // function to display the results of a search
    stringstream SS(search);
    string buffer;
    vector<string> docIDs;
    vector<string> words;

    while (SS >> buffer) {
        if (buffer.find("AND") != string::npos) { // if the query contains the word "AND"
            while (SS >> buffer) {
                if (buffer == "NOT" || buffer == "AUTHOR") { // read the words until NOT or AUTHOR or the end of the input is reached and push search terms back to a vector
                    break;
                } else {
                    words.push_back(buffer);
                }
            }

            docIDs = idx.getIntersection(words); // gets the intersection of the search terms and returns a vector<string> that contains all the doc ids where the intersection occured

            do {
                if (buffer == "NOT") { // handle the disunion case
                    SS >> buffer;
                    docIDs = idx.getDisunion(docIDs, buffer); // removes doc IDs that contain a given word and returns the new vector
                }

                if (buffer == "AUTHOR") { // handle the AUTHOR case
                    SS >> buffer;
                    docIDs = idx.filterAuthor(docIDs, buffer); //removes doc IDs that do not contain a certain author and returns the vector
                }
            } while (SS >> buffer); // keep going as long as there are more words in the user's input

        } else if (buffer.find("OR") != string::npos) { // checks to see if the string contains the word "OR"
            while (SS >> buffer) {
                if (buffer == "NOT" || buffer == "AUTHOR") { // continue to add words to the search terms until the end is reached or AUTHOR is reached, or NOT is reached
                    break;
                } else {
                    words.push_back(buffer);
                }
            }

            docIDs = idx.getUnion(words); // returns a vector<string> containing doc Ids that fit the union of every word in the query

            do {
                if (buffer == "NOT") { // handle the disunion case
                    SS >> buffer;
                    docIDs = idx.getDisunion(docIDs, buffer); // removes doc IDs that contain a given word and returns the new vector
                } else if (buffer == "AUTHOR") {
                    SS >> buffer;
                    docIDs = idx.filterAuthor(docIDs, buffer); //removes doc IDs that do not contain a certain author and returns the vector
                }
            } while (SS >> buffer);

        } else { // handles the case that niether OR or AND were requested by the user
            words.push_back(buffer); // push the single word back
            docIDs = idx.getDocumentIDs(buffer); // get every doc that word is contained in

            while (SS >> buffer) {
                if (buffer == "NOT") { // if there is a NOT
                    SS >> buffer;
                    docIDs = idx.getDisunion(docIDs, buffer); // You know the deal by this point, returns the vector after removing every doc ID that contains that word
                } else if (buffer == "AUTHOR") { // handle AUTHOR case
                    SS >> buffer;
                    docIDs = idx.filterAuthor(docIDs, buffer); // removes all doc IDs that dont have a certain author in them
                }
            }
        }
    }

    if (!docIDs.empty()) { // handle the case that results actually came back

        currentSearch = idx.getDocuments(docIDs); // gets the actual document pointers from the index instead of just the IDs
        int numResults = currentSearch.size();
        //cout << "Before ranking: " << documents.size() << endl;
        currentSearch = rankSearches(currentSearch, words); // function that ranks the results based on what words were input

        //cout << "After ranking: " << documents.size() << endl;
        cout << '\n' << "----------Results----------" << endl;
        cout << '\n' << "Found " << numResults << " results:" << '\n' << endl;
        int counter = 1;
        for (const auto& document: currentSearch) { // display the information of up to 15 articles based on relavence
            string title = document->getTitle();
            if (title.empty()) title = "N/a";
            string authors = document->getAuthor();
            if (authors.empty()) authors = "N/a";
            string date = document->getDatePublished();
            if (date.empty()) date = "N/a";
            string publisher = document->getPublisher();
            if (publisher.empty()) publisher = "N/a";

            cout << counter << ". " << title<< endl;
            cout << "     Author(s):      " << authors << endl;
            cout << "     Date published: " << date << endl;
            cout << "     Publisher:      " << publisher << endl;
            cout << endl;

            counter++;
        }
        return true; // true means results were found
    } else { // handle the case that no results were found
        cout << '\n' << "No results found for query: " << search << endl;
        return false;
    }
}

bool SearchQuery::printResult(int resultNumber) { // function to handle if the user wants more information on a given article
    resultNumber--; // get the index of the article in the vector
    if (resultNumber < currentSearch.size() && resultNumber < 15) { // makes sure the user enters a valid number
        cout << endl;
        Document *result = currentSearch.at(resultNumber);
        string printMe = result->getAbstract(); // get the abstract to display information about the document
        if (printMe.empty()) { // if the abstract doesnt exist, use the body
            printMe = result->getBody();
        }
        stringstream SS(printMe);
        string temp;
        for (int i = 1; i <= 300; i++) { // display the abstract OR the body up to 300 words
            if (SS >> temp)
            {
                cout << temp;
                if (i > 250 && temp.find('.') != string::npos) { // if we can find the end of a sentence and the count is already bigger than 250, then stop it there
                    break;
                } else if (i % 25 == 0) { // for formatting, cout an endline
                    cout << '\n';
                } else {
                    cout << " ";
                }
            } else { // if the string is empty, just break out of the loop and stop displaying
                break;
            }
        }
        cout << endl;
        return true;
    }
    return false; // if the user inputs an invalid number
}

vector<Document*> SearchQuery::rankSearches(const vector<Document*> &documents, const vector<string> &words) { // calls the index handler to rank searches based on the search terms
    return idx.scoreDocuments(documents, words);
}

bool SearchQuery::isNumber(const string &stringToCheck) { // function to check if an input is a valid number
    auto it = stringToCheck.begin();
    while (it != stringToCheck.end()) {
        if (!isdigit(*it)) return false;
        it++;
    }
    return true;
}