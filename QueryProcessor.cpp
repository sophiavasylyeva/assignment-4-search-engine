#include "QueryProcessor.h"


std::vector<Doc> QueryProcessor::parseQuery(std::string x)                  //parse query and decipher, call getter functions
{
   std::stringstream y(x);
   std::vector<std::string> words;
   std::vector<Doc> finalDocs;
   std::string temp;
   while(getline(y, temp, ' '))
    {
        words.push_back(temp);
    }
    vector<pair<Doc, int>> allDocs = index->getWords(words[0]);
    for(size_t i = 1; i < words.size();i++){
        if(words[i].length() > 4 && words[i].substr(0,4) == "ORG:") {                    //organization  //determining what type of term it is 
            string term = words[i].substr(4, words[i].length() - 4);                     //starting to read after substring is removed
            vector<pair<Doc, int>> docs = index->getOrganizations(term);           //call function from indexHandler and setting it equal to vector
            allDocs = intersection(allDocs,docs); 


        }
        else if(words[i].length() > 7 && words[i].substr(0, 7) == "PERSON:"){           //person
            string term = words[i].substr(7,words[i].length() - 7);
            vector<pair<Doc, int>> docs = index->getPeople(term);
            allDocs = intersection(allDocs,docs); 


        }
        else if(words[i].length() > 1 && words[i].substr(0,1) == "-"){                  //not term ignoring -
            string term = words[i].substr(1, words[i].length() - 1);
            vector<pair<Doc, int>> docs = index->getWords(term);
            allDocs = complement(allDocs,docs); 

        }
        else{
            string term = words[i];
            vector<pair<Doc, int>> docs = index->getWords(term);
            allDocs = intersection(allDocs, docs);
        }
    }

    allDocs = Relevency(allDocs);          
    for (size_t i = 0 ; i < allDocs.size(); i++){
        finalDocs.push_back(allDocs[i].first);
    }
    sort(finalDocs);
    return finalDocs;
}
std::vector<std::pair<Doc,int>> QueryProcessor::intersection(vector<pair<Doc,int>> docs1,vector<pair<Doc,int>> docs2){ //venn diagram comparing A and B focuses on what has both
    vector<pair<Doc,int>> final; //result of intersection
    for(size_t i = 0; i < docs1.size(); i++){ 
        for(size_t j = 0; j < docs2.size(); j++){
            if(docs1[i].first.uuid == docs2[j].first.uuid){
                final.push_back(docs1[i]);
                final[final.size()-1].second += docs2[j].second; //pushing back Docs from docs 1 
                break;
            }
        }
    }
    return final;
}
std::vector<std::pair<Doc,int>> QueryProcessor::complement(vector<pair<Doc,int>> docs1,vector<pair<Doc,int>> docs2){  //ven diagram only focuses on A not both or B
    vector<pair<Doc,int>> final; //result of intersection
    for(size_t i = 0; i < docs1.size(); i++){ 
        bool found = false;
        for(size_t j = 0; j < docs2.size(); j++){
            if(docs1[i].first.uuid == docs2[j].first.uuid){  //keeping track if the value does exit
                found = true;
                break;
            }
        }
        if(!found){
            final.push_back(docs1[i]);
        }
    }
    return final;
}
std::vector<std::pair<Doc,int>> QueryProcessor::Relevency(vector<pair<Doc,int>> docs){  //calculate tf -idf
    for(size_t i = 0; i < docs.size(); i++){
       double tf = (double)docs[i].second/ docs[i].first.totalWordCount;            //N word,doc / N doc - formula fo tf idf 
       double idf = log2((double)index->getNumDocs()/ docs.size());  
       docs[i].first.relevancy = tf*idf;
    }
// sort docs by docs[i].first.relevancy
    return docs;


}
std::vector<Doc> QueryProcessor::sort(std::vector<Doc> vec)  //merge sort is better 
    {   
        if(vec.size() <= 1 ){
            return vec;
        }
        int middle = vec.size()/ 2;
        vector<Doc> first(vec.begin(),vec.begin()+middle);
        vector<Doc> second(vec.begin()+ middle,vec.end());
        first = sort(first);
        second = sort(second);
        return merge(first,second);
    }


std::vector<Doc> QueryProcessor::merge(vector<Doc> first,vector<Doc> second)
{
     vector<Doc> merged;
     size_t it1 = 0; //keeping track of where we are in subarray
     size_t it2 = 0;
    while(it1 < first.size() || it2 < second.size()){
        if(it1 == first.size()){
            merged.push_back(second[it2]);
            it2++;
        }
        else if(it2 == second.size()){
            merged.push_back(first[it1]);
            it1++;

        }
        else if(first[it1].relevancy > second[it2].relevancy){
            merged.push_back(first[it1]);
            it1++;
        }
        else{
            merged.push_back(second[it2]);
            it2++;

        }
    }
    return merged;
}

void QueryProcessor::setIndex(IndexHandler* i){
    index = i;
}
