#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "Hashing.h"

TEST_CASE("Check that Hashing.h works", "[Hashing.h]") {
    Hashing<std::string, int> hashTable;

    hashTable.insert("a1", 5, 0);
    hashTable.insert("a2", 6, 1);
    hashTable.insert("a3", 7, 2);
    hashTable.insert("a1", 8, 3);

    REQUIRE(hashTable.getsize() == 3);
    REQUIRE(hashTable.find("a1")[5] == 0);
    REQUIRE(hashTable.find("a2")[6] == 1);
    REQUIRE(hashTable.find("a3")[7] == 2 );
    REQUIRE(hashTable.find("a1")[8] == 3);
    hashTable.clear();
    REQUIRE(hashTable.getsize() == 0);

    //testing rehash function 
    hashTable.createHash(10);
    REQUIRE(hashTable.getcapacity() == 10);
    for (int i = 0; i < 11; i++) {
        hashTable.insert(to_string(i), i, i); 
    }
    REQUIRE(hashTable.getcapacity() == 20);
    REQUIRE(hashTable.getsize() == 11);
    REQUIRE(hashTable.find("0")[0] == 0);
}