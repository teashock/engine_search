//
// Created by ilyap on 07.05.2025.
//

#ifndef SEARCHSERVER_H
#define SEARCHSERVER_H

#include "InvertedIndex.h"
#include <vector>
#include <string>

struct RelativeIndex {
    size_t doc_id;
    float rank;

    bool operator ==(const RelativeIndex &other) const {
        return doc_id == other.doc_id && abs(rank - other.rank) < 1e-6;
    }
};

class SearchServer {
public:
    /**
     * @param  idx  в конструктор класса передается ссылка на класс Inverted Index,
     * чтобы SearchServer мог узнать частоту слов встречаемых в запросе
     */
    SearchServer(InvertedIndex& idx) : index(idx) {};

    /**
     * Метод обработки поисковых запросов
     * @param queries_input поисковые запросы
     * взятые из файл requests.json
     * @return возвращает отсортированный список релевантных ответов для заданных запросов
     */
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);

private:
    InvertedIndex& index;
};

#endif //SEARCHSERVER_H
