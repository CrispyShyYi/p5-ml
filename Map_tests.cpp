#include "Map.hpp"
#include <string>
#include <vector>
#include <iostream>
#include "unit_test_framework.hpp"

using std::pair;
using std::string;
using std::vector;

TEST(test_empty) {
    Map<string, double> words;
    ASSERT_TRUE(words.empty());
    ASSERT_TRUE(words.size() == 0);
    ASSERT_TRUE(words.find("pi") == words.end());

    words["three"] = 3;
    
}



TEST_MAIN()
