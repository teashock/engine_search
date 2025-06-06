//
// Created by ilyap on 06.05.2025.
//

#include "../include/InvertedIndex.h"
#include <sstream>
#include <thread>

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = input_docs;
    freq_dictionary.clear();

    std::vector<std::thread> threads;

    // 1. Обработка документов по потокам
    for (size_t docId = 0; docId < input_docs.size(); ++docId) {
        threads.emplace_back([this, docId, &input_docs]() {
            std::map<std::string, size_t> wordCount;
            std::istringstream iss(input_docs[docId]);
            std::string word;

            while (iss >> word) {
                ++wordCount[word];
            }

            std::lock_guard<std::mutex> lock(freq_mutex);
            for (std::map<std::string, size_t>::const_iterator it = wordCount.begin(); it != wordCount.end(); ++it) {
                freq_dictionary[it->first].push_back(Entry{docId, it->second});
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {

    std::lock_guard<std::mutex> lock(freq_mutex);
    auto it = freq_dictionary.find(word);
    return it != freq_dictionary.end() ? it->second : std::vector<Entry>();
}
