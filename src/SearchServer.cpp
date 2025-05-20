//
// Created by ilyap on 07.05.2025.
//

#include "../include/SearchServer.h"

#include <algorithm>
#include <set>
#include <sstream>
#include <string>
#include <fstream>
#include "../nlohmann_json/include/nlohmann/json.hpp"

#include "../include/ConverterJSON.h"

using json = nlohmann::json;

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &queries_input) {
    std::vector<std::vector<RelativeIndex>> results;

    for (const std::string &query : queries_input) {
        std::map<size_t, size_t> doc_absolute_relevance;
        std::set<std::string> unique_word;
        std::istringstream stream(query);
        std::string word;

        //1.Разбиваем запрос на уникальные слова
        while (stream >> word) {
            unique_word.insert(word);
        }

        //2. Обрабатываем каждое слово
        for (const auto& word : unique_word) {
            std::vector<Entry> entries = index.GetWordCount(word);
            for (const auto& entry : entries) {
                doc_absolute_relevance[entry.doc_ID] += entry.count;
            }
        }

        if (doc_absolute_relevance.empty()) {
            results.push_back({});
            continue;
        }

        //3. Находим максимальную абсолютную релевантность
        size_t max_relevance = 0;
        for (std::map<size_t, size_t>::const_iterator it = doc_absolute_relevance.begin(); it != doc_absolute_relevance.end(); ++it) {
            if (it->second > max_relevance) {
                max_relevance = it->second;
            }
        }

        //4. Формируем список результатов
        std::vector<RelativeIndex> relative_indices;
        for (std::map<size_t, size_t>::const_iterator it = doc_absolute_relevance.begin(); it != doc_absolute_relevance.end(); ++it) {
            RelativeIndex ri;
            ri.doc_id = it->first;
            ri.rank = static_cast<float>(it->second) / static_cast<float>(max_relevance);
            relative_indices.push_back(ri);
        }

        //5. Сортировка по убыванию релевантности
        std::sort(relative_indices.begin(), relative_indices.end(), [](const RelativeIndex& a, const RelativeIndex& b) {
            if (a.rank != b.rank) {
                return a.rank > b.rank;
            }
            return a.doc_id < b.doc_id;
        });

        ConverterJSON converter;
        int max_responses = converter.GetResponsesLimit();
        if (relative_indices.size() > static_cast<size_t>(max_responses)) {
            relative_indices.resize(max_responses);
        }
        results.push_back(relative_indices);
    }
    return results;
}