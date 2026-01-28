#include "SearchServer.h"
#include <algorithm>
#include <set>
#include <sstream>

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
    std::vector<std::vector<RelativeIndex>> allResults;

    for (const auto& query : queries_input) {
        std::istringstream iss(query);
        std::string word;
        std::vector<std::string> words;
        while (iss >> word) {
            words.push_back(word);
        }

        if (words.empty()) {
            allResults.push_back({});
            continue;
        }

        std::set<std::string> uniqueWords(words.begin(), words.end());
        std::vector<std::pair<std::string, std::vector<Entry>>> sortedWords;

        for (const auto& w : uniqueWords) {
            auto entries = _index.GetWordCount(w);
            sortedWords.emplace_back(w, entries);
        }

        // Сортируем слова по частоте (от редких к частым)
        std::sort(sortedWords.begin(), sortedWords.end(),
                  [](const auto& a, const auto& b) {
                      return a.second.size() < b.second.size();
                  });

        std::map<size_t, float> docRelevance;

        for (size_t i = 0; i < sortedWords.size(); ++i) {
            const auto& entries = sortedWords[i].second;

            if (i == 0) {
                // Для первого (самого редкого) слова
                for (const auto& e : entries) {
                    docRelevance[e.doc_id] = e.count;
                }
            } else {
                // Для остальных слов - пересечение с уже найденными документами
                std::map<size_t, float> newRelevance;
                for (const auto& e : entries) {
                    if (docRelevance.find(e.doc_id) != docRelevance.end()) {
                        newRelevance[e.doc_id] = docRelevance[e.doc_id] + e.count;
                    }
                }
                docRelevance = std::move(newRelevance);
            }

            if (docRelevance.empty()) break;
        }

        // Рассчитываем относительную релевантность
        std::vector<RelativeIndex> result;
        float maxAbsRelevance = 0;

        for (const auto& [docId, absRel] : docRelevance) {
            if (absRel > maxAbsRelevance) {
                maxAbsRelevance = absRel;
            }
        }

        for (const auto& [docId, absRel] : docRelevance) {
            float relRank = maxAbsRelevance > 0 ? absRel / maxAbsRelevance : 0;
            result.push_back({docId, relRank});
        }

        // Сортируем по убыванию релевантности
        std::sort(result.begin(), result.end(),
                  [](const RelativeIndex& a, const RelativeIndex& b) {
                      return a.rank > b.rank;
                  });

        allResults.push_back(result);
    }

    return allResults;
}
