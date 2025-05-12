//
// Created by ilyap on 03.05.2025.
//

#include "../include/ConverterJSON.h"
#include <iostream>

ConverterJSON::ConverterJSON() {
    loadConfig();
    loadRequests();
}

void ConverterJSON::loadConfig() {
    std::ifstream config_file("../bin/config.json");

    if (!config_file.is_open()) {
        std::cerr << "Config file is missing!" << std::endl;
        return;
    }

    try {
        config_file >> config_read;
    } catch (const std::exception& e) {
        std::cerr << "Error reading configuration file: " << e.what() << std::endl;
    }

    if (!config_read.contains("config")) {
        std::cerr << "Config file is empty!" << std::endl;
        return;
    }

    std::string name = config_read["config"].value("name", "");
    std::string version = config_read["config"].value("version", "");

    if (version != "0.1") {
        std::cerr << "config.json has incorrect file version!" << std::endl;
    }

    std::cout << "Starting " << name << ", version " << version << std::endl;
}

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::vector<std::string> documents;

    for (const auto& doc : config_read["files"]) {
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
    if (config_read.contains("config") && config_read["config"].contains("max_responses")) {
        return config_read["config"]["max_responses"].get<int>();
    }
    return 5;
}

void ConverterJSON::loadRequests() {
    std::ifstream request_file("../bin/requests.json");
    if (!request_file.is_open()) {
        std::cerr << "Request file is missing!" << std::endl;
        return;
    }
    try {
        request_file >> requests_read;
    } catch (const std::exception& e) {
        std::cerr << "Error reading requests file: " << e.what() << std::endl;
    }
}

std::vector<std::string> ConverterJSON::GetRequests() {
    std::vector<std::string> results;

    if (!requests_read.contains("requests")) {
        std::cerr << "Missing or invalid 'requests' section in requests.json!" << std::endl;
        return results;
    }

    for (const auto& request : requests_read["requests"]) {
        results.push_back(request.get<std::string>());
    }
    return results;
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
            answer_JSON["answers"][key]["result"] = "false";
        } else {
            answer_JSON["answers"][key]["result"] = "true";
            json relevance_array = json::array();
            for (const auto& [docid, rank] : answers[i]) {
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