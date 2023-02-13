cmake_minimum_required(VERSION 3.15)
project(search-engine-pointerpals)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS_DEBUG "-O3")

add_executable(search-engine-pointerpals main.cpp AVLTree.h HashTable.h Document.h Document.cpp DocumentParser.cpp DocumentParser.h IndexHandler.cpp IndexHandler.h Index.cpp Index.h Entry.cpp Entry.h Porter2/porter2_stemmer.cpp Porter2/porter2_stemmer.h SearchQuery.cpp SearchQuery.h SearchEngine.cpp SearchEngine.h)