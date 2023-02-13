// owner: Alex Shockley
/*
 *  1.0 : created the document parser and added functionality to open a directory and traverse through every file in said directory
 *  1.1 : Finally figured out how json files worked and added functionality to create document objects and fill them with the correct data based on a json file and pass said document objects to the index
 *  1.2 : Now able to read the metadata file.csv that I didnt know existed prior to today, and can now get the publisher and date published from a given article
 */
#include "DocumentParser.h"

DocumentParser::DocumentParser(IndexHandler &i) : idx(i) {} // constructor that initializes the indexHandler

DocumentParser::DocumentParser(const DocumentParser &copy) : idx(copy.idx) {} // basic copy constructor

DocumentParser &DocumentParser::operator=(const DocumentParser &copy) { // basic = operator
    if (this == &copy) {
        return *this;
    }
    this->idx = copy.idx;
    return *this;
}

DocumentParser::~DocumentParser() = default; // destructor

void DocumentParser::openDirectory(const string &directory) { // Now this is where the fun begins
    string filepath;
    string num;
    ifstream fin;
    ifstream metaDataStream("metadata-cs2341.csv"); // get the metaData file so we can find the publisher and publish date
    if(metaDataStream.is_open()) { // make sure that the stream is open
        string trash; // i still dont know how to use .ignore :)
        getline(metaDataStream,trash); // get the title line
        while(!metaDataStream.eof()) {
            string docID;
            string date;
            string publisher;
            getline(metaDataStream,trash,',');
            getline(metaDataStream,docID,',');
            getline(metaDataStream,trash,',');
            getline(metaDataStream,trash,',');
            getline(metaDataStream,trash,',');
            getline(metaDataStream,trash,',');
            getline(metaDataStream,trash,',');
            getline(metaDataStream,trash,',');
            getline(metaDataStream,trash,',');
            getline(metaDataStream,date,',');
            getline(metaDataStream,trash,',');
            getline(metaDataStream,publisher,',');
            getline(metaDataStream,trash);
            metaData.emplace(docID,pair<string,string>(date,publisher)); // fill a map with values that are of type, string, string so that we can easily search them
        }
    }
    DIR *dp; // This... Is a thing that we need for some reason
    struct dirent *dirp; // the directory file
    struct stat filestat;

    dp = opendir(directory.c_str() ); // open the directory
    if (dp == nullptr) // check and see if there was an error opening a given directory
    {
        cout << "Error(" << errno << ") opening " << directory << endl;
    }

    int i = 0; // iterator to limit the max number of files during testing
    while ((dirp = readdir( dp )) != nullptr) // while files exist in the directory
    {
        if ( dirp  == nullptr)
            break;

        filepath = directory + "/" + dirp->d_name;

        // If the file is a directory (or is in some way invalid) we'll skip it
        if (stat( filepath.c_str(), &filestat )) continue;
        if (S_ISDIR( filestat.st_mode ))         continue;



        if(filepath.find("json") == string::npos) continue; // for some reason, the directory thinks the metaData file is still in there so this is needed to stop it from converting a csv to json

            fin.open( filepath.c_str()); // open the specific json file
            parseDocument(fin); // pass it to a function that parses it

            cout << filepath << ": " << i << endl;
        fin.close(); // be sure to close files
        i++;
    }
    closedir( dp ); // closing the directory
}

void DocumentParser::parseDocument(ifstream &input) { // takes in a json file and parses it to a document object
   if(input.is_open()) {
       json j;
       string id;
       string title;
       string authors;
       string abstract;
       string body;
       int uniqueWords = 0;
       idx.incrementNumDocs();
       input >> j;

       id = j["paper_id"].get<string>(); // gets the id

       title = j["metadata"]["title"].get<string>(); // gets the title in  "metaData"

       stringstream SS;

       SS << title;
       string currAuthor;
       auto docAuthors = j["metadata"]["authors"].get<vector<json>>(); // gets the list of authors in metadata

       for (int i = 0; i < docAuthors.size(); i++) { // iterate through the vector of authors and adds them to a single string
           currAuthor = docAuthors.at(i)["last"].get<string>();
           if (!currAuthor.empty()) {
               idx.addAuthor(currAuthor, id); // adds the author to the Author index in the index handler
               authors += currAuthor;
               if (i != docAuthors.size() - 1) { // seperates the authors with a comma for displaying later in the program
                   authors += ", ";
               }
           }
       }
       //SS << authors;

       auto abstractText = j["abstract"].get<vector<json>>();
       for (const auto &paragraph: abstractText) {
           abstract += paragraph["text"].get<string>() + '\n';
       }
       SS << abstract;

       auto bodyText = j["body_text"].get<vector<json>>();
       for (const auto &paragraph: bodyText) {
           body += paragraph["text"].get<string>() + "\n";
       }
       SS << body;

       string temp;

       while (SS >> temp) {
           if (idx.addWord(temp, id)) { // if the word exists already, add it to the vector but dont incriment unique words, else add it to the AVL tree and
               uniqueWords++;

           }
       }

       idx.addToTotalUniqueWords(uniqueWords); //adds the uniqueWords variable to the total num of unique words

       auto *newDoc = new Document(id, title, authors, abstract, body, uniqueWords); // create document pointer and assign values

       string date = metaData.find(id)->second.first; // code to make sure the document contains the publisher and publish date
       string pub = metaData.find(id)->second.second;
       newDoc->setDatePublished(date);
       newDoc->setPublisher(pub);

       idx.addDocument(id, newDoc); // add the document to the index
   }
}