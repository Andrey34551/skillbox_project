#include "InvertedIndex.h"
#include <sstream>
#include <algorithm>
#include <thread>

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& input_docs) {
    docs = input_docs;
    freq_dictionary.clear();

    std::vector<std::thread> threads;
    for (size_t i = 0; i < docs.size(); ++i) {
        threads.emplace_back(&InvertedIndex::indexDocument, this, i, std::ref(docs[i]));
    }

    for (auto& t : threads) {
        t.join();
    }
}

void InvertedIndex::indexDocument(size_t doc_id, const std::string& text) {
    std::istringstream iss(text);
    std::string word;
    std::map<std::string, size_t> wordCount;

    while (iss >> word) {
        if (word.length() > 100) continue;
        wordCount[word]++;
    }

    std::lock_guard<std::mutex> lock(dict_mutex);
    for (const auto& [w, count] : wordCount) {
        freq_dictionary[w].push_back({doc_id, count});
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    auto it = freq_dictionary.find(word);
    if (it != freq_dictionary.end()) {
        return it->second;
    }
    return {};
}
