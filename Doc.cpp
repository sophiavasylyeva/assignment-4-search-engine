#include "Doc.h"

// file names to check are passed on as command line arguments
Doc::Doc(string u, string t, string p, string d, int w, string r)
{
    uuid = u;
    title = t;
    publication = p;
    date = d;
    totalWordCount = w;
    rawText = r;
    relevancy = 0;
}

/**
 * This function will be called in the UI to print the document information
 * after the QueryProcessor finds the top relevent articles.
*/
void Doc::printDoc()
{
    cout << this->title << endl;
    cout << "\t" << this->publication << endl;
    cout << "\t" << this->date << endl;
    // FIXME: This needs to print out more information!
}