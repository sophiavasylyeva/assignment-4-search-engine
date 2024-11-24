#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "QueryProcessor.h"
#include "DocParser.h"
#include "Doc.h"
#include "IndexHandler.h"

TEST_CASE("Check that QueryProcessor.h works", "[QueryProcessor.h]") {
    //mimic architecture, shared indexhandler pointer with docparser and queryengine
    //run docparser on sample data
    //hardcode few queries and run with queryengine
    //verify answers (use persistence as key), test all cases

    IndexHandler* index = new IndexHandler;
    DocParser d;
    d.setIndex(index);
    d.testReadJsonFile("../sample_data");
    QueryProcessor q;
    q.setIndex(index);
    vector<Doc> results;
    results = q.parseQuery("ago");
    REQUIRE(results.size() == 1);
    REQUIRE(results[0].uuid == "5293252386bb424828d17b0ac60299cb52a8ecb9");
    results = q.parseQuery("common ORG:reuters");
    REQUIRE(results.size() == 1);
    REQUIRE(results[0].uuid == "e17252151a2d36344b7104d14805414a55042abe");
    results = q.parseQuery("common PERSONS:schweitzer");
    REQUIRE(results.size() == 2);
    REQUIRE(results[0].uuid == "e17252151a2d36344b7104d14805414a55042abe");
    results = q.parseQuery("compani -compar");
    REQUIRE(results.size() == 1);
    REQUIRE(results[0].uuid == "e17252151a2d36344b7104d14805414a55042abe");



}