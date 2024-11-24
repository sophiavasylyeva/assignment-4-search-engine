#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <iostream>
#include <string>
#include <fstream>
#include "DocParser.h"
#include "QueryProcessor.h"

using namespace std;

class UserInterface
{
private:
    DocParser ui_docParser;
    QueryProcessor ui_queryProcessor;
    IndexHandler* ih;
    std::chrono::duration<double> timeIndexing;
    std::chrono::duration<double> timeQuery;

    /**
     * mainMenuFormat() function will do the following:
     *    - Simply output the main menu.
     *        ~ This function does NOT deal with user inputs. Look at the mainMenu() function for user inputs.
    */
    void mainMenuFormat()
    {
    cout << endl << endl;
    cout << "------------ Welcome to Supersearch! ------------" << endl;
    cout << "        [Please enter 1-5 or 0 to exit]        " << endl << endl;
    cout << "1.) Create new index." << endl << endl;
    cout << "2.) Save new index." << endl << endl;
    cout << "3.) Load saved index." << endl << endl;
    cout << "4.) Enter a query." << endl << endl;
    cout << "5.) Show statistics." << endl << endl;
    cout << "-------------------------------------------------" << endl << endl;
    cout << endl;
    cout << "Enter Here: ";
    }

    /**
     * createIndexFormat() function will do the following:
     *    - Simply output the menu that asks for user to enter in the directory WITHOUT quotations.
    */
    void createIndexFormat()
    {
        cout << endl << endl;
        cout << "-------------------------------------------------" << endl << endl << endl << endl;
        cout << "  Please enter the directory you wish to index:  " << endl << endl << endl << endl;
        cout << "-------------------------------------------------" << endl << endl;
        cout << endl;
        cout << "Enter Here: ";
    }

    void createIndexSuccessful()
    {
        cout << endl << "------The directory parsing was successful!------" << endl << endl;
    }

    void saveIndexSuccessful()
    {
        cout << endl << "------The index persistence was successful!------" << endl << endl;
    }

    void loadIndexSuccessful()
    {
        cout << endl << "----The saved index was successfully loaded!-----" << endl << endl;
    }

    /**
     * enterQueryFormat() function will do the following:
     *    - Simply output the menu that asks the user to input their query.
    */
    void enterQueryFormat()
    {
        cout << endl << endl;
        cout << "-------------------------------------------------" << endl << endl << endl << endl;
        cout << "   Please enter the query you wish to search:    " << endl << endl << endl << endl;
        cout << "-------------------------------------------------" << endl << endl;
        cout << endl;
        cout << "Enter Here: ";
    }

    /**
     * printQueryFormat() function will do the following:
     *    - Output the ranked documents using the public print function in Doc.
    */
    void printQueryOutput(vector<Doc>& sortedDocuments)
    {
        int numberDocs = 0;
        cout << endl << endl;
        cout << "-------------------------------------------------" << endl << endl;
        cout << "     [Here are the most relevant documents!]     " << endl << endl;
        if (sortedDocuments.size() == 0) {
            cout << "No results were found!" << endl;
        }

        else if (sortedDocuments.size() < 15) {
            for (size_t i = 1; (i - 1) < sortedDocuments.size(); i++) {
            cout << i << ".)  ";
            sortedDocuments.at(i - 1).printDoc();
            cout << endl;
            numberDocs++;
            }
        }   

        else if (sortedDocuments.size() >= 15) {
            for (size_t i = 1; (i - 1) < 15; i++) {
                cout << i << ".) ";
                sortedDocuments.at(i - 1).printDoc();
                cout << endl;
                numberDocs++;
            }
        }
        cout << endl;
        selectDocument(sortedDocuments, numberDocs);
    }

    void selectDocument(vector<Doc>& documents, int size) {
        string selection;
        selectDocumentPrint();
        getline(cin, selection);

        while (stoi(selection) != 0) {
            if (stoi(selection) < 0 || stoi(selection) > size) {
                cout << "Invalid input. Try again!" << endl;
                selectDocumentPrint();
                getline(cin, selection);
            }
            else {
                cout << "------------------------------------------------------" << endl << endl;
                string text = documents.at(stoi(selection) - 1).rawText;
                for (size_t i = 0; i < text.length(); i++) {
                    if (i % 100 == 0) {
                        cout << endl;
                    }
                    cout << text[i];
                }
                cout << endl << endl << "------------------------------------------------------" << endl << endl;
                selectDocumentPrint();
                getline(cin, selection);
            }
        }
    }

