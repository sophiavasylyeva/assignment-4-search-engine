#include "IndexHandler.h"
#include "Doc.h"
#include <cassert>


//TIMING 
    // std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    // start= std::chrono::high_resolution_clock::now();

    // end=std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> time= end-start;
    // cout << time.count();

void IndexHandler::addWord(string word, int x){
    /**
     * Insert the following into AvlTree "words":
     *    - Stemmed/stopped word from DocParser
     *    - The position of the document in "data" that will be used to find the specific document
     *    - The stemmed/stopped word's relevency number
    */
    words.insert(word, data.size(), x);
}

void IndexHandler::addPeople(string person, int x){
    /**
     * Insert the following into AvlTree "people":
     *    - A person's name from DocParser
     *    - The position of the document in "data" that will be used to find the specific document
     *    - The relevency number of the person's name (this will always be 1)
    */
    people.insert(person, data.size(), x);
}

void IndexHandler::addOrganization(string orz, int x){
    /**
     * Insert the following into AvlTree "people":
     *    - An organization's name from DocParser
     *    - The position of the document in "data" that will be used to find the specific document
     *    - The relevency number of the organization's name (this will always be 1)
    */
    organization.insert(orz, data.size(), x);
}


vector<pair<Doc, int>> IndexHandler::getWords(string s){
    map<int,int> docs = words.find(s);
    vector<pair<Doc, int>> allDocs;
    for (const auto& it : docs) {
        allDocs.push_back(pair<Doc,int>(data.at(it.first), it.second)); // FIXME: Getting segfault here. it.first is looking outside of the size of data.
    }
    return allDocs;
}

vector<pair<Doc, int>> IndexHandler::getPeople(string s){
    map<int, int> docs = people.find(s);
    vector<pair<Doc, int>> allDocs;
    for (const auto& it : docs) {
        allDocs.push_back(pair<Doc,int>(data[it.first], it.second));
    }
    return allDocs;

}

vector<pair<Doc, int>> IndexHandler::getOrganizations(string s){
    map<int, int> docs = organization.find(s);
    vector<pair<Doc, int>> allDocs;
    for (const auto& it : docs) {
        allDocs.push_back(pair<Doc,int>(data[it.first], it.second));
    }
    return allDocs;
}

void IndexHandler::addDoc(Doc doc){
    data.push_back(doc);
}

int IndexHandler::getNumDocs() {
    return data.size();
}

size_t IndexHandler::getTotalWords() {
    return words.treeSize();
}

void IndexHandler::createPersistence() {
    createWords();
    createPeople();
    createOrganizations();
    createDocuments();
}

void IndexHandler::createWords() {
    ofstream outfileWords("../words.txt");
    words.saveTree(outfileWords);
    words.makeEmpty();
}

void IndexHandler::createPeople() {
    ofstream outfilePeople("../people.txt");
    people.saveTree(outfilePeople);
    people.makeEmpty();
}

void IndexHandler::createOrganizations() {
    ofstream outfileOrganizations("../organizations.txt");
    organization.saveTree(outfileOrganizations);
    organization.makeEmpty();
}

void IndexHandler::createDocuments() {
    ofstream outfileDocuments("../documents.txt");
    for (size_t i = 0; i < data.size(); i++) {
        outfileDocuments << data.at(i).uuid << " ";
        outfileDocuments << data.at(i).title << "^" << " ";
        outfileDocuments << data.at(i).publication << "`" << " ";
        outfileDocuments << data.at(i).date << " ";
        outfileDocuments << data.at(i).totalWordCount << " ";
        outfileDocuments << data.at(i).rawText << "|" << " ";
        outfileDocuments << endl;
    }
}

void IndexHandler::loadPersistence() {
    loadWords();
    loadPeople();
    loadOrganizations();
    loadDocuments();
}

void IndexHandler::loadWords() {
    ifstream fileWords;
    fileWords.open("../words.txt");
    if (fileWords.is_open() == false) {
        throw invalid_argument("Cannot open 'words.txt'");
    }

    string currentLine;
    string key;
    int value;
    int relevency;

    while (getline(fileWords, currentLine)) {
        stringstream ss(currentLine);
        ss >> key;
        
        while(!ss.eof()) {
            ss >> value;
            ss >> relevency;
            
            words.insert(key, value, relevency);
        }
    }
}

void IndexHandler::loadPeople() {
    ifstream filePeople;
    filePeople.open("../people.txt");
    if (filePeople.is_open() == false) {
        throw invalid_argument("Cannot open 'people.txt'");
    }

    string currentLine;
    string key;
    int value;
    int relevency;

    while (getline(filePeople, currentLine)) {
        stringstream ss(currentLine);
        ss >> key;
        
        while(!ss.eof()) {
            ss >> value;
            ss >> relevency;
            
            people.insert(key, value, relevency);
        }
    }
}

void IndexHandler::loadOrganizations() {
    ifstream fileOrganizations;
    fileOrganizations.open("../organizations.txt");
    if (fileOrganizations.is_open() == false) {
        throw invalid_argument("Cannot open 'organizations.txt'");
    }

    string currentLine;
    string key;
    int value;
    int relevency;

    while (getline(fileOrganizations, currentLine)) {
        stringstream ss;
        ss << currentLine;
        ss >> key;
        
        while(!ss.eof()) {
            ss >> value;
            ss >> relevency;
            
            organization.insert(key, value, relevency);
        }
    }
}

void IndexHandler::loadDocuments() {
    ifstream fileDocuments;
    fileDocuments.open("../documents.txt");
    if (fileDocuments.is_open() == false) {
        throw invalid_argument("Cannot open 'documents.txt'");
    }

    string currentLine;
    string uuid;
    string title;
    string publication;
    string date;
    int totalWordCount;
    string rawText;
    while (getline(fileDocuments, currentLine)) {
        stringstream ss;
        ss << currentLine;
        ss >> uuid;
        getline(ss, title, '^');
        getline(ss, publication, '`');
        ss >> date;
        ss >> totalWordCount;
        getline(ss, rawText, '|');
        Doc document(uuid, title, publication, date, totalWordCount, rawText);
        data.push_back(document);
    }
}