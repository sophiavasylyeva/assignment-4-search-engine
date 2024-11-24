#ifndef IndexHandler_H
#define IndexHandler_H
#include <map>
#include <vector>
#include <sstream>
#include <chrono>
#include <fstream>
#include <string>
#include "Hashing.h"
#include "DS_AVLTree.h"
#include "Doc.h"

//pseudocode
    //setters/getters for index
    //store all docs
    //will use - store all needed data in vector for quick and easy access (internal storage)
    //persistence
    //store stats

class IndexHandler
{
    private:
        AvlTree <string, int> people;
        AvlTree <string, int> organization;
        AvlTree <string, int> words;
        std::vector<Doc> data; //internal storage
        void writeFromFile(string);
        void createWords();
        void createPeople();
        void createOrganizations();
        void createDocuments();
        void loadWords();
        void loadPeople();
        void loadOrganizations();
        void loadDocuments();
    public:
        vector<pair<Doc, int>> getWords(string);
        vector<pair<Doc, int>> getPeople(string);
        vector<pair<Doc, int>> getOrganizations(string);
        void addWord(string, int);
        void addPeople(string, int);
        void addOrganization(string, int);
        void addDoc(Doc);
        void createPersistence();
        void loadPersistence();
        int getNumDocs();
        void getDataSize();
        size_t getTotalWords();
};


#endif