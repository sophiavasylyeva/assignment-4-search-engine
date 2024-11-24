#ifndef QueryProcessor_H
#define QueryProcessor_H


#define DEBUG

#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <vector>
#include <math.h>
#include <sstream>
#include "Doc.h"
#include "IndexHandler.h"


class QueryProcessor{
private:
    IndexHandler* index;
    std::vector<std::string> storage;

public:
    vector<Doc> parseQuery(std::string);  //parse query and decipher, call getter functions
    vector<pair<Doc,int>> intersection(vector<pair<Doc,int>>,vector<pair<Doc,int>>);
    vector<pair<Doc,int>> complement(vector<pair<Doc,int>>,vector<pair<Doc,int>>);
    vector<pair<Doc,int>> Relevency(vector<pair<Doc,int>>);
    vector<Doc> sort(vector<Doc>);
    vector<Doc> merge(vector<Doc> first,vector<Doc> second);
    void setIndex(IndexHandler*);



};
#endif