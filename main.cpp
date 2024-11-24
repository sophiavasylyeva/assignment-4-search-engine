#include <iostream>
#include <string>
#include "User_Interface.h"

using namespace std;

int main(int argc, char *argv[])
{
    UserInterface ui;
    string mode;

    /**
     * If the command-line argument does not contain a mode, throw an error and close the program.
    */
    if (argc == 1) {
        cout << "Error opening the program!" << endl;
        cout << "Please use any of the following command line arguments:" << endl;
        cout << "./supersearch index <your directory>" << endl;
        cout << "./supersearch query <your query>" << endl;
        cout << "./superearch ui" << endl;
        return -1;
    }

    mode = argv[1]; // Determine what mode will be executed from the bash command-line code.

    /**
     * If the mode is "index", do the following:
     *    - Create the index from the given directory in command line (argv[2]).
     *    - Save the index in a file (persistence).
    */
    if (mode == "index") {
        string directory = argv[2];
        ui.createIndex(directory);
        ui.saveIndex();
    }

    /**
     * If the mode is "query", do the following:
     *    - Load the exisiting index from the saved file.
     *    - Send the query command to the query processor.
    */
    if (mode == "query") {
        string queryCommand;
        for (int i = 2; i < argc; i++) {
            queryCommand += argv[i];
            queryCommand += ' ';
        }
        ui.loadSavedIndex();
        ui.enterQuery(queryCommand);
    }

    /**
     * If the mode is "ui", do the following:
     *    - Opens the user interface, which will only close once (0) is selected.
     *        ~ The user interface will allow the user to manually enter in an index.
     *        ~ The user interface will allow the user to manually enter in a query.
    */
    if (mode == "ui") {
        ui.mainMenu();
    }

    return 0;
}