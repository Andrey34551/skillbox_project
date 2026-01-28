#ifndef CONVERTERJSON_H
#define CONVERTERJSON_H

#include <string>
#include <vector>
#include <map>

class ConverterJSON {
public:
    ConverterJSON();
    std::vector<std::string> GetTextDocuments();
    int GetResponsesLimit() const;
    std::vector<std::string> GetRequests();
    void putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers);
private:
    std::string engineName;
    std::string version;
    int maxResponses = 5;
    std::vector<std::string> filePaths;
    std::vector<std::string> requests;
    void loadConfig();
    void loadRequests();
};

#endif // CONVERTERJSON_H
