#include "include/ConverterJSON.h"
#include "include/InvertedIndex.h"
#include "include/SearchServer.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void loadConfig(const std::string& filename) {
    std::ifstream config_file(filename);
    if (!config_file.is_open()) {
        throw std::runtime_error("Config file is missing");
    }

    json config_data;
    try {
        config_file >> config_data;
    } catch (const std::exception& e) {
        std::cerr << "Error reading configuration file: " << e.what() << std::endl;
    }

    if (!config_data.contains("config")) {
        std::cerr << "Config file is empty!" << std::endl;
        return;
    }

    std::string name = config_data["config"].value("name", "");
    std::string version = config_data["config"].value("version", "");

    if (version != "0.1") {
        std::cerr << "config.json has incorrect file version!" << std::endl;
    }

    std::cout << "Starting " << name << ", version " << version << std::endl;
}

void loadRequest(const std::string& filename) {
    std::ifstream request_file(filename);
    if (!request_file.is_open()) {
        throw std::runtime_error("Request file is missing");
    }

    json request_data;
    try {
        request_file >> request_data;
    } catch (const std::exception& e) {
        std::cerr << "Error reading requests file: " << e.what() << std::endl;
    }
}

void runIndexUpdate(InvertedIndex& index, ConverterJSON& converter, int interval_seconds) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(interval_seconds));
        std::cout << "[AutoUpdate] Re-indexing documents..." << std::endl;
        std::vector<std::string> docs = converter.GetTextDocuments();
        index.UpdateDocumentBase(docs);
        std::cout << "[AutoUpdate] Index updated!" << std::endl;
    }
}

int main() {
    try {
        loadConfig("../bin/config.json");
        loadRequest("../bin/requests.json");

        // 1. Создаём объект ConverterJSON для чтения конфигурации, документов и запросов
        ConverterJSON converter;
        int update_time = converter.GetUpdateTime();
        if (update_time > 0) {
            std::cout << "[Main] Update time: " << update_time << std::endl;
        }

        // 2. Получаем документы, запросы и время обновления
        std::vector<std::string> documents = converter.GetTextDocuments();
        std::vector<std::string> requests = converter.GetRequests();

        // 3. Создаём и заполняем обратный индекс
        InvertedIndex index;
        index.UpdateDocumentBase(documents);

        if (update_time > 0) {
            std::thread update_thread(runIndexUpdate, std::ref(index), std::ref(converter), update_time);
            update_thread.detach();
        }

        // 4. Создаём поисковый сервер
        SearchServer search_server(index);

        // 5. Выполняем поиск по запросам
        std::vector<std::vector<RelativeIndex>> search_results = search_server.search(requests);

        // 6. Сохраняем результаты поиска
        std::vector<std::vector<std::pair<int, float>>> answers;
        for (size_t i = 0; i < search_results.size(); ++i) {
            std::vector<std::pair<int, float>> answer_line;
            for (size_t j = 0; j < search_results[i].size(); ++j) {
                answer_line.push_back(std::make_pair(search_results[i][j].doc_id, search_results[i][j].rank));
            }
            answers.push_back(answer_line);
        }

        converter.putAnswers(answers);
        std::cout << "The search is completed. The results are saved in \"answers.json\".\n";
        if (update_time > 0) {
            std::cout << "\nSearch engine is running in watch mode (auto reindexing enabled)...\n";
            std::cout << "Press Ctrl+C to stop the program.\n";
            while (true) {
                std::this_thread::sleep_for(std::chrono::seconds(10));
                std::cout << "[Main] Waiting...\n";
            }
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
