#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "DS_AVLTree.h"
#include <string>
#include <map>

TEST_CASE("Check that DS_AVLTree.h works", "[DS_AVLTree]") {

    /**
     * Create a new AVL Tree called Tree1...
     *    - The first string is the word that will be put into the AVL Tree.
     *    - The second string is the document name.
     *    - The integer is the relevancy number for the specific document.
    */
    AvlTree<string, int> Words;

    Words.insert("hello", 5500, 6); // The word "hello" was written 6 times in a document found in position 5500 of the "data" vector
    Words.insert("banks", 2200, 10); // The word "banks" was written 10 times in a document found in position 2200 of the "data" vector
    Words.insert("money", 6610, 13); // The word "money" was written 13 times in a document found in position 6610 of the "data" vector

    map<int, int> tempMap; // TempDcoument used for comparison below...
    tempMap[5500] = 6;

    REQUIRE(Words.find("hello") == tempMap); // Determine if the find function returns the map containing the position "data" position integer and the relevancy number.

    Words.makeEmpty(); // Remove all information from the AvlTree "Words" (This will be used for persistence)

    REQUIRE(Words.treeSize() == 0);
    REQUIRE(Words.isEmpty() == true);
}