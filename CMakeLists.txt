cmake_minimum_required(VERSION 3.15)
project(FinalProjExamples)

# filesystem.h require C++17
set(CMAKE_CXX_STANDARD 17)

file(COPY sample_data/ DESTINATION sample_data/)
file(COPY archive/ DESTINATION archive/)

# show compiler output and enable warnings
set(CMAKE_VERBOSE_MAKEFILE ON)
add_compile_options(-Wall -Wextra -pedantic)

add_executable(rapidJSONExample rapidJSONExample.cpp)
add_executable(supersearch main.cpp Doc.cpp IndexHandler.cpp porter2_stemmer.cpp QueryProcessor.cpp)

# this makes sure we also link rapidjson
target_include_directories(rapidJSONExample PRIVATE rapidjson/)

set(CMAKE_VERBOSE_MAKEFILE OFF)

include(CTest)
add_executable(tests_AVLTree DS_AVLTree_test.cpp)
add_test(NAME testAVLTree COMMAND tests_AVLTree)
add_executable(tests_IndexHandler IndexHandler_test.cpp IndexHandler.cpp Doc.cpp)
add_test(NAME testIndexHandler COMMAND tests_IndexHandler)
add_executable(tests_Hashing Hashing_tester.cpp)
add_test(NAME testHashing COMMAND tests_Hashing)
add_executable(tests_QueryProcessor QueryProcessor_tester.cpp Doc.cpp IndexHandler.cpp porter2_stemmer.cpp QueryProcessor.cpp)
add_test(NAME testQueryProcessor COMMAND tests_QueryProcessor)
