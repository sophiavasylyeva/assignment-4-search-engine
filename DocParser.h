#ifndef DocParser_H
#define DocParser_H
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <iomanip>
#include <unordered_map>
#include "porter2_stemmer.h"
#include <regex>
#include "Doc.h"
#include "IndexHandler.h"



// RapidJSON headers we need for our parsing.
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"

using namespace rapidjson;
using namespace std;
using namespace Porter2Stemmer;


class DocParser
{
private:
    std::vector<string> stop;
    IndexHandler* index;
public:
    void stopWords();
    void setIndex(IndexHandler*);
    void testReadJsonFile(const string &fileName);
    vector<string> tokenizer(string);
    void createPersistence();
    void loadPersistence();
    void testFileSystem(const string &path);
    size_t getDataSize();
    size_t getTotalWords();
};

// given folder path, get all file names in folder: input, given function
// parse all JSONs in file names: modify given function
    // tokenize - lowercase: froms sentiment analyzer
    // stemming: porterZ
    // store processed data in index: call index handler 
    // remove stop words: store list in csv/txt and parse in

    //later: 
    // store relevancy data 
    // persistence restructuring 



// Function Prototypes


/**
 * example code that reads and parses a json file and extracts the title and person
 * entities.
 * @param fileName filename with relative or absolute path included.
 */
void DocParser::testReadJsonFile(const string &fileName)
{

    // open an ifstream on the file of interest and check that it could be opened.
    ifstream input(fileName);
    if (!input.is_open())
    {
        cerr << "cannot open file: " << fileName << endl;
        return;
    }

    // Create a RapidJSON IStreamWrapper using the file input stream above.
    IStreamWrapper isw(input);

    // Create a RapidJSON Document object and use it to parse the IStreamWrapper object above.
    Document d;
    d.ParseStream(isw);

    // Now that the document is parsed, we can access different elements the JSON using
    // familiar subscript notation.

    // This accesses the -title- element in the JSON. Since the value
    //  associated with title is a string (rather than
    //  an array or something else), we call the GetString()
    //  function to return the actual title of the article
    //  as a c-string.

    string id = d["uuid"].GetString(); //string uuid
    cout << endl;


    string title = d["title"].GetString(); //title
    cout << endl;

    auto persons = d["entities"]["persons"].GetArray(); //persons
    cout << "  Person Entities + sentiment:"
         << "\n";
    for (auto &p : persons)
    {
        string temp;
        stringstream ss(p["name"].GetString());
        while (getline(ss, temp, ' ')) {
            index->addPeople(temp, 1); // Add the name of ONE person from document to the AvlTree "people"
        }
    }

    cout << endl;


    auto organization = d["entities"]["organizations"].GetArray(); //organizations
    cout << "  Organization Entities + sentiment:"
         << "\n";
    for (auto &o : organization)
    {
         string temp;
         stringstream ss(o["name"].GetString());
         while (getline(ss, temp, ' ')) {
            index->addOrganization(temp, 1);
        }
    }

    cout << endl;

    
    string publication = d["thread"]["site"].GetString(); //publication
    cout << endl;

    string date = d["thread"]["published"].GetString(); //date
    cout << endl;
    
    string text = d["text"].GetString();            //rawText  
    text = regex_replace(text, regex("\n"), "");
    vector <string> tokens = tokenizer(text);
    int wordCount = 0;
    std::unordered_map<string, int> words;  
    for(size_t i = 0; i < tokens.size(); i++){
        trim(tokens[i]);
        stem(tokens[i]);
        if(find(stop.begin(), stop.end(), tokens[i]) == stop.end()){
            wordCount++;                                //totalWordCount
            if(words.find(tokens[i]) == words.end()) 
                words[tokens[i]] = 1;
            else{
                words[tokens[i]]++;
            }
    }

    }
     for(const auto& it: words){
        wordCount+= it.second;
        index->addWord(it.first, it.second);
    }
 
    Doc doc(id, title, publication, date, wordCount, text);
    index->addDoc(doc);

    input.close();
}

/**
 * example code for how to traverse the filesystem using std::filesystem
 * which is new for C++17.
 *
 * @param path an absolute or relative path to a folder containing files
 * you want to parse.
 */
void DocParser::testFileSystem(const string &path) //DONT TOUCH    
{

    // recursive_director_iterator used to "access" folder at parameter -path-
    // we are using the recursive iterator so it will go into subfolders.
    // see: https://en.cppreference.com/w/cpp/filesystem/recursive_directory_iterator
    auto it = filesystem::recursive_directory_iterator(path);

    // loop over all the entries.
    for (const auto &entry : it)
    {

        cout << "--- " << setw(60) << left << entry.path().c_str() << " ---" << endl;

        // We only want to attempt to parse files that end with .json...
        if (entry.is_regular_file() && entry.path().extension().string() == ".json")
        {
            testReadJsonFile(entry.path().string());
        }
    }
}



std::vector<string> DocParser::tokenizer(string x){ 
    size_t start = 0;
    std::vector<string> s;
    for(size_t i  = 0; i < x.length(); i++){
        if(!isalpha(x[i])){
            if(start < i)
                s.push_back(x.substr(start, i - start));  
            start = i + 1;
        }
    }
    if(start < x.length()) //switched from len
        s.push_back(x.substr(start,x.length() - start));  
    return s;
}

void DocParser::stopWords(){
    //store stopwords in vector
     //internal storage
     stop.clear();
    fstream filename;
    filename.open("stopWords.txt",ios::in); //open a file to perform read operation using file object
    if (filename.is_open()){ //checking whether the file is open
      string tp;
      while(getline(filename, tp)){ //read data from file object and put it into string.
         //cout << tp << "\n"; //print the data of the string
         trim(tp);
         stem(tp);
         stop.push_back(tp);
      }
      filename.close(); //close the file object.
    }
}

void DocParser::setIndex(IndexHandler* i){
    index = i;
}

void DocParser::createPersistence(){
    index->createPersistence();
}

void DocParser::loadPersistence(){
    index->loadPersistence();
}

size_t DocParser::getDataSize(){
    return index->getNumDocs();
}

size_t DocParser::getTotalWords(){
    return index->getTotalWords();
}

#endif