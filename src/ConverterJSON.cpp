//
// Created by ilyap on 03.05.2025.
//

#include "../include/ConverterJSON.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::vector<std::string> documents;
    json config_data;

    std::ifstream config_file("../bin/config.json");
    config_file >> config_data;

    if (!config_data.contains("files")) {
        std::cerr << "'files' section is missing in the config!" << std::endl;
        return documents;
    }

    for (const auto& doc : config_data["files"]) {
        std::ifstream doc_file(doc);
        if (!doc_file.is_open()) {
            std::cerr << "File " << doc << " is missing!" << std::endl;
            continue;
        }
        std::stringstream buffer;
        buffer << doc_file.rdbuf();
        documents.push_back(buffer.str());
    }
    return documents;
}

int ConverterJSON::GetResponsesLimit() {
    std::ifstream config_file("../bin/config.json");
    json config_data;
    config_file >> config_data;
    if (config_data.contains("config") && config_data["config"].contains("max_responses")) {
        return config_data["config"]["max_responses"].get<int>();
    }
    return 5;
}

std::vector<std::string> ConverterJSON::GetRequests() {
    std::vector<std::string> results;
    std::ifstream request_file("../bin/requests.json");
    json request_data;
    request_file >> request_data;
    if (!request_data.contains("requests")) {
        std::cerr << "Missing or invalid 'requests' section in requests.json!" << std::endl;
        return results;
    }

    for (const auto& request : request_data["requests"]) {
        results.push_back(request.get<std::string>());
    }
    return results;
}

int ConverterJSON::GetUpdateTime() {
    std::ifstream config_file("../bin/config.json");
    json config_data;
    config_file >> config_data;

    if (config_data.contains("config") && config_data["config"].contains("update_time")) {
        return config_data["config"]["update_time"].get<int>();
    }
    return 0;
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
    json answer_JSON;

    for (size_t i = 0; i < answers.size(); ++i) {
        std::string request_ID;
        if (i < 10) {
            request_ID = "00" + std::to_string(i + 1);
        } else if (i < 100) {
            request_ID = "0" + std::to_string(i + 1);
        } else {
            request_ID = std::to_string(i + 1);
        }
        std::string key = "request" + request_ID;



        if (answers[i].empty()) {
            answer_JSON["answers"][key]["result"] = false;
            answer_JSON["answers"][key]["relevance"] = json::array();
        } else {
            answer_JSON["answers"][key]["result"] = true;
            json relevance_array = json::array();
            for (size_t j = 0; j < answers[i].size(); ++j) {
                int docid = answers[i][j].first;
                float rank = answers[i][j].second;
                relevance_array.push_back({ {"docid", docid}, {"rank", rank} });
            }
            answer_JSON["answers"][key]["relevance"] = relevance_array;
        }
    }

    std::ofstream answer_file("answers.json");

    if (!answer_file.is_open()) {
        std::cerr << "Answers file is missing!" << std::endl;
        return;
    }
    answer_file << answer_JSON.dump(4);
}