// owner: Jay Kynerd
// 1.0: run

#include "SearchEngine.h"

SearchEngine::SearchEngine() : index(), parser(index), query(index) {}

void SearchEngine::run(const string &directory) {
    int status = 0;

    while (true) {
        string search;
        search = query.getQuery(status);
        if (status == 0) { // status 0: program entry/index is cleared
            if (search == "*** search") {
                parser.openDirectory(directory);
                status = 1; // status = 1 if index is populated
            } else if (search == "*** quit") {
                break;
            }
        } else if (status == 1) { // status 1: index is populated, ready to search
            if (search == "*** save") {
                index.saveIndex();
            } else if (search == "*** clear") {
                index.clearIndex();
                status = 0; // status = 0 if user clears index
            } else if (search == "*** quit") {
                break;
            } else {
                if (query.printResults(search)) {
                    status = 2; // status = 2 if search results are found
                }
            }
        } else if (status == 2) { // status 2: query has been entered
            if (search == "*** new") {
                status = 1; // status = 1 for new search
            } else if (search == "*** quit") {
                break;
            } else {
                int result = stoi(search); // converts query to int to find result
                if (!query.printResult(result)) {
                    status = 3; // status = 3 if result number invalid
                }
            }
        } else if (status == 3) { // status 3: invalid result number entered
            if (search == "*** new") {
                status = 1; // status = 1 for new search
            } else if (search == "*** quit") {
                break;
            } else {
                int result = stoi(search);
                if (query.printResult(result)) {
                    status = 2; // status = 2 if valid result number is entered
                }
            }
        }
    }

    if (status != 0) { // prints stats if index is not empty
        index.printStats();
    }
}