#include "ConverterJSON.h"
#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

ConverterJSON::ConverterJSON() {
    loadConfig();
    loadRequests();
}

void ConverterJSON::loadConfig() {
    std::ifstream configFile("config.json");
    if (!configFile.is_open()) {
        throw std::runtime_error("config file is missing");
    }

    json j;
    configFile >> j;
    configFile.close();

    if (j.empty() || !j.contains("config")) {
        throw std::runtime_error("config file is empty");
    }

    auto config = j["config"];
    engineName = config.value("name", "SearchEngine");
    version = config.value("version", "0.1");
    maxResponses = config.value("max_responses", 5);

    if (j.contains("files")) {
        filePaths = j["files"].get<std::vector<std::string>>();
    }

    std::cout << "Starting " << engineName << " v" << version << std::endl;
}

void ConverterJSON::loadRequests() {
    std::ifstream requestsFile("requests.json");
    if (!requestsFile.is_open()) {
        return;
    }

    json j;
    requestsFile >> j;
    requestsFile.close();

    if (j.contains("requests")) {
        requests = j["requests"].get<std::vector<std::string>>();
    }
}

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::vector<std::string> docs;
    for (const auto& path : filePaths) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "File not found: " << path << std::endl;
            continue;
        }
        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        docs.push_back(content);
        file.close();
    }
    return docs;
}

int ConverterJSON::GetResponsesLimit() const {
    return maxResponses;
}

std::vector<std::string> ConverterJSON::GetRequests() {
    return requests;
}

void ConverterJSON::putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers) {
    json j;
    json answersObj;

    for (size_t i = 0; i < answers.size(); ++i) {
        std::string requestId = "request" + std::string(3 - std::to_string(i + 1).length(), '0') + std::to_string(i + 1);
        json requestResult;

        if (answers[i].empty()) {
            requestResult["result"] = false;
        } else {
            requestResult["result"] = true;
            json relevanceArray = json::array();
            for (const auto& [docId, rank] : answers[i]) {
                relevanceArray.push_back({{"docid", docId}, {"rank", rank}});
            }
            requestResult["relevance"] = relevanceArray;
        }
        answersObj[requestId] = requestResult;
    }

    j["answers"] = answersObj;

    std::ofstream answersFile("answers.json");
    answersFile << j.dump(4);
    answersFile.close();
}