    void selectDocumentPrint() {
        cout << "-[Please enter the number to view, or 0 to exit]-" << endl << endl;
        cout << "Enter Here: ";
    }

    double getTimingIndex() {
        return timeIndexing.count();
    }

    double getTimingQuery() {
        return timeQuery.count();
    }

    void getStats() {
        // Timing for index and query processing getTimingIndex() & getTimingQuery()
        // Total # of documents getDataSize() -> data.size();
        // Total words in AvlTree words -> TreeSize();
        cout << "------------[Here are some statistics]-----------" << endl << endl;
        cout << "Total time for indexing: " << getTimingIndex() << " milliseconds" << endl;
        cout << "Total time for most recent query: " << getTimingQuery() << " milliseconds" << endl;
        cout << "Total number of articles in index: " << ui_docParser.getDataSize() << endl;
        cout << "Total number of words indexed: " << ui_docParser.getTotalWords() << endl << endl;
        cout << "-------------------------------------------------" << endl << endl;
    }

public:
    UserInterface() {
        ih = new IndexHandler;
        ui_docParser.setIndex(ih);
        ui_queryProcessor.setIndex(ih);
    }
    /**
     * mainMenu() function will do the following:
     *    - Output the user interface until exited (user enters 0).
    */
    void mainMenu()
    {
        string userInput;

        mainMenuFormat();
        getline(cin, userInput);

        while (userInput != "0") {
            if (userInput == "1") {
                string userDirectory;

                createIndexFormat();
                getline(cin, userDirectory);

                std::chrono::time_point<std::chrono::high_resolution_clock> startIndexing, endIndexing;
                startIndexing= std::chrono::high_resolution_clock::now();

                createIndex(userDirectory);

                endIndexing=std::chrono::high_resolution_clock::now();
                timeIndexing= endIndexing-startIndexing;

                mainMenuFormat();
                getline(cin, userInput);
            }
            else if (userInput == "2") {
                saveIndex();

                mainMenuFormat();
                getline(cin, userInput);
            }
            else if (userInput == "3") {
                loadSavedIndex();

                mainMenuFormat();
                getline(cin, userInput);
            }
            else if (userInput == "4") {
                string userQuery;
                
                enterQueryFormat();
                getline(cin, userQuery);

                std::chrono::time_point<std::chrono::high_resolution_clock> startQuery, endQuery;
                startQuery= std::chrono::high_resolution_clock::now();

                enterQuery(userQuery);
                endQuery=std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> timeQuery= endQuery-startQuery;

                mainMenuFormat();
                getline(cin, userInput);
            }
            else if (userInput == "5") {
                getStats();

                mainMenuFormat();
                getline(cin, userInput);
            }
            else {
                cout << endl << endl;
                cout << "Invalid input. Try again!" << endl;;

                mainMenuFormat();
                getline(cin, userInput);
            }
        }
        cout << endl;
        cout << "Goodbye!" << endl << endl;
        return;
    }

    /**
     * createIndex() function will do the following:
     *    - Send a given directory to the document parser.
     *        ~ The document parser should do through each file and build the index (either AVL Tree or Hash Table).
    */
    void createIndex(string directory)
    {
        ui_docParser.testFileSystem(directory); // Call the DocParser function that reads the directory and opens each file.
        createIndexSuccessful();
    }

    void saveIndex() { // Call the DocParser function that saves the current index.
        ui_docParser.createPersistence();
        saveIndexSuccessful();
    }

    void loadSavedIndex() {
        ui_docParser.loadPersistence();
        loadIndexSuccessful();
    }

    /**
     * enterQuery() function will do the following:
     *    - Send a given query to the query processor.
    */
    void enterQuery(string query)
    {
        string queryUserInput;
        vector<Doc> sortedDocuments;
        sortedDocuments = ui_queryProcessor.parseQuery(query);
        printQueryOutput(sortedDocuments);
        
    }
};

#endif