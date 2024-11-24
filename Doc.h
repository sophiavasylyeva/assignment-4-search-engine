#ifndef Doc_H
#define Doc_H
#include <iostream>
using namespace std;

class Doc{
    public:
        string uuid;
        string title;
        string publication;
        string date;
        int totalWordCount;
        string rawText;
        double relevancy;
        Doc(string u, string t, string p, string d, int w, string r);

        void printDoc();
    private:
};

#endif