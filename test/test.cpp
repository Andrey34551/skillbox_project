#include "gtest/gtest.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include <iostream>

// Тест для InvertedIndex
TEST(InvertedIndexTest, Basic) {
    std::vector<std::string> docs = {
        "London is the capital of great britain",
        "big ben is the nickname for the Great bell of the striking clock"
    };

    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);

    auto result1 = idx.GetWordCount("London");
    ASSERT_EQ(result1.size(), 1);
    EXPECT_EQ(result1[0].doc_id, 0);
    EXPECT_EQ(result1[0].count, 1);

    auto result2 = idx.GetWordCount("the");
    ASSERT_EQ(result2.size(), 2);
    EXPECT_EQ(result2[0].doc_id, 0);
    EXPECT_EQ(result2[0].count, 1);
    EXPECT_EQ(result2[1].doc_id, 1);
    EXPECT_EQ(result2[1].count, 3);
}

// Тест для SearchServer
TEST(SearchServerTest, Simple) {
    std::vector<std::string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk water water water water water",
        "americano cappuccino"
    };

    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);

    SearchServer srv(idx);
    std::vector<std::string> request = {"milk water", "sugar"};
    auto results = srv.search(request);

    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0].size(), 3);
    EXPECT_EQ(results[1].size(), 0);

    // Проверяем релевантность
    if (!results[0].empty()) {
        EXPECT_EQ(results[0][0].doc_id, 2);  // Самый релевантный
        EXPECT_FLOAT_EQ(results[0][0].rank, 1.0f);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
