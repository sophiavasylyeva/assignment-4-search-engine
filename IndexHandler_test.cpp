#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "IndexHandler.h"
#include "Doc.h"

TEST_CASE("Check that IndexHandler.cpp works", "[IndexHandler]") {
    IndexHandler index; // Create an indexHandler object named index

    /**
     * Let's pretend the DocParser read through the document named "Doc1_title".
     * This document contained the word "hello" 3 times, which are the only words in the raw text.
     * Before the Doc object that holds Doc1_title's information is created, we push back the word it found with its relevency.
    */
    index.addWord("hello", 3);

    /**
     * Now the Doc object we call Document will be pushed back into the "data" vector in the internal storage of IndexHandler
    */
    Doc Document1("Doc1_uuid", "Doc1_title", "Doc1_publication", "Doc1_date", 3, "hello hello hello");
    index.addDoc(Document1);

    /**
     * There should be only 1 Doc object in IndexHandler's "data".
    */
    REQUIRE(index.getNumDocs() == 1);

    vector<pair<Doc,int>> tempMap = index.getWords("hello");
    REQUIRE(tempMap.size() == 1);
    REQUIRE(tempMap.at(0).first.uuid == "Doc1_uuid");
    REQUIRE(tempMap.at(0).first.title == "Doc1_title");
    REQUIRE(tempMap.at(0).first.publication == "Doc1_publication");
    REQUIRE(tempMap.at(0).first.date == "Doc1_date");
    REQUIRE(tempMap.at(0).first.totalWordCount == 3);
    REQUIRE(tempMap.at(0).first.rawText == "hello hello hello");
    REQUIRE(tempMap.at(0).second == 3);
}