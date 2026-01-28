#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include <iostream>

int main() {
    try {
        ConverterJSON converter;
        InvertedIndex index;
        index.UpdateDocumentBase(converter.GetTextDocuments());

        SearchServer server(index);
        auto requests = converter.GetRequests();
        auto results = server.search(requests);

        std::vector<std::vector<std::pair<int, float>>> answers;
        for (const auto& resultList : results) {
            std::vector<std::pair<int, float>> docRanks;
            for (const auto& relIndex : resultList) {
                docRanks.emplace_back(relIndex.doc_id, relIndex.rank);
            }
            answers.push_back(docRanks);
        }

        converter.putAnswers(answers);
        std::cout << "Search completed. Results saved to answers.json" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
